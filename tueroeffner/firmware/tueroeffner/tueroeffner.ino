// Türöffner – ESP32 (Arduino-Core 3.x), keine Zusatzbibliotheken nötig.
// Haustür: Relais/PhotoMOS parallel zur Türöffner-Taste der Gegensprechanlage.
// Wohnungstür: Servo drückt die Klinke herunter.
// Bedienung über Webseite im lokalen WLAN, Anmeldung per persönlicher PIN.

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <time.h>
#include <esp_timer.h>
#include <esp_random.h>
#include "config.h"
#include "webseite.h"

WebServer server(80);

// ---------- Zeit ----------
static uint64_t jetztMs() { return esp_timer_get_time() / 1000ULL; }  // läuft nicht nach 49 Tagen über

static String uhrzeit() {
  struct tm t;
  if (!getLocalTime(&t, 10)) return "--:--";
  char buf[20];
  strftime(buf, sizeof(buf), "%d.%m. %H:%M", &t);
  return buf;
}

// ---------- Protokoll ----------
struct Eintrag { String zeit, name, aktion; };
static Eintrag protokoll[20];
static int protokollAnzahl = 0;

static void protokolliere(const String& name, const String& aktion) {
  for (int i = min(protokollAnzahl, 19); i > 0; i--) protokoll[i] = protokoll[i - 1];
  protokoll[0] = { uhrzeit(), name, aktion };
  if (protokollAnzahl < 20) protokollAnzahl++;
  Serial.printf("[%s] %s: %s\n", protokoll[0].zeit.c_str(), name.c_str(), aktion.c_str());
}

// ---------- Sitzungen ----------
struct Sitzung { char token[33]; int benutzer; uint64_t ablauf; };
static Sitzung sitzungen[MAX_SITZUNGEN];

static void neuesToken(char* out) {
  for (int i = 0; i < 4; i++) sprintf(out + i * 8, "%08lx", (unsigned long)esp_random());
  out[32] = 0;
}

static bool gleich(const char* a, const char* b) {  // Vergleich in konstanter Zeit
  size_t la = strlen(a), lb = strlen(b);
  uint8_t diff = la != lb;
  for (size_t i = 0; i < la; i++) diff |= a[i] ^ b[i % (lb ? lb : 1)];
  return diff == 0;
}

static int sitzungAnlegen(int benutzer) {
  int frei = 0;
  uint64_t aeltestes = UINT64_MAX;
  for (int i = 0; i < MAX_SITZUNGEN; i++) {
    if (sitzungen[i].ablauf < aeltestes) { aeltestes = sitzungen[i].ablauf; frei = i; }
  }
  neuesToken(sitzungen[frei].token);
  sitzungen[frei].benutzer = benutzer;
  sitzungen[frei].ablauf = jetztMs() + (uint64_t)SITZUNG_TAGE * 24 * 3600 * 1000;
  return frei;
}

static String cookieToken() {
  if (!server.hasHeader("Cookie")) return "";
  String c = server.header("Cookie");
  int p = c.indexOf("sid=");
  if (p < 0) return "";
  int e = c.indexOf(';', p);
  return c.substring(p + 4, e < 0 ? c.length() : e);
}

// liefert Benutzer-Index oder -1
static int angemeldeterBenutzer() {
  String t = cookieToken();
  if (t.length() != 32) return -1;
  for (auto& s : sitzungen) {
    if (s.ablauf > jetztMs() && gleich(s.token, t.c_str())) return s.benutzer;
  }
  return -1;
}

// ---------- Schutz gegen PIN-Raten ----------
static int fehlversuche = 0;
static uint64_t gesperrtBis = 0;
static uint64_t naechsteSperre = SPERRE_START_MS;

// ---------- Aktoren (nicht blockierend) ----------
static uint64_t relaisAusUm = 0;
enum ServoZustand { SERVO_AUS, SERVO_UNTEN, SERVO_ZURUECK };
static ServoZustand servoZustand = SERVO_AUS;
static uint64_t servoNaechsterSchritt = 0;

static void relais(bool an) {
  digitalWrite(PIN_RELAIS_HAUSTUER, an == RELAIS_AKTIV_HIGH ? HIGH : LOW);
}

static bool servoAktiv = false;

static void servoPuls(uint32_t us) {
  if (!servoAktiv) { ledcAttach(PIN_SERVO, 50, 14); servoAktiv = true; }  // 50 Hz, 14 Bit
  ledcWrite(PIN_SERVO, (uint32_t)((uint64_t)us * 16384 / 20000));
}

static void servoLos() {
  if (servoAktiv) { ledcDetach(PIN_SERVO); servoAktiv = false; }  // kein Halten -> kein Brummen, Klinke frei
  pinMode(PIN_SERVO, OUTPUT);
  digitalWrite(PIN_SERVO, LOW);
}

static void haustuerOeffnen() {
  relais(true);
  relaisAusUm = jetztMs() + HAUSTUER_PULS_MS;
}

static void wohnungstuerOeffnen() {
  servoPuls(SERVO_GEDRUECKT_US);
  servoZustand = SERVO_UNTEN;
  servoNaechsterSchritt = jetztMs() + SERVO_HALTEN_MS;
}

static void aktorenAktualisieren() {
  uint64_t n = jetztMs();
  if (relaisAusUm && n >= relaisAusUm) { relais(false); relaisAusUm = 0; }
  if (servoZustand == SERVO_UNTEN && n >= servoNaechsterSchritt) {
    servoPuls(SERVO_RUHE_US);
    servoZustand = SERVO_ZURUECK;
    servoNaechsterSchritt = n + 600;             // Zeit zum Zurückfahren
  } else if (servoZustand == SERVO_ZURUECK && n >= servoNaechsterSchritt) {
    servoLos();
    servoZustand = SERVO_AUS;
  }
}

// ---------- HTTP ----------
static void json(int code, const String& body) {
  server.sendHeader("Cache-Control", "no-store");
  server.send(code, "application/json", body);
}

static String esc(const String& s) {
  String o;
  for (char c : s) { if (c == '"' || c == '\\') o += '\\'; o += c; }
  return o;
}

static void seite() {
  server.sendHeader("Cache-Control", "no-store");
  server.send_P(200, "text/html; charset=utf-8", WEBSEITE);
}

static void status() {
  int b = angemeldeterBenutzer();
  String s = "{\"angemeldet\":" + String(b >= 0 ? "true" : "false");
  if (b >= 0) {
    s += ",\"name\":\"" + esc(BENUTZER[b].name) + "\",\"protokoll\":[";
    for (int i = 0; i < protokollAnzahl; i++) {
      if (i) s += ",";
      s += "{\"zeit\":\"" + esc(protokoll[i].zeit) + "\",\"name\":\"" + esc(protokoll[i].name) +
           "\",\"aktion\":\"" + esc(protokoll[i].aktion) + "\"}";
    }
    s += "]";
  }
  json(200, s + "}");
}

static void login() {
  uint64_t n = jetztMs();
  if (n < gesperrtBis) {
    json(429, "{\"fehler\":\"Zu viele Fehlversuche. Gesperrt für " + String((uint32_t)((gesperrtBis - n) / 60000) + 1) + " min.\"}");
    return;
  }
  String pin = server.arg("pin");
  int treffer = -1;
  for (int i = 0; i < (int)(sizeof(BENUTZER) / sizeof(BENUTZER[0])); i++) {
    if (gleich(pin.c_str(), BENUTZER[i].pin)) treffer = i;  // bewusst ohne break
  }
  if (treffer < 0) {
    if (++fehlversuche >= MAX_FEHLVERSUCHE) {
      gesperrtBis = n + naechsteSperre;
      naechsteSperre = naechsteSperre * 2 > SPERRE_MAX_MS ? SPERRE_MAX_MS : naechsteSperre * 2;
      fehlversuche = 0;
      protokolliere("?", "Login gesperrt nach Fehlversuchen");
    }
    json(401, "{\"fehler\":\"PIN falsch.\"}");
    return;
  }
  fehlversuche = 0;
  naechsteSperre = SPERRE_START_MS;
  int s = sitzungAnlegen(treffer);
  server.sendHeader("Set-Cookie", String("sid=") + sitzungen[s].token +
                    "; Path=/; Max-Age=" + String(SITZUNG_TAGE * 86400) + "; HttpOnly; SameSite=Strict");
  protokolliere(BENUTZER[treffer].name, "angemeldet");
  json(200, "{\"ok\":true}");
}

static void logout() {
  String t = cookieToken();
  for (auto& s : sitzungen) if (t.length() == 32 && gleich(s.token, t.c_str())) s.ablauf = 0;
  server.sendHeader("Set-Cookie", "sid=; Path=/; Max-Age=0; HttpOnly; SameSite=Strict");
  json(200, "{\"ok\":true}");
}

static void oeffnen() {
  int b = angemeldeterBenutzer();
  if (b < 0) { json(401, "{\"fehler\":\"Bitte erst anmelden.\"}"); return; }
  String ziel = server.arg("tuer");
  if (ziel == "haus") {
    haustuerOeffnen();
    protokolliere(BENUTZER[b].name, "Haustür");
  } else if (ziel == "wohnung") {
    if (servoZustand != SERVO_AUS) { json(409, "{\"fehler\":\"Läuft schon.\"}"); return; }
    wohnungstuerOeffnen();
    protokolliere(BENUTZER[b].name, "Wohnungstür");
  } else {
    json(400, "{\"fehler\":\"Unbekannte Tür.\"}");
    return;
  }
  json(200, "{\"ok\":true}");
}

// ---------- Setup / Loop ----------
void setup() {
  Serial.begin(115200);
  pinMode(PIN_RELAIS_HAUSTUER, OUTPUT);
  relais(false);
  servoLos();

  WiFi.mode(WIFI_STA);
  WiFi.setHostname(MDNS_NAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("WLAN verbinden");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.printf("\nIP: %s\n", WiFi.localIP().toString().c_str());

  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "fritz.box", "pool.ntp.org");
  if (MDNS.begin(MDNS_NAME)) MDNS.addService("http", "tcp", 80);

  const char* kopfzeilen[] = { "Cookie" };
  server.collectHeaders(kopfzeilen, 1);
  server.on("/", HTTP_GET, seite);
  server.on("/api/status", HTTP_GET, status);
  server.on("/api/login", HTTP_POST, login);
  server.on("/api/logout", HTTP_POST, logout);
  server.on("/api/oeffnen", HTTP_POST, oeffnen);
  server.onNotFound([] { server.send(404, "text/plain", "nicht gefunden"); });
  server.begin();
}

void loop() {
  server.handleClient();
  aktorenAktualisieren();
  static uint64_t letzterVersuch = 0;              // WLAN weg? alle 10 s neu verbinden
  if (WiFi.status() != WL_CONNECTED && jetztMs() - letzterVersuch > 10000) {
    letzterVersuch = jetztMs();
    WiFi.reconnect();
  }
  delay(2);
}

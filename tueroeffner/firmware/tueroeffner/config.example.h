#pragma once

// ---------- WLAN ----------
#define WIFI_SSID     "DEIN-WLAN"
#define WIFI_PASSWORD "DEIN-WLAN-PASSWORT"
#define MDNS_NAME     "tuer"            // erreichbar als http://tuer.local

// ---------- Pins ----------
#define PIN_RELAIS_HAUSTUER 26          // -> IN des Relaismoduls (VCC des Moduls an 3V3/5V, NICHT an einen GPIO)
#define PIN_SERVO           25          // Signalleitung Servo
#define RELAIS_AKTIV_HIGH   false       // Low-Level-Trigger (verbautes Modul). High-Level-Modul: true

// ---------- Haustür ----------
#define HAUSTUER_PULS_MS 3000           // so lange wird "die Taste gedrückt"

// ---------- Wohnungstür (Servo) ----------
// Pulsbreiten in Mikrosekunden; beim ersten Test vorsichtig anpassen!
#define SERVO_RUHE_US      1000         // Arm liegt frei über der Klinke
#define SERVO_GEDRUECKT_US 1900         // Klinke ganz unten
#define SERVO_HALTEN_MS    5000         // Zeit zum Aufdrücken der Tür

// ---------- Benutzer ----------
// Eine Zeile pro Person: { "Name", "PIN" }. PIN mind. 6 Ziffern.
struct Benutzer { const char* name; const char* pin; };
static const Benutzer BENUTZER[] = {
  { "Moritz", "123456" },
  { "Gast",   "654321" },
};

// ---------- Schutz ----------
#define MAX_FEHLVERSUCHE   5
#define SPERRE_START_MS    (5UL * 60 * 1000)   // 5 min, verdoppelt sich je Sperre
#define SPERRE_MAX_MS      (60UL * 60 * 1000)  // max. 1 h
#define SITZUNG_TAGE       90                  // so lange bleibt ein Handy angemeldet
#define MAX_SITZUNGEN      24

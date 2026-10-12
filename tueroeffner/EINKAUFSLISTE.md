# Einkaufsliste Türöffner

Konkrete Teile zum Bestellen, aufgeteilt in Bestellwellen. Grundlage ist [KONZEPT.md](KONZEPT.md): ESP32 mit PhotoMOS am Ritto-TwinBus-Wohntelefon 4630/01 (Haustür) und starkem Servo an der Klinke (Wohnungstür).

Preise sind **Richtwerte** (Stand Oktober 2026, Brutto, ohne Versand) und schwanken je Händler stark. Mengenangaben sind so gewählt, dass immer ein Ersatzteil da ist — beim Löten an einer fremden Anlage willst du nicht wegen eines gegrillten Bauteils zwei Wochen warten.

> **Wichtig: nicht alles auf einmal bestellen.** Der Servo ist der teuerste Einzelposten und die richtige Größe steht erst nach dem Messen fest. Der PhotoMOS passt nur, wenn die Spannung am Taster im erwarteten Bereich liegt. Deshalb die Reihenfolge unten einhalten.

---

## Welle 0: Messen und Werkzeug (vor jeder anderen Bestellung)

Diese Welle entscheidet, *was* du danach kaufst. Vieles davon hast du vielleicht schon.

| Teil | Suchbegriff / Beispiel | Menge | ca. | Wozu |
|---|---|---|---|---|
| Multimeter | Aneng AN8008, UNI-T UT210E, oder jedes mit DC/AC-Spannung und Durchgangsprüfer | 1 | 20–35 € | Spannung am Türöffner-Taster messen (Schritt 2 im Konzept). Ohne das kein Bauteilkauf. |
| Digitale Kofferwaage | „Kofferwaage digital 50 kg“ | 1 | 8–12 € | Klinkenkraft messen: Haken ans Griffende, runterziehen bis die Falle ganz drin ist, Wert ablesen → bestimmt die Servogröße. |
| Kleiner Schraubendreher-Satz | Elektronik-Bitsatz, Torx + Kreuz + Schlitz | 1 | 8–15 € | Haustelefon öffnen, Klemmblock lösen. |
| Lötkolben, temperaturregelbar | Pinecil V2, Ersa i-Con Nano, TS101 | 1 | 25–60 € | Zwei Litzen an die Taster-Lötpunkte. Billige 30-W-Stäbe ohne Regelung reißen bei dünnen Platinen-Pads leicht die Leiterbahn ab. |
| Lötzinn 0,7 mm | bleifrei mit Flussmittelkern, 100 g | 1 | 8–12 € | |
| Entlötlitze | „Entlötlitze 2 mm“ | 1 | 3–5 € | **Für den Rückbau beim Auszug.** Gleich mitkaufen, sonst steht man später ohne da. |
| Abisolierzange / Seitenschneider | | 1 | 10–15 € | |

**Zwischensumme:** 0 € (alles vorhanden) bis ca. 110 €.

### Entscheidungsbaum nach dem Messen

| Messergebnis | Konsequenz für die Bestellung |
|---|---|
| Spannung am Taster < 60 V, Strom < 0,4 A (Normalfall bei TwinBus) | PhotoMOS **AQY212EH** wie in Welle 2. |
| Spannung oder Strom höher | Statt PhotoMOS ein **Relaismodul mit Optokoppler** (Kontakte ≥ 2 A) nehmen — siehe „Fallbacks“. |
| Klinkenkraft am Griffende ≤ 2,5 kg | Servo mit **35 kg·cm** reicht (DS3235). |
| Klinkenkraft 2,5–4 kg | Direkt **60 kg·cm** nehmen (DS5160). Siehe Hinweis zur Nennspannung unten. |
| Klinke schwergängig / über 4 kg | Erst Schloss fetten/einstellen, dann neu messen. Sonst wird auch ein starker Servo unzuverlässig. |

---

## Welle 1: Steuerung und Trockentest (sofort bestellbar)

Damit baust du den Aufbau auf dem Breadboard mit **LED statt Relais und Servo** auf, flashst die Firmware und testest die Weboberfläche im WLAN. Das geht völlig unabhängig von Haustelefon und Tür.

| Teil | Suchbegriff / Beispiel | Menge | ca. | Hinweis |
|---|---|---|---|---|
| ESP32-Entwicklungsboard | **ESP32-DevKitC V4** (ESP32-WROOM-32, 38-polig) | 2 | je 8–12 € | Zweites als Reserve und zum Weiterbasteln. 38-polige Version nehmen, dann sind GPIO 25/26 aus `config.h` auf der Stiftleiste. Alternativ ESP32-S3-DevKitC (USB-C). |
| USB-Datenkabel | Micro-USB (bei S3: USB-C), mind. 1 m | 1 | 3–5 € | Muss ein **Datenkabel** sein, reine Ladekabel funktionieren nicht. |
| USB-Netzteil 5 V / 1 A | beliebiges Handy-Netzteil | 1 | 0–7 € | Dauerversorgung des ESP32. |
| Breadboard | 830 Kontakte | 1 | 4–6 € | |
| Jumperkabel-Set | m/m und m/f, 20 cm | 1 | 5–8 € | |
| LED 5 mm + 1 kΩ Widerstand | LED-Sortiment oder 10× rot | 1 | 2–4 € | **Platzhalter für PhotoMOS und Servo beim ersten Test** (Schritt 5 im Konzept): LED blinkt, wo später die Tür aufgeht. |
| Lochrasterplatine | 5×7 cm, Streifenraster, 5er-Pack | 1 | 5–8 € | Trägt später PhotoMOS + Widerstand. |
| Stiftleisten 2,54 mm | 40-polig, gerade, 5er-Pack | 1 | 3–5 € | |
| Kunststoffgehäuse | ABS-Gehäuse ca. 100 × 60 × 25 mm | 1 | 5–8 € | ESP32 soll nicht offen neben dem Haustelefon hängen. |

**Zwischensumme:** ca. 40–60 €.

Nach dieser Welle kannst du den kompletten Software-Teil abhaken: Firmware flashen, feste IP im Router, QR-Code, PIN-Login, Protokoll — alles aus Abschnitt 4, 5 und 8 des Konzepts.

---

## Welle 2: Haustür (nach dem Messen am Taster)

| Teil | Suchbegriff / Beispiel | Menge | ca. | Hinweis |
|---|---|---|---|---|
| PhotoMOS-Relais | **Panasonic AQY212EH** (AQY21x-Familie) | 2 | je 3–6 € | **Auf das Gehäuse achten:** Durchsteckvariante für die Lochrasterplatine, nicht die SMD-Ausführung. Beim Händler Datenblatt gegenprüfen (Lastspannung, Laststrom, LED-Strom), bevor du bestellst. Bei Reichelt, Conrad, Mouser oder Digi-Key kaufen — bei Marktplatz-Ware gibt es Fälschungen. |
| Widerstand 220 Ω | 1/4 W, Metallschicht, 10er-Pack | 1 | 1–2 € | Vorwiderstand für die LED-Seite des PhotoMOS (GPIO 26 → 220 Ω → Pin 1). |
| Litze flexibel 0,14 mm² (AWG 26) | rot und schwarz, je 5–10 m | 1 Set | 5–8 € | Dünn und weich, damit die Lötstellen auf der Ritto-Platine nicht mechanisch belastet werden. |
| Steckverbinder 2-polig | JST-XH-Set mit Crimpzange, **oder** einfacher: 2-polige Schraubklemme / Dupont-Steckerpaar | 1 | 3–12 € | Laut Konzept-Entscheidung soll die Verbindung zum ESP32 **steckbar** sein, damit das Telefon beim Rückbau einfach abgeht. |
| Schrumpfschlauch-Sortiment | 1–6 mm | 1 | 4–6 € | |
| Kabelbinder + Klebesockel | klein, 2,5 mm | 1 Set | 3–5 € | Zugentlastung der Litzen am Telefon — die wichtigste Maßnahme gegen abgerissene Lötpunkte. |

**Zwischensumme:** ca. 20–40 €.

---

## Welle 3: Wohnungstür (nach der Kraftmessung)

| Teil | Suchbegriff / Beispiel | Menge | ca. | Hinweis |
|---|---|---|---|---|
| Starker Servo | **DS3235** (35 kg·cm) oder **DS5160** (60 kg·cm), Metallgetriebe, 180°/270° | 1 | 25–45 € | Siehe Nennspannungs-Hinweis unten. Beim Kauf über AliExpress nur im offiziellen DSSERVO-Store, sonst drohen Fälschungen mit Plastikgetriebe. |
| Netzteil für den Servo | Steckernetzteil **6 V oder 7,5 V, ≥ 4 A** (besser 5 A), Hohlstecker 5,5/2,1 mm | 1 | 14–22 € | **Mehr als die 3 A aus dem Konzept:** Der Anlaufstrom eines 35-kg-Servos liegt bei mehreren Ampere, bei 60 kg·cm noch höher. Ein knappes Netzteil bricht beim Drücken in der Spannung ein und der Servo ruckelt oder schafft die Klinke nicht. |
| DC-Einbaubuchse 5,5/2,1 mm | mit Schraubklemme | 2 | je 2 € | |
| Elko 1000 µF / 25 V | low ESR, radial | 2 | je 1 € | Direkt an die Servo-Versorgung. 25 V statt 16 V kostet dasselbe und hält länger. Puffert die Stromspitze, verhindert ESP32-Resets. |
| WAGO-Klemmen 221-412 | 5er-Pack | 1 | 4–6 € | Masse von Servo-Netzteil und ESP32 verbinden — ohne Löten und wieder trennbar. |
| Servo-Verlängerungskabel | 30–50 cm, JR/Futaba | 2 | je 2 € | Der Servo hängt an der Tür, der ESP32 nicht. |
| Alu-Flachprofil | 2 × 20 mm, 50 cm, aus dem Baumarkt | 1 | 5–8 € | Hebelarm vom Servohorn zur Klinke. |
| Metall-Servohorn | 25 T, passend zum Servo | 1 Set | 4–8 € | Die mitgelieferten Plastikhörner geben bei 3–4 Nm nach. |
| Moosgummi oder Filzgleiter | selbstklebend | 1 | 2–4 € | Auf die Druckfläche, damit der Arm die Klinke nicht verkratzt und nicht abrutscht. |
| Halterung | 3D-Druck (Datei selbst zeichnen) **oder** Alu-Winkel + Schraubzwinge + Moosgummi | 1 | 0–20 € | Kein 3D-Drucker? Entweder Druckservice (JLC3DP, PCBWay, lokaler Makerspace, ca. 10–20 € inkl. Versand) oder die Baumarktvariante geklemmt. **Nicht bohren** — Mietwohnung. |

**Zwischensumme:** ca. 60–115 €.

### Hinweis zur Servo-Nennspannung

Die Drehmomentangaben solcher Servos gelten fast immer bei **7,4 V**. Bei 6 V liefern sie typisch 15–20 % weniger. Das Konzept rechnet im Worst Case mit 4 Nm ≈ 40 kg·cm an der Klinkenachse — ein 35-kg·cm-Servo ist dafür **grenzwertig**, besonders bei 6 V. Deshalb:

- Gemessene Klinkenkraft klar niedrig (≤ 2,5 kg am Griffende) → DS3235 mit 7,4-V-Netzteil ist in Ordnung.
- Alles darüber oder Unsicherheit → **DS5160 (60 kg·cm)** nehmen. Die 10–15 € Mehrpreis sind billiger als ein zweiter Bestellvorgang, und Reserve bedeutet hier: der Servo drückt langsam und kraftvoll statt am Anschlag zu würgen.
- Servo-Netzteil nie über den ESP32 versorgen und umgekehrt. Zwei getrennte Netzteile, nur die Masse verbinden (so steht es auch im Konzept).

---

## Welle 4: Kleinkram für den Betrieb

| Teil | Suchbegriff | Menge | ca. | Hinweis |
|---|---|---|---|---|
| Laminierhüllen oder Klarsichthülle | A6/A7 | 1 | 3–6 € | QR-Code am Briefkasten wetterfest machen. Laminieren geht im Copyshop für ein paar Cent, ein Laminiergerät braucht man dafür nicht. |
| Doppelseitiges Montageband | 3M VHB oder Tesa Powerstrips | 1 | 5–8 € | Powerstrips sind rückstandsfrei ablösbar — passt zur Mietwohnung. |

---

## Summen

| Umfang | ca. |
|---|---|
| Nur Welle 1 (Software testen, LED statt Hardware) | **40–60 €** |
| Welle 1 + 2 (Haustür funktioniert) | **60–100 €** |
| Welle 1 + 2 + 3 + 4 (komplett, beide Türen) | **130–190 €** |
| Zusätzlich Werkzeug, falls nichts vorhanden | **+ 60–110 €** |

---

## Fallbacks — erst kaufen, wenn nötig

| Teil | ca. | Wann |
|---|---|---|
| 1-Kanal-Relaismodul 3,3 V mit Optokoppler | 4–7 € | Wenn die Messung am Taster mehr als 60 V oder mehr als 0,4 A ergibt. Dann in `config.h` `RELAIS_AKTIV_HIGH` prüfen — viele Module schalten bei LOW. |
| Zweites Ritto TwinBus Wohntelefon 4630 (gebraucht) | 25–60 € | Nur falls du die Entscheidung vom 12.10. doch umdrehst und das Original komplett unangetastet lassen willst. Dann nur das Zweitgerät umbauen und tauschen (Zuordnung über den gelben Program-Taster). |
| WLAN-Repeater oder Mesh-Knoten | 30–70 € | Wenn Schritt 4 im Konzept zeigt, dass am Briefkasten kein WLAN ankommt. **Das vorher mit dem Handy prüfen** — ohne Empfang an der Haustür ist das ganze Projekt wirkungslos. |
| Smart Lock (Nuki Go / Nuki 4, Bold, Yale Linus) | 150–280 € | Wenn die Klinken-Mechanik unzuverlässig bleibt oder die Wohnungstür auch **abgeschlossen** geöffnet werden soll. Der Servo zieht nur die Falle zurück, nie den Riegel. |

---

## Was du bewusst **nicht** brauchst

- **Keinen Arduino Uno R4 WiFi** oder Uno/Nano mit WLAN-Shield — teurer und schwächer als der ESP32 (Begründung in Abschnitt 1 des Konzepts).
- **Nichts für 230 V**, keinen Klingeltrafo, keine Verteilerkomponenten. Es wird ausschließlich am Haustelefon auf Kleinspannungsseite gearbeitet.
- **Keinen Bus-Adapter** für TwinBus. Wir zapfen den Taster auf der Platine an, der Bus bleibt unberührt.
- **Keinen Raspberry Pi und keinen Server.** Webseite und Logik laufen komplett auf dem ESP32.
- **Kein Zertifikat, keine Domain, kein Cloud-Abo.** Das Ding ist nur im Heim-WLAN erreichbar, und das bleibt so (Abschnitt 5 des Konzepts).
- **Keine Endschalter oder H-Brücke.** Die gehören zum Linearaktuator, der im Konzept gegen den Servo verloren hat.

---

## Bezugsquellen

| Händler | Wofür | Lieferzeit |
|---|---|---|
| Reichelt, Conrad | PhotoMOS, Widerstände, Elkos, Gehäuse, Lochraster, Litze — Originalware mit Datenblatt | 1–3 Tage |
| Mouser, Digi-Key | PhotoMOS, wenn bei Reichelt/Conrad nicht lieferbar | 2–5 Tage |
| Amazon | ESP32-Boards, Breadboard, Jumperkabel, Werkzeug, wenn es schnell gehen soll | 1–2 Tage |
| AliExpress (offizieller DSSERVO-Store) | Servo deutlich günstiger | 2–4 Wochen |
| Baumarkt | Alu-Profil, Montageband, Moosgummi, Schraubzwinge | sofort |

Praktisch: Werkzeug und Welle 1 zusammen bei einem Händler bestellen, den Servo wegen der Lieferzeit **direkt nach der Kraftmessung** separat — der ist sonst der Posten, auf den du am Ende drei Wochen wartest.

---

## Reihenfolge in Kurzform

1. Welle 0 besorgen (oder feststellen, dass alles da ist).
2. Welle 1 bestellen und parallel: Haustelefon aufschrauben, Taster durchmessen, Klinkenkraft wiegen, WLAN an der Haustür prüfen.
3. Firmware flashen und mit LED testen, bis die Weboberfläche im Handy sauber läuft.
4. Welle 2 bestellen, PhotoMOS auf Lochraster aufbauen, erst gegen das Multimeter testen (Durchgang beim Schalten), dann ans Telefon löten.
5. Welle 3 bestellen, sobald die Kraft gemessen ist. Servo frei in der Hand einstellen (`SERVO_RUHE_US` / `SERVO_GEDRUECKT_US`), erst dann an die Tür.
6. Welle 4 und QR-Code an den Briefkasten.

# Einkaufsliste Türöffner

Konkrete Teile zum Bestellen, aufgeteilt in Bestellwellen. Grundlage ist [KONZEPT.md](KONZEPT.md): ESP32 mit Relaismodul am Ritto-TwinBus-Wohntelefon 4630/01 (Haustür) und starkem Servo an der Klinke (Wohnungstür).

Preise sind **Richtwerte** (Stand Oktober 2026, Brutto, ohne Versand) und schwanken je Händler stark. Mengenangaben sind so gewählt, dass immer ein Ersatzteil da ist — beim Löten an einer fremden Anlage willst du nicht wegen eines gegrillten Bauteils zwei Wochen warten.

> **Wichtig: nicht alles auf einmal bestellen.** Der Servo ist der teuerste Einzelposten und die richtige Größe steht erst nach dem Messen fest. Welle 0 bis 2 kannst du dagegen sofort bestellen.

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
| Spannung am Taster im Kleinspannungsbereich (Normalfall bei TwinBus) | Keine Bauteilentscheidung nötig — das Relaismodul aus Welle 2 deckt mit 10 A / 250 V AC jeden realistischen Fall ab. Die Messung bestätigt nur, dass du richtig liegst, und hilft beim Finden des Tasters. |
| Unerwartet hohe Spannung (über ~60 V) oder Netzspannung | **Stopp, nicht weiterbauen.** Dann hast du nicht den Taster erwischt, sondern etwas anderes. Erst klären, dann anklemmen. |
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
| LED 5 mm + 1 kΩ Widerstand | LED-Sortiment oder 10× rot | 1 | 2–4 € | **Platzhalter für Relais und Servo beim ersten Test** (Schritt 5 im Konzept): LED blinkt, wo später die Tür aufgeht. |
| Lochrasterplatine | 5×7 cm, Streifenraster, 5er-Pack | optional | 5–8 € | War für den PhotoMOS gedacht. Das Relaismodul ist eine fertige Platine — **brauchst du für diesen Aufbau nicht mehr.** |
| Stiftleisten 2,54 mm | 40-polig, gerade, 5er-Pack | optional | 3–5 € | Siehe Lochrasterplatine — fürs Relaismodul nicht nötig. |
| Kunststoffgehäuse | ABS-Gehäuse ca. 100 × 60 × 25 mm | 1 | 5–8 € | ESP32 soll nicht offen neben dem Haustelefon hängen. |

**Zwischensumme:** ca. 40–60 €.

Nach dieser Welle kannst du den kompletten Software-Teil abhaken: Firmware flashen, feste IP im Router, QR-Code, PIN-Login, Protokoll — alles aus Abschnitt 4, 5 und 8 des Konzepts.

### Welle 1 als Amazon.de-Bestellliste

Direkt bestellbar, ohne auf Messwerte zu warten. Die **ASIN** ist jeweils der dauerhafte Teil — falls ein Link irgendwann ins Leere läuft, ASIN in die Amazon-Suche kopieren.

> **Preise und Verfügbarkeit beim Klick prüfen.** Diese Links stammen aus dem Suchindex; die Amazon-Produktseiten selbst waren aus der Arbeitsumgebung heraus nicht aufrufbar (Netzwerk-Policy), konnten also nicht live gegengeprüft werden. Bei Marktplatz-Artikeln wechseln Preis, Anbieter und Lieferbarkeit ständig — bei mehreren Breadboard-Sets zeigte der Index schon „derzeit nicht verfügbar".

| Pos | Artikel | Menge | Hinweis |
|---|---|---|---|
| 1 | [2× ESP32DevKitC, ESP-WROOM-32D](https://www.amazon.de/dp/B0D4QBCCCZ) — ASIN `B0D4QBCCCZ` | 1 Set | Genau die zwei Boards, die du brauchst. Alternativen: [AITRIP 2× ESP32-DevKitC-32, USB-C/CH340C](https://www.amazon.de/dp/B0BRB64NCH) (`B0BRB64NCH`) oder [AZDelivery 5× DevKit C V4, USB-C](https://www.amazon.de/dp/B0DJ2XYKK2) (`B0DJ2XYKK2`) — deutscher Anbieter, aber 5 Stück. |
| 2 | [BOJACK Breadboard-Set: 2× 830 + 2× 400 Kontakte + 126 Jumperkabel](https://www.amazon.de/dp/B0B18G3V5T) — ASIN `B0B18G3V5T` | 1 Set | **Deckt zwei Posten auf einmal** (Breadboard und Jumperkabel). Alternative: [GTIWUNG 4× 830 + Kabel](https://www.amazon.de/dp/B0CFXYP5SF) (`B0CFXYP5SF`). |
| 3 | [WayinTop 200× LED 5 mm + 600 Widerstände, 30 Werte 10 Ω–1 MΩ](https://www.amazon.de/dp/B07YWX42RJ) — ASIN `B07YWX42RJ` | 1 Set | Test-LED plus 1 kΩ Vorwiderstand für den Trockenlauf. Dass laut Bewertungen die LED-Farben streuen, ist dafür egal. |
| 4 | [Streifenrasterplatine 100 × 160 mm, RM 2,54, 5 Stück](https://www.amazon.de/dp/B008N7RZ3M) — ASIN `B008N7RZ3M` | **optional** | Streifenraster wie im Konzept — Leiterbahnen lassen sich mit dem Messer durchtrennen. Alternative ohne Streifen: [Hartpapier 60 × 100 mm](https://www.amazon.de/dp/B0CGLN1CF7) (`B0CGLN1CF7`). |
| 5 | [IZOKEE 30× Stift- und Buchsenleisten, 40-polig, RM 2,54](https://www.amazon.de/dp/B07DBY753C) — ASIN `B07DBY753C` | **optional** | Wie Pos 4: war für den PhotoMOS-Aufbau. Das Relaismodul bringt Schraubklemmen und Stiftleiste mit. |
| 6 | [Projektgehäuse ABS 100 × 60 × 25 mm](https://www.amazon.de/dp/B0DWRPN2S6) — ASIN `B0DWRPN2S6` | 1 | Innenmaß ca. 96 × 56 × 23 mm, das ESP32-DevKitC ist ca. 55 × 28 mm — passt mit Platz für die Lochrasterplatine. Alternative als 5er-Pack: [VGEBY](https://www.amazon.de/dp/B074Z7TZYP) (`B074Z7TZYP`). |
| 7 | USB-Kabel + 5-V-Netzteil | 1 | **Bewusst ohne Link**, weil es vom gewählten Board abhängt: USB-C bei AITRIP und AZDelivery, Micro-USB beim DevKitC-Klassiker. Ein normales Handy-Netzteil und ein vorhandenes Datenkabel reichen — reine Ladekabel ohne Datenleitungen funktionieren nicht. |

### Was damit wegfällt

- Pos 2 ersetzt die getrennten Zeilen „Breadboard" und „Jumperkabel-Set" aus der Tabelle oben.
- **Pos 4 und 5 brauchst du nicht**, seit die Haustür über ein fertiges Relaismodul statt über einen selbst aufgebauten PhotoMOS läuft. Nur mitnehmen, wenn du ohnehin Lochraster im Haus haben willst.

### Worauf du beim Board achten musst

- **38-polige Variante.** Die Firmware nutzt GPIO 25 und 26 (`config.example.h`); auf den kleineren 30-Pin-Boards fehlen die teils.
- **USB-Chip:** CH340 oder CP2102. Beide funktionieren, brauchen unter Windows aber unterschiedliche Treiber. Unter Linux und macOS sind beide meist schon dabei.
- **No-Name ist hier völlig in Ordnung** und deutlich günstiger. Einzige Konsequenz: die Ausfallquote ist höher — bei den billigsten Angeboten berichten Bewertungen von verbogenen Pins und Boards, die beim ersten Einschalten sterben. Deshalb zwei Stück, dann ist der Ersatz schon da statt in zwei Wochen.

---

## Welle 2: Haustür (sofort bestellbar)

| Teil | Suchbegriff / Beispiel | Menge | ca. | Hinweis |
|---|---|---|---|---|
| 1-Kanal-Relaismodul 3,3 V | mit Optokoppler, **Low-Level-Trigger** | 2 | je 4–7 € | Das gewählte Bauteil (Entscheidung vom 12.10.2026, Begründung in Abschnitt 2 des Konzepts). Fertige Platine mit Schraubklemmen — kein Löten, kein Vorwiderstand, keine Lochrasterplatine. Zwei Stück: eins zum Testen, eins fürs Telefon. |
| Litze flexibel 0,14 mm² (AWG 26) | rot und schwarz, je 5–10 m | 1 Set | 5–8 € | Dünn und weich, damit die Lötstellen auf der Ritto-Platine nicht mechanisch belastet werden. |
| Steckverbinder 2-polig | JST-XH-Set mit Crimpzange, **oder** einfacher: 2-polige Schraubklemme / Dupont-Steckerpaar | 1 | 3–12 € | Laut Konzept-Entscheidung soll die Verbindung zum ESP32 **steckbar** sein, damit das Telefon beim Rückbau einfach abgeht. |
| Schrumpfschlauch-Sortiment | 1–6 mm | 1 | 4–6 € | |
| Kabelbinder + Klebesockel | klein, 2,5 mm | 1 Set | 3–5 € | Zugentlastung der Litzen am Telefon — die wichtigste Maßnahme gegen abgerissene Lötpunkte. |

**Zwischensumme:** ca. 15–30 €.

### Welle 2 als Amazon.de-Bestellliste

| Pos | Artikel | Menge | Hinweis |
|---|---|---|---|
| 1 | [Relaismodul 3,3 V, 1 Kanal, Low-Level-Trigger, mit Optokoppler](https://www.amazon.de/dp/B0CL748J9C) — ASIN `B0CL748J9C` | 1–2 | **Das Hauptbauteil für die Haustür.** Alternativen: [`B0DBZQVWPM`](https://www.amazon.de/dp/B0DBZQVWPM) oder [`B0DJ6PD435`](https://www.amazon.de/dp/B0DJ6PD435) — letzteres läuft auf **3,3 V und 5 V** und ist damit die sichere Wahl, siehe unten. |
| 2 | [Schrumpfschlauch-Set, 100 Stück, 6 Größen, 2:1](https://www.amazon.de/dp/B003H9CJ1Y) — ASIN `B003H9CJ1Y` | 1 | Billigste brauchbare Variante. Alternative **mit Kabelbindern drin**: [`B004CRC352`](https://www.amazon.de/dp/B004CRC352) — spart die eigene Kabelbinder-Zeile. |
| 3 | [Zwillingslitze 0,14 mm², 10 m, 2-adrig](https://www.amazon.de/dp/B002QE5OKY) — ASIN `B002QE5OKY` | 1 | 2-adrig heißt: ein Kabel zum Telefon statt zwei einzelne. 10 m ist für zwei 30-cm-Strippen reichlich, aber die kleinste sinnvolle Menge. Einzelspule 25 m: [`B0774W79T7`](https://www.amazon.de/dp/B0774W79T7). |
| 4 | Steckverbinder 2-polig | 0 | **Entfällt.** Nimm zwei Jumperkabel aus Welle 1 Pos 2, schneide eine Seite ab und löte sie an — das ist genau der steckbare Übergang, den das Konzept für den Rückbau will. |
| 5 | Widerstand 220 Ω, Lochraster, Stiftleisten | 0 | **Entfallen alle** — die brauchte nur der PhotoMOS. |

#### Beim Relaismodul beachten

- **VCC des Moduls an den 3V3- oder 5V-Pin des ESP32, nicht an einen GPIO.** Eine Bewertung misst rund 100 mA Stromaufnahme; ein ESP32-GPIO darf etwa 40 mA. An den GPIO kommt nur `IN`.
- **`RELAIS_AKTIV_HIGH` steht in `config.example.h` jetzt auf `false`**, passend zum Low-Level-Trigger: das Modul schaltet bei 0 V am Eingang. Nimmst du doch ein High-Level-Modul, muss der Wert auf `true`.
- **Wenn das Relais bei 3,3 V nicht sicher anzieht**, berichten Bewertungen zu solchen Modulen — dann die 3,3-/5-V-Variante nehmen und `VCC` vom 5V-Pin versorgen. Der Trigger bleibt davon unberührt, der läuft über den Optokoppler.

---

## Welle 3: Wohnungstür (nach der Kraftmessung)

| Teil | Suchbegriff / Beispiel | Menge | ca. | Hinweis |
|---|---|---|---|---|
| Starker Servo | **DS3235** (35 kg·cm) oder **DS5160** (60 kg·cm), Metallgetriebe, 180°/270° | 1 | 25–45 € | Siehe Nennspannungs-Hinweis unten. No-Name-Klone tun es hier problemlos — im schlechtesten Fall liefern sie etwas weniger Drehmoment als angegeben, was die Reserve-Empfehlung unten ohnehin abdeckt. |
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

### Welle 3 als Amazon.de-Bestellliste

| Pos | Artikel | Menge | Hinweis |
|---|---|---|---|
| 1 | [DS3235 Servo, 35 kg·cm, 2er-Pack, **180°**](https://www.amazon.de/dp/B0CX97XF9J) — ASIN `B0CX97XF9J` | 1 Set | Laut Listung 23–35 kg·cm bei 5–8,4 V, IP66, Edelstahlgetriebe. **180° nehmen, nicht 270°** — Begründung unten. 2er-Pack, weil der erste Einstellversuch an der Klinke erfahrungsgemäß nicht der letzte ist. |
| 1a | [DS3235 mit 25T-Metallarm im Lieferumfang](https://www.amazon.de/dp/B0DGQBD8K5) — ASIN `B0DGQBD8K5` | Alt. | Spart Pos 6. Achtung: dort 180°/360°-Varianten, die **270er-Option meiden**. |
| 1b | [DS5160, 60 kg·cm, 270°, IP67](https://www.amazon.de/dp/B09VS3SHVD) — ASIN `B09VS3SHVD` | Alt. | 6–8,4 V. Nimm den, wenn die Kofferwaage über 2,5 kg zeigt. Weitere 35-kg-Optionen: [`B0CBX6N5RX`](https://www.amazon.de/dp/B0CBX6N5RX), [`B07YWD857N`](https://www.amazon.de/dp/B07YWD857N), [`B08THYMY3Y`](https://www.amazon.de/dp/B08THYMY3Y). |
| 2 | [Steckernetzteil 6 V, Stromstärke wählbar 1–5 A, Hohlstecker 5,5/2,1](https://www.amazon.de/dp/B08YJSWDWK) — ASIN `B08YJSWDWK` | 1 | **Beim Bestellen die 4-A- oder 5-A-Variante auswählen.** Siehe Spannungs-Hinweis unten. |
| 3 | [DC-Einbaubuchse 5,5 × 2,1 mm mit Schraubklemme, 5er](https://www.amazon.de/dp/B07GYM4BKH) — ASIN `B07GYM4BKH` | 1 Set | Bringt den Hohlstecker des Netzteils lötfrei auf Litze. Alternativen: [`B00N7I1L6E`](https://www.amazon.de/dp/B00N7I1L6E), [`B09PD6J4BN`](https://www.amazon.de/dp/B09PD6J4BN). |
| 4 | [Elko 1000 µF / 25 V, radial, 105 °C, 5er](https://www.amazon.de/dp/B0CF6DG7XV) — ASIN `B0CF6DG7XV` | 1 Set | Direkt an die Servo-Versorgung. 20er-Pack mit Rubycon: [`B07C31V5YZ`](https://www.amazon.de/dp/B07C31V5YZ). |
| 5 | [WAGO 221-412, 50 Stück](https://www.amazon.de/dp/B08768VL3Z) — ASIN `B08768VL3Z` | 1 Set | Massen von Servo-Netzteil und ESP32 verbinden. Klemmt feindrähtig ab 0,14 mm² — passt also zur Litze aus Welle 2. |
| 6 | Servohorn 25T Metall | 0–1 | Liegt bei den meisten DS3235-Angeboten bei (eine Bewertung bestätigt 25 Zähne). Erst nach Erhalt nachkaufen, oder gleich Pos 1a nehmen. |
| 7 | Alu-Flachprofil 2 × 20 mm, Moosgummi, Montageband | – | **Baumarkt.** Billiger als jeder Amazon-Versand und du kannst das Profil direkt vor Ort ablängen. |

#### Warum 180° und nicht 270°

Beide Varianten kosten gleich viel, aber die Firmware fährt den Servo mit Pulsbreiten von 1000–1900 µs (`SERVO_RUHE_US` / `SERVO_GEDRUECKT_US`). Bei einem 270°-Servo deckt derselbe Pulsbereich etwa die 1,5-fache Winkelstrecke ab: jede Mikrosekunde bewegt den Arm weiter, die Einstellung wird gröber und Überschwingen drückt die Klinke gegen den Anschlag. Für die rund 45°, die wir brauchen, ist die 180°-Variante feiner auflösbar. Wenn nur 270° lieferbar ist, geht das auch — dann die beiden Pulswerte entsprechend näher zusammenlegen.

#### Netzteil: 6 V statt 7,5 V

Ein 7,5-V-Netzteil mit ≥ 4 A war nicht zu finden; die 7,5-V-Angebote liegen bei 800 mA, das ist viel zu wenig. Die Serie aus Pos 2 gibt es nur in 5 V, 6 V und 12 V. Also **6 V mit 4–5 A**:

- 12 V wäre zu viel — der DS3235 ist für 5–8,4 V angegeben, 12 V zerstört ihn.
- 6 V kostet gegenüber 7,4 V rund 15–20 % Drehmoment, die Listung nennt 23–35 kg·cm über den Spannungsbereich. Das ist der Grund, bei unklarer Kraftmessung direkt den DS5160 zu nehmen.
- Polarität vor dem Anstecken prüfen: bei 5,5/2,1-Steckern ist innen meist plus, aber nicht immer.
- Wer 7,4 V will: 2S-LiPo oder ein Labornetzteil. Für eine dauerhaft verbaute Lösung an der Wohnungstür ist das 6-V-Steckernetzteil die unaufwendigere Variante.

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
| Welle 1 + 2 (Haustür funktioniert) | **55–90 €** |
| Welle 1 + 2 + 3 + 4 (komplett, beide Türen) | **125–180 €** |
| Zusätzlich Werkzeug, falls nichts vorhanden | **+ 60–110 €** |

---

## Fallbacks — erst kaufen, wenn nötig

| Teil | ca. | Wann |
|---|---|---|
| Zweites Ritto TwinBus Wohntelefon 4630 (gebraucht) | 25–60 € | Nur falls du die Entscheidung vom 12.10. doch umdrehst und das Original komplett unangetastet lassen willst. Dann nur das Zweitgerät umbauen und tauschen (Zuordnung über den gelben Program-Taster). |
| WLAN-Repeater oder Mesh-Knoten | 30–70 € | Wenn Schritt 4 im Konzept zeigt, dass am Briefkasten kein WLAN ankommt. **Das vorher mit dem Handy prüfen** — ohne Empfang an der Haustür ist das ganze Projekt wirkungslos. |
| Smart Lock (Nuki Go / Nuki 4, Bold, Yale Linus) | 150–280 € | Wenn die Klinken-Mechanik unzuverlässig bleibt oder die Wohnungstür auch **abgeschlossen** geöffnet werden soll. Der Servo zieht nur die Falle zurück, nie den Riegel. |

---

## Was du bewusst **nicht** brauchst

- **Keinen Arduino Uno R4 WiFi** oder Uno/Nano mit WLAN-Shield — teurer und schwächer als der ESP32 (Begründung in Abschnitt 1 des Konzepts).
- **Nichts für 230 V**, keinen Klingeltrafo, keine Verteilerkomponenten. Es wird ausschließlich am Haustelefon auf Kleinspannungsseite gearbeitet.
- **Keinen Bus-Adapter** für TwinBus. Wir zapfen den Taster auf der Platine an, der Bus bleibt unberührt.
- **Keinen PhotoMOS und keinen Vorwiderstand.** War bis zum 12.10.2026 der Plan, ersetzt durch das Relaismodul — fertige Platine, lötfrei, Trennung durch Luftspalt statt durch Datenblattangabe.
- **Keinen Raspberry Pi und keinen Server.** Webseite und Logik laufen komplett auf dem ESP32.
- **Kein Zertifikat, keine Domain, kein Cloud-Abo.** Das Ding ist nur im Heim-WLAN erreichbar, und das bleibt so (Abschnitt 5 des Konzepts).
- **Keine Endschalter oder H-Brücke.** Die gehören zum Linearaktuator, der im Konzept gegen den Servo verloren hat.

---

## Bezugsquellen

| Händler | Wofür | Lieferzeit |
|---|---|---|
| Reichelt, Conrad | Nur noch, wenn du etwas Bestimmtes mit Datenblatt brauchst. Für diese Liste nicht nötig. | 1–3 Tage |
| Amazon | ESP32-Boards, Breadboard, Jumperkabel, Werkzeug, wenn es schnell gehen soll | 1–2 Tage |
| AliExpress | Servo und Kleinteile am günstigsten, No-Name ist hier durchgehend unkritisch | 2–4 Wochen |
| Baumarkt | Alu-Profil, Montageband, Moosgummi, Schraubzwinge | sofort |

Praktisch: Werkzeug und Welle 1 zusammen bei einem Händler bestellen, den Servo wegen der Lieferzeit **direkt nach der Kraftmessung** separat — der ist sonst der Posten, auf den du am Ende drei Wochen wartest.

---

## Reihenfolge in Kurzform

1. Welle 0 besorgen (oder feststellen, dass alles da ist).
2. Welle 1 bestellen und parallel: Haustelefon aufschrauben, Taster durchmessen, Klinkenkraft wiegen, WLAN an der Haustür prüfen.
3. Firmware flashen und mit LED testen, bis die Weboberfläche im Handy sauber läuft.
4. Relaismodul am Breadboard testen: GPIO 26 an `IN`, `VCC` an 3V3/5V, mit dem Multimeter den Durchgang am Schließer prüfen. Erst dann die Litzen ans Telefon löten.
5. Welle 3 bestellen, sobald die Kraft gemessen ist. Servo frei in der Hand einstellen (`SERVO_RUHE_US` / `SERVO_GEDRUECKT_US`), erst dann an die Tür.
6. Welle 4 und QR-Code an den Briefkasten.

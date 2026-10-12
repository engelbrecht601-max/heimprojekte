# Türöffner per WLAN – Konzept

Ziel: Wer im Heim-WLAN ist, scannt einen QR-Code am Briefkasten, öffnet eine Webseite auf dem Handy und kann damit
1. die **Haustür** öffnen (Relais parallel zur Türöffner-Taste der Gegensprechanlage) und
2. die **Wohnungstür** öffnen (Motor drückt die Klinke von innen runter).

Alles läuft lokal auf einem Mikrocontroller, ohne Cloud und ohne App-Installation.

---

## 1. Hardware-Auswahl

| Option | Bewertung |
|---|---|
| **ESP32 DevKit (z. B. ESP32-DevKitC, ESP32-S3-DevKitC)** | **Empfehlung.** ~8–15 €, WLAN onboard, viel RAM, ausgereifte Webserver-Bibliotheken, mDNS, OTA-Updates. Programmierbar mit der Arduino IDE. |
| Arduino Uno R4 WiFi | Geht auch, ist aber teurer (~28 €). Das WLAN läuft über einen ESP32-S3-Coprozessor, die Webserver-Bibliothek ist deutlich einfacher und weniger robust. Nur sinnvoll, wenn er schon rumliegt. |
| Arduino Uno/Nano ohne WLAN | Ungeeignet. |

Der Code-Entwurf in `firmware/` ist für den **ESP32 mit Arduino-Core 3.x** geschrieben.

### Einkaufsliste

Konkrete Teile, Mengen, Richtpreise und Bezugsquellen stehen in **[EINKAUFSLISTE.md](EINKAUFSLISTE.md)**, aufgeteilt in Bestellwellen.

Grobes Budget: ca. 40–60 € für den reinen Softwaretest (ESP32 + LED als Platzhalter), ca. 130–190 € für den kompletten Aufbau mit beiden Türen, zuzüglich Werkzeug.

Wichtig: Den **Servo** erst bestellen, **nachdem** die Klinkenkraft gemessen ist (Schritt 3 unter „Nächste Schritte“) — die Größe hängt direkt am Messergebnis. Alles andere kann sofort raus.

---

## 2. Haustür: Relais an die Gegensprechanlage

### Prinzip
In deiner Wohnung hängt ein Haustelefon mit einer **Türöffner-Taste** (Schlüssel-Symbol). Die Taste schließt meist einfach zwei Kontakte kurz. Das Relais wird **parallel zu dieser Taste** angeschlossen und für 2–3 Sekunden geschlossen. Die Anlage merkt keinen Unterschied zum Tastendruck.

### Welche Anlage hast du? (das entscheidet alles)
Hersteller und Typ stehen meist hinten auf dem Haustelefon (Gehäuse abnehmen, Typenschild).

| Anlagentyp | Typische Hersteller/Systeme | Machbarkeit |
|---|---|---|
| **Analog 1+n, 4+n, 6+n** (mehrere Adern, Klemmen wie `ET`, `E`, `Ö`, `7`, `b`) | ältere Siedle 1+n/6+n, Ritto analog, STR, Grothe, Elcom 4+n | **Einfach.** Taste parallel brücken. |
| **2-Draht-Bus** (nur 2 Adern, Klemmen `a`/`b` oder `BUS`) | TCS:Bus, Siedle In-Home-Bus, Ritto TwinBus, Gira 2-Draht, Comelit SimpleBus, Elcom/Bticino 2-Draht | **Nicht per Taste-Brücken**, da die Taste ein Bus-Telegramm auslöst. Lösungen: Taste im Hörer selbst brücken (wenn sie ein einfacher Taster auf der Platine ist, oft machbar), oder Bus-Adapter (für TCS gibt es fertige ESP32-Projekte wie „Doorman“, für Siedle offizielle Schnittstellen). |
| **IP-/Video-Anlage** | Gira, Doorbird, Siedle Access | Meist eigene App/API – dann besser die nutzen. |

### Vor dem Anschließen messen
Mit dem Multimeter an den beiden Kontakten der Türöffner-Taste:
1. **Spannung offen** (Taste nicht gedrückt), DC und AC messen.
2. **Strom bei gedrückter Taste** (Multimeter in Reihe), falls möglich.

**Entscheidung (12.10.2026): 1-Kanal-Relaismodul mit Optokoppler, Low-Level-Trigger.** Der Schließer (NO) kommt parallel zur Taste, `IN` an GPIO 26, `VCC` an den 3V3- oder 5V-Pin des ESP32 — **nicht an einen GPIO**: das Modul zieht rund 100 mA, ein ESP32-GPIO darf etwa 40 mA.

Damit entscheidet die Messung **nicht mehr über das Bauteil**. Die üblichen Modulkontakte (10 A / 250 V AC) liegen weit über allem, was eine Türöffner-Taste führt, egal ob 1+n oder Bus. Die Messung bestätigt nur noch, dass du auf Kleinspannung bist, und hilft beim Finden des richtigen Tasters.

Was du dafür in Kauf nimmst: Ein mechanisches Relais **klickt** hörbar und prellt beim Schalten. Für einen 3-Sekunden-Puls am Türöffner ist beides harmlos. Dafür ist die galvanische Trennung hier ein echter Luftspalt und hängt nicht an einer Datenblattangabe — bei No-Name-Ware das deutlich stärkere Argument als bei einem Halbleiter, dessen Isolationsfestigkeit man nicht nachmessen kann.

### Potentialtrennung, warum das wichtig ist
- Die Gegensprechanlage hat ihr eigenes Netzteil und ihre eigene Masse. Der ESP32 darf **keine elektrische Verbindung** dazu haben, sonst riskierst du Fehlströme, Brummen auf der Sprechleitung oder einen kaputten ESP bzw. eine kaputte Anlage.
- Das Relaismodul trennt galvanisch: auf der ESP-Seite nur Optokoppler und Relaisspule, auf der Anlagen-Seite nur der Schaltkontakt. Dazwischen liegt im Relais ein Luftspalt.
- **Niemals** direkt an 230 V oder am Klingeltrafo im Verteiler arbeiten. Wir bleiben ausschließlich am Haustelefon in der Wohnung, auf Kleinspannungsseite.

### Bei uns verbaut: Ritto TwinBus Wohntelefon 4630/01

Stand 12.10.2026, laut Foto der Platine: **Ritto TwinBus**, Wohntelefon 4630/01 (Version 2.10), 4 Adern an Klemme KL1 (`a`, `b` = Bus, dazu zwei weitere Adern). Damit gilt:

- **Nicht** an den Klemmen `a`/`b` brücken. Das schließt den Bus kurz und kann die Anlage im ganzen Haus stören.
- Stattdessen wird der **Schlüssel-Taster auf der Platine** angezapft. Die beiden weißen Tasten rechts auf der Platine sind nur Stößel, darunter sitzen die eigentlichen Taster. Zwei dünne Litzen an die Lötpunkte des Türöffner-Tasters, daran der Schließer (NO) des Relaismoduls parallel. Das Telefon „drückt“ dann selbst und sendet das passende Bus-Telegramm.
- Den richtigen Taster und seine Pins vorher bestimmen: Telefon eingebaut lassen, mit dem Multimeter die Spannung über die Taster-Pins messen, Taste drücken. Die Spannung fällt beim Drücken auf etwa 0 V. Auf die Polarität musst du dabei nicht achten: der Relaiskontakt ist ein potentialfreier Schließer und schaltet in beide Richtungen, AC wie DC.
- **Mietfreundliche Variante:** Ein zweites, gebrauchtes Ritto TwinBus Wohntelefon 4630 besorgen (gebraucht meist günstig), nur dieses umbauen und gegen das Original tauschen. Das Original bleibt unverändert und kommt beim Auszug wieder rein. Das Ersatzgerät muss der Wohnung zugeordnet werden, dafür ist der gelbe **Program**-Taster da (Ablauf laut Ritto-Anleitung zum 4630, vor dem Tausch nachlesen).
- **Entscheidung (12.10.2026):** Umgebaut wird das vorhandene Telefon, beim Auszug wird zurückgebaut. Dafür: vorher Platine und Verkabelung fotografieren, Klemmblock KL1 zum Löten abziehen (Telefon dann vom Bus getrennt), Litzen zugentlasten und über einen 2-poligen Stecker zum ESP32 führen. Rückbau = Litzen ablöten, Lötstellen säubern.
- Bei TwinBus darf der Türöffner in der Regel auch ohne vorheriges Klingeln ausgelöst werden. Das trotzdem einmal von Hand testen.

### Häufige Stolperfalle
Manche Anlagen lassen den Türöffner nur **innerhalb eines Zeitfensters nach dem Klingeln** zu (z. B. 30 s). Teste daher zuerst: Geht der Türöffner am Haustelefon, ohne dass vorher jemand geklingelt hat? Wenn nicht, müsste der Ablauf „erst klingeln, dann in der App öffnen“ sein, was für den QR-Code-Ablauf sogar ganz natürlich ist.

---

## 3. Wohnungstür: Servo oder Linearaktuator?

### Kraftabschätzung
- Eine Türklinke mit Falle braucht typischerweise **20–40 N** am Griffende, um ganz runterzugehen (Rückstellfeder + Fallenfeder; schwergängige Schlösser auch mehr).
- Hebelarm von der Drehachse zum Druckpunkt: ca. **10 cm**.
- Drehmoment an der Klinkenachse: 40 N × 0,1 m = **4 Nm ≈ 40 kg·cm** (Worst Case). Typisch eher 2–3 Nm.
- Weg: ca. 40–45° Drehung, am Griffende ca. 6–8 cm Hub.

### Vergleich

| | Standard-Servo (MG996R, ~10 kg·cm) | **Starker Servo (35–60 kg·cm)** | Linearaktuator 12 V |
|---|---|---|---|
| Kraft reicht? | Nein, nur ~1 Nm | **Ja**, 3,4–6 Nm | Ja, 100–200 N verfügbar |
| Geschwindigkeit | schnell | **schnell (< 0,3 s)** | langsam (5–20 mm/s → 3–10 s für 60 mm) |
| Ansteuerung | PWM direkt | **PWM direkt** | H-Brücke oder 2 Relais, Endschalter |
| Bauform an der Tür | klein | **kompakt** | lang (≥ 15 cm), schwer zu montieren |
| Manuelle Bedienung | – | **möglich, wenn nur „aufliegend“ montiert** | sperrt oft, da selbsthemmend |

**Empfehlung: starker Servo (≥ 35 kg·cm, metallgetriebe)**, montiert *über* der Klinke mit einem Arm, der die Klinke **nur von oben herunterdrückt** und nicht fest mit ihr verbunden ist. So kann man die Klinke weiterhin normal von Hand bedienen, und im Ruhezustand liegt der Arm frei. Servo nach dem Drücken wieder in Ruhestellung fahren und PWM abschalten (macht der Code), damit er nicht dauerhaft brummt und Strom zieht.

Montage ohne Bohren: Klemmhalterung an Türblatt oder Langschild, z. B. 3D-gedruckt. Separate Servo-Versorgung (6–7,4 V, ≥ 3 A Spitze), Massen von Servo-Netzteil und ESP verbinden, Elko direkt am Servo.

### Wichtige Grenzen
- Klinke drücken zieht nur die **Falle** zurück. Ist die Tür **abgeschlossen** (Riegel per Schlüssel), passiert nichts. Das Konzept funktioniert also nur für eine „ins Schloss gezogene“ Tür.
- Die Person muss die Tür gleichzeitig **aufdrücken**. Der Code hält die Klinke daher ein paar Sekunden unten.
- Die Wohnungstür muss jederzeit von innen ohne Hilfsmittel zu öffnen sein (Fluchtweg). Die Mechanik darf die Klinke nie blockieren, auch bei Stromausfall nicht.

### Fertige Alternative zum Vergleich
Ein Smart Lock wie **Nuki** (oder Bold, Yale Linus) montiert innen auf dem Schließzylinder, schließt auf **und** zieht die Falle zurück, hat eine lokale API und lässt sich ohne Bohren mietfreundlich anbringen. Kann man später auch vom ESP32 aus ansteuern, falls die Klinken-Mechanik nicht zuverlässig wird.

---

## 4. Weboberfläche im WLAN mit QR-Code

- Der ESP32 bekommt im Router eine **feste IP** (DHCP-Reservierung, z. B. in der Fritzbox unter Heimnetz → Netzwerk → „Diesem Gerät immer die gleiche IPv4-Adresse zuweisen“).
- Zusätzlich ist er per mDNS als `http://tuer.local` erreichbar. Android-Handys können `.local` nicht immer auflösen, deshalb in den **QR-Code die IP-Adresse** schreiben, z. B. `http://192.168.178.50/`.
- QR-Code mit beliebigem Generator erzeugen (oder `qrencode -o qr.png http://192.168.178.50/`), laminiert an den Briefkasten.
- Die Seite ist eine einfache Handy-Seite mit zwei großen Knöpfen: **Haustür öffnen** und **Wohnungstür öffnen**.

### Reichweite prüfen
Die Person steht **vor der Haustür** bzw. am Briefkasten. Dort muss euer WLAN noch ankommen, und das Handy muss sich automatisch verbunden haben. Bei einem Mehrfamilienhaus mit Wohnung im 3. OG ist das oft nicht der Fall. Vorher mit dem Handy testen; ggf. Repeater/Mesh-Knoten Richtung Treppenhaus.

---

## 5. Zugriffsschutz

„Nur im Heimnetz erreichbar“ ist die Basis, reicht aber allein nicht. Jedes Gerät im WLAN (Smart-TV, Gäste-Handy von vor zwei Jahren, kompromittiertes IoT-Gerät, Nachbar mit altem WLAN-Passwort) könnte sonst die Tür öffnen.

Im Code-Entwurf umgesetzt:

| Maßnahme | Wie |
|---|---|
| **Persönliche PIN pro Person** | In `config.h` hinterlegt. Einmal eingeben, dann merkt sich das Handy ein Sitzungs-Token (Cookie). |
| **Sperre bei Fehlversuchen** | Nach 5 falschen PINs ist der Login 5 min gesperrt, jede weitere Sperre verdoppelt die Zeit (max. 1 h). |
| **Cookie-Schutz** | `HttpOnly` und `SameSite=Strict`: eine fremde Webseite im Browser kann die Tür nicht „im Hintergrund“ öffnen (CSRF). |
| **Nur POST zum Öffnen** | Kein Öffnen durch bloßes Aufrufen eines Links oder Link-Vorschauen in Messengern. |
| **Protokoll** | Die letzten 20 Öffnungen mit Name und Zeit sind auf der Seite sichtbar. |
| **Kein Cloud-Zugang** | Keine Portfreigabe im Router einrichten. Der ESP32 ist nur lokal erreichbar. |

Zusätzlich empfohlen:
- **Gäste-WLAN** für Besuch nutzen. Das ist vom Heimnetz isoliert, Gäste erreichen den Türöffner dadurch automatisch nicht.
- Starkes WLAN-Passwort, WPA2/WPA3.
- PINs nicht auf den QR-Code drucken. Der QR-Code enthält nur die Adresse und ist damit für Fremde nutzlos.
- Bewusst **kein HTTPS** im Entwurf: Ein selbstsigniertes Zertifikat auf dem ESP32 erzeugt auf jedem Handy Warnmeldungen. Im eigenen WPA2-WLAN ist das Mitlesen der PIN ein überschaubares Risiko. Wer es härter will: ESP32 nur über einen Reverse-Proxy mit Zertifikat (z. B. auf einem Raspberry Pi/Home Assistant) ansprechen.
- Sitzungen liegen im RAM. Nach einem Neustart des ESP32 muss jeder einmal seine PIN neu eingeben (gewollt simpel, lässt sich später in den Flash verlagern).

---

## 6. Recht und Sicherheit (kurz)

- **Gegensprechanlage gehört dem Vermieter bzw. der Eigentümergemeinschaft.** Das Anklemmen im eigenen Haustelefon ist ein kleiner, rückbaubarer Eingriff, formal aber eine Veränderung an fremdem Eigentum. Am besten kurz bei Vermieter/Hausverwaltung nachfragen und alles so verbauen, dass es spurlos wieder rauskommt (Klemmen statt Löten an der Anlage).
- **Haustür betrifft alle Mitbewohner des Hauses.** Wer euer WLAN-Passwort hat, kann ab jetzt Fremde ins Haus lassen. PINs und Protokoll helfen, das nachvollziehbar zu halten.
- **Wohnungstür und Hausratversicherung:** Bei Einbruch ohne Spuren durch eine technische Öffnung kann die Versicherung Fragen stellen. Wer die Wohnungstür motorisch öffnen lässt, sollte das ggf. kurz mit der Versicherung klären oder die Wohnungstür-Funktion nur nutzen, wenn jemand zu Hause ist.
- **Fluchtweg:** Die Klinke muss immer frei von Hand bedienbar bleiben.
- **Elektrik:** Nur Kleinspannung. Keine Eingriffe am Klingeltrafo oder im Sicherungskasten.

---

## 7. Nächste Schritte

1. Haustelefon aufschrauben (Typ ist bekannt: Ritto TwinBus 4630/01), Platine und Verkabelung fotografieren und den **Türöffner-Taster auf der Platine** identifizieren.
2. Türöffner-Taste durchmessen (Spannung, ggf. Strom).
3. Klinkenkraft grob messen: Kofferwaage ans Griffende, runterziehen, Wert ablesen.
4. WLAN-Empfang an Haustür und Briefkasten prüfen.
5. Firmware flashen, erst nur mit LED statt Relais und Servo frei in der Hand testen.

Die passende Bestellreihenfolge dazu steht am Ende von [EINKAUFSLISTE.md](EINKAUFSLISTE.md): Welle 1 kann sofort raus, Welle 2 und 3 erst nach dem Messen.

---

## 8. Firmware flashen

Dateien: `firmware/tueroeffner/` (`tueroeffner.ino`, `config.example.h`, `webseite.h`).

1. Arduino IDE installieren, unter *Boardverwalter* „esp32 by Espressif“ (Version 3.x) installieren.
2. `config.example.h` nach `config.h` kopieren und dort WLAN-Daten, Pins und **eigene PINs** eintragen. `config.h` steht in der `.gitignore` und landet nie im Repo.
3. Board „ESP32 Dev Module“ wählen, hochladen, seriellen Monitor (115200 Baud) öffnen: dort steht die IP.
4. Im Router feste IP vergeben, QR-Code mit `http://<IP>/` erstellen.
5. Servo-Pulsbreiten `SERVO_RUHE_US` / `SERVO_GEDRUECKT_US` erst ohne Montage testen, dann an die Klinke anpassen.

Hinweis: Der Entwurf wurde nicht auf echter Hardware getestet und hier auch nicht kompiliert (die ESP32-Toolchain war in dieser Umgebung nicht ladbar). Beim ersten Kompilieren können kleine Anpassungen nötig sein.

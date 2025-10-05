# FA205
## FA205-Bibliotheken für MicrochipStudio
Die **Technischen Richtlinien FA205** sind maßgeblich für Unterricht und Berufsschul-Abschlußprüfung im Beruf **Elektroniker/-in für Geräte und Systeme** in Baden-Württemberg.
Die vorliegenden Bibliotheken wurden speziell für die Verwendung mit der **Microchip-Studio-IDE** zusammen mit einem **ATmega328pXplainedMini**-Conrollermodul erstellt. Sind aber selbstverständlich für alle Controllermodule mit ATmega328p-Controller, wie z.B. ArduinoUno oder ArduinoNano geeignet.

### Sonstige FA205-Impementierungen
Implementierungen sind auch für folgende Plattformen verfügbar: 
+ FA205 mit ArduinoUno/Nano (ATmega328P) und ArduinoIDE: https://github.com/feslehrer/FA205_Library_for_Arduino.git
+ FA205 mit ESP32 und ESP32-Carrier-Board: https://github.com/feslehrer/FA205-ESP32.git

## Arduino-Carrier-Board
Erstellt wurden die Bibliotheken mit der Hardwareplattform **Arduino-Carrier-Board** von **AS-Elektronik** (www.ase-schlierbach.de).
<br><img src="https://user-images.githubusercontent.com/24614659/236320998-94544814-81e8-421b-9627-0c5e2c16ead0.png" alt="Arduino-Carrier-Board" width="600">

### Carrier-Board mit aufgestecktem Controllermodul (hier: ATmega328pXplainedMini)
<img src="https://user-images.githubusercontent.com/24614659/236322408-e825f460-5135-4205-903b-463df35603ee.png" alt="Arduino-Carrier-Board mit ATmega328pXplainedMini" width="600">

### ATmega328pXplainedMini und Pinbelegung der ArduinoUno-Header
<img src="https://user-images.githubusercontent.com/24614659/236321583-8457da01-e94d-4db5-8539-40f2c315c2b6.png" alt="Arduino-Header" width="200"><img src="https://user-images.githubusercontent.com/24614659/236321645-c13d7fa3-2566-49c7-b55e-53918c5f59d4.png" alt="ESP32-Carrier-Board" width="400">

## Einrichtung der Richtlinien-Bibliotheken:
   In der Datei: <a href="Dokumente\1_Implementierung_AtmelStudio7_ATmega328.pdf">1_Implementierung_AtmelStudio7_ATmega328.pdf</a> befinden sich genaue Anleitungsschritte
   zur Einrichtung der Technischen Richtlinie FA205 in MicrochipStudio. 

1. Download der Zip-Datei im Repository.

2. Die Zip-Datei auf einen beliebigen Ordner entpacken. (Z.B. C:\Users\your_user\Documents\Controller)

3. In MicrochipStudio den Pfad zur Projektvorlage (MeinProjekt.zip) einstellen:
   Tools->Options->Projects->General-> user projects template location: "C:\Users\your_user\Documents\Controller\avr"

4. TeamSettings-Datei (AtmelStudio_Desktop_Settings.vssettings) laden:
   Tools->Options->Environment->Import and Export Settings-> check "Use team settings file" -> Dateinamen der Team-Settings-Datei
   eintragen:
     "C:\Users\your_user\Documents\Controller\avr\TeamSettings-2025-04-30.vssettings"

### Unterrichtsbeispiele für MicrochipStudio sind im Ordner "avr\examples" 
- <a href="Dokumente/Uebungen_Beispiele/2_2_Countdown_Siebensegment.pdf">2.2 Countdown Zaehler 7-Segment</a>
- <a href="Dokumente/Uebungen_Beispiele/4_2_1_Inkrementaler_Drehgeber_mit_Externem_Interrupt.pdf">4.2. Inkrementaler Drehgeber</a>  (<a href="https://ase-schlierbach.de/produkt/drehgeber-shield/">Drehgeber-Shield</a>)
- <a href="Dokumente/Uebungen_Beispiele/6_3_1_FES_Datenlogger.pdf">6.3. Datenlogger</a>   (<a href="https://ase-schlierbach.de/produkt/datalogger-shield/">Datalogger-Shield</a>)
- <a href="Dokumente/Uebungen_Beispiele/6_5_Einfache_Robotersteuerung.pdf">6.5. Robotersteuerung</a>   (<a href="https://ase-schlierbach.de/produkt/motorsteuerung-shield/">Roboter-Shield</a>)
- <a href="Dokumente/Uebungen_Beispiele/6_6_Drehzahlregelung_mit_PI_Regler.pdf">6.6. Drehzahlregelung mit PI Regler</a>  (<a href="https://ase-schlierbach.de/produkt/adapter-shield/">2mm-Adapter-Shield</a>)
- <a href="Dokumente/Uebungen_Beispiele/7_1_Ampelsteuerung_LfB.pdf">7.1. Ampelsteuerung</a>  (<a href="https://ase-schlierbach.de/produkt/ampel-shield/">Ampel-Shield</a>)
- <a href="Dokumente/Uebungen_Beispiele/7_2_MP3_Player_Steuerung.pdf">7.2. MP3-Player Steuerung</a>  (<a href="https://ase-schlierbach.de/produkt/mp3-player-shield/">MP3-Player-Shield</a>)


Für die Beispielprogramme werden z.T. Zusatzshields und Hardware benötigt: 
- https://ase-schlierbach.de
- http://www.hawell-elektronik.de/F_04Antrieb.htm

Projektseite:
- <a href="https://github.com/feslehrer/NiboBlueConfig_for_ArduinoCarrierBoard.git">NiboBee-Serial-Bluetooth-Adapter am Arduino-Carrier-Board</a>
- <a href="https://github.com/feslehrer/Temperaturmessung_4L">Temperaturmessung mit PT100 in Vierleiterschaltung</a>

Beispiel Arbeitsaufträge sind im Ordner **Dokumente\Uebungen_Beispiele**

letzte Änderung:
30.04.2025@Rah

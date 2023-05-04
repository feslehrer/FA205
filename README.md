# FA205
 FA205-Bibliotheken für MicrochipStudio

Einrichtung der Richtlinien-Bibliotheken:
   In der Datei: "Dokumente\1_Implementierung_AtmelStudio7_ATmega328.pdf" befinden sich genaue Anleitungsschritte
   zur Einrichtung der Technischen Richtlinie FA205 in MicrochipStudio. 
<img src="https://user-images.githubusercontent.com/24614659/236320998-94544814-81e8-421b-9627-0c5e2c16ead0.png" alt="ESP32-Carrier-Board" width="600">

<img src="https://user-images.githubusercontent.com/24614659/236322408-e825f460-5135-4205-903b-463df35603ee.png" alt="ESP32-Carrier-Board" width="600">

<img src="https://user-images.githubusercontent.com/24614659/236321583-8457da01-e94d-4db5-8539-40f2c315c2b6.png" alt="ESP32-Carrier-Board" width="200">
<img src="https://user-images.githubusercontent.com/24614659/236321645-c13d7fa3-2566-49c7-b55e-53918c5f59d4.png" alt="ESP32-Carrier-Board" width="400">

1. Download der Zip-Datei im Repository.

2. Die Zip-Datei auf C: entpacken. Die Ordnerstruktur sollte so aussehen:
   C:\FA205\avr\*

3. In MicrochipStudio den Pfad zur Projektvorlage (MeinProjekt.zip) einstellen:
   Tools->Options->Projects->General-> user projects template location: "C:\FA205\avr\"

4. TeamSettings-Datei (AtmelStudio_Desktop_Settings.vssettings) laden:
   Tools->Options->Environment->Import and Export Settings-> check "Use team settings file" -> Dateinamen der Team-Settings-Datei
   eintragen:
     "C:\FA205\avr\AtmelStudio_Desktop_Settings.vssettings"

Unterrichtsbeispiele für MicrochipStudio sind im Ordner "avr\examples" 
- 2_2_Countdown_Zaehler_7_Segment
- 4_2_Inkrementaler_Drehgeber  (Drehgeber-Shield)
- 6_3_Datenlogger   (Datalogger-Shield)
- 6_5_Robotersteuerung   (Roboter-Shield)
- 6_6_Drehzahlregelung_mit_PI_Regler  (2mm-Adapter-Shield)
- 7_1_Ampelsteuerung  (Ampel-Shield)


Für die Beispielprogramme werden z.T. Zusatzshields und Hardware benötigt: 
- https://ase-schlierbach.de
- http://www.hawell-elektronik.de/F_04Antrieb.htm 

Beispiel Arbeitsaufträge sind im Ordner "Dokumente\Uebungen_Beispiele"

letzte Änderung:
4.5.2023@Rah

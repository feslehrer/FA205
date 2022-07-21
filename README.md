# FA205
 FA205-Bibliotheken für MicrochipStudio

Einrichtung der Richtlinien-Bibliotheken:
   In der Datei: "Dokumente\1_Implementierung_AtmelStudio7_ATmega328.pdf" befinden sich genaue Anleitungsschritte
   zur Einrichtung der Technischen Richtlinie FA205 in MicrochipStudio. 

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
2_2_Countdown_Zaehler_7_Segment
4_2_Inkrementaler_Drehgeber  (Drehgeber-Shield)
6_3_Datenlogger   (Datalogger-Shield)
6_5_Robotersteuerung   (Roboter-Shield)
6_6_Drehzahlregelung_mit_PI_Regler  (2mm-Adapter-Shield)
7_1_Ampelsteuerung  (Ampel-Shield)


Für die Beispielprogramme werden z.T. Zusatzshields und Hardware benötigt: 
https://ase-schlierbach.de
http://www.hawell-elektronik.de/F_04Antrieb.htm 

Beispiel Arbeitsaufträge sind im Ordner "Dokumente\Uebungen_Beispiele"

letzte Änderung:
21.07.2022@Rah

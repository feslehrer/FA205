# FA205
 FA205-Bibliotheken für MicrochipStudio

Einrichtung der Richtlinien-Bibliotheken:
   In der Datei: "Dokumente\1_Implementierung_AtmelStudio7_ATmega328.pdf" befinden sich genaue Anleitungsschritte
   zur Einrichtung der Technischen Richtlinie FA205 in MicrochipStudio. 

1. Download der Zip-Datei im Repository.

2. Die Zip-Datei C: entpacken. Die Ordnerstruktur sollte so aussehen:
   C:\FA205\avr\*

3. In MicrochipStudio den Pfad zur Projektvorlage (MeinProjekt.zip) einstellen:
   Tools->Options->Projects->General-> user projects template location: "C:\FA205\avr\"

4. TeamSettings-Datei (AtmelStudio_Desktop_Settings.vssettings) laden:
   Tools->Options->Environment->Import and Export Settings-> check "Use team settings file" -> Dateinamen der Team-Settings-Datei
   eintragen:
     "C:\FA205\avr\AtmelStudio_Desktop_Settings.vssettings"


letzte Änderung:
24.02.2022@Rah

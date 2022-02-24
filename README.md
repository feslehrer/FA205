# FA205
 FA205-Bibliotheken für MicrochipStudio

Einrichtung der Richtlinien-Bibliotheken:
   In der Datei: "1_Implementierung_AtmelStudio7_ATmega328.pdf" befinden sich genaue Anleitungsschritte

1. Die Zip-Datei C: entpacken. Die Ordnerstruktur sollte so aussehen:
   C:\FA205\avr\*

2. In MicrochipStudio den Pfad zur Projektvorlage (MeinProjekt.zip) einstellen:
   Tools->Options->Projects->General-> user projects template location: "C:\FA205\avr\"

3. TeamSettings-Datei (AtmelStudio_Desktop_Settings.vssettings) laden:
   Tools->Options->Environment->Import and Export Settings-> check "Use team settings file" -> Dateinamen der Team-Settings-Datei
   eintragen:
     "C:\FA205\avr\AtmelStudio_Desktop_Settings.vssettings"


letzte Änderung:
24.02.2022@Rah
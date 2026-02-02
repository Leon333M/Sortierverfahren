Hier ist eine kurze Anleitung, um zu erklären, wie die EXE gestartet werden muss.

Um alle int-Messungen zu machen, muss die EXE z.B. so gestartet werden:
.\build\Release\Sortierverfahren.exe 1200 a a a a

Um alle String-Messungen zu machen, muss die EXE z.B. so gestartet werden:
.\build\Release\Sortierverfahren.exe 1200 a a s a

Natürlich können auch direkt einzelne Messungen gemacht werden:

Das 1. Argument (die Zahl) dient dazu anzugeben, welche Arraygröße gemessen werden soll.

Das 2. Argument gibt an, welche implementierte Variante gemessen werden soll:
g = Grundlaufzeiten (sequenziell)
p = Tiefenbasierte Thread-Erzeugung
w = Worker-Thread-Variante
m = Tiefenmessung (sequenziell)
o = Tiefenmessung (Tiefenbasierte Thread-Erzeugung)
a = alle Varianten
i = Inkrement-Array

Das 3. Argument gibt an, welcher Listentyp gemessen werden soll:
z = Zufall
s = Sortiert
i = Invertiert sortiert
f = Fast sortiert
d = Dupliziert
a = Alle Listentypen

Das 4. Argument ist nur dafür da, um String-Messungen zu machen. 
Wenn dieses Argument "s" ist, wird zu String gewechselt, sonst wird es ignoriert und es bleibt bei int-Messungen.

Das 5. Argument dient dem ausführlichen Messen von allem.
Daher wird nur geprüft, ob vorhanden, und wenn ja, werden alle Messungen automatisiert gestartet.

Um beispielsweise bei einem int-Array der Größe 1200 die Worker-Thread-Variante eine Zufallsliste zu messen, 
ist dies der Startbefehl:
.\build\Release\Sortierverfahren.exe 1200 w z

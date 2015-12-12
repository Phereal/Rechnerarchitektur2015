Aufgabe 3 (10 Punkte)

Im Tutorium wurde ein switch vorgestellt, der einen Ein- und mehrere Ausgänge besaß und Pakete
zwischen diesen vermittelt hat.
In dieser Aufgabe erweitert ihr dieses Konzept um mehrere Eigenschaften. Zum einen ist die Anzahl
an Ein- und Ausgängen nicht mehr statisch festgelegt, sondern wird variabel bei der Erzeugung des
switches angegeben. Zusätzlich arbeiten die Absender schneller als die Empfänger (d.h. auf unter-
schiedlichen clocks).
Baut dafür ein SystemC-Programm, das (mindestens) folgende Klassen besitzt:
• packet ist die Datenstruktur, die eure Pakete beschreibt. Ein Paket enthält dabei eine Zieladresse
die angibt, auf welchen Ziel-Port es geschickt werden soll, ein prioritäts-Flag das angibt ob das
Paket priorisiert (also vor anderen, nicht priorisierten Paketen) durchgeleitet werden soll sowie
das Datum selbst – letzteres wird als unsigned long long kodiert.
• switch erbt von sc_module und ist dafür zuständig, Pakete von einem Eingang an einen anderen
Ausgang zu vermitteln. Dafür besitzt die switch-Klasse n Eingänge und m Ausgänge sowie
einen Eingang für eine Clock.
• producer und consumer stellen die Klassen dar, die an den Eingängen bzw. Ausgängen des
switches Angeschlossen werden und generieren die Pakete bzw. nehmen diese ein Empfang.
Dafür enthalten diese jeweils einen Aus- oder Eingang sowie einen Eingang für eine Clock.
Das Programm wird (aus der Kommandozeile) auf folgende Weise gestartet:
switch numIn numOut genSpeed inClock outClock bufferSize
Dabei beschreiben die Parameter folgende Eigenschaften:
• numIn und numOut legen die Anzahl der Ein- und Ausgänge fest und liegt jeweils zwischen 0
und 127.
• genSpeed legt (in Prozent von 0 bis 100) die Wahrscheinlichkeit fest, mit der ein producer ein
Paket (pro Clock-Zyklus) generiert.
• inClock und outClock legen die clock-Geschwindigkeiten für producer, consumer und den
switch (der so schnell läuft die die producer) in NS von 1 bis 100 fest. Es muss inClock =
outClock gelten.
• bufferSize steuert, wie groß der fifo-Puffer pro Ausgang ist, um Pakete zwischenzuspeichern.
Da die consumer langsamer getaktet sein können als der switch können die Pakete nicht in
jedem Takt weitergegeben werden; entsprechend müssen sie teilweise gepuffert werden. Beachtet
für die Pufferung die Prioritätsinformation der Pakete, die manche Pakete in der Queue vorne
einsortiert werden lässt.
Die Signale sind per Templates auf eure packet-Typen zu spezifizieren (d.h. sc_in<packet> etc.).
Solltet ihr zusätzliche Signale oder Ports (z.B. zur Kommunikation unter den Modulen) benötigen dürft
ihr diese beliebig hinzufügen.
Ermittelt über verschiedene Testläufe, ab wann euer Switch anfängt Pakete zu verlieren. Welche Para-
meter spielen dafür eine Rolle, welche nicht? Warum?
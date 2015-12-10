Aufgabe 3 (10 Punkte)

Im Tutorium wurde ein switch vorgestellt, der einen Ein- und mehrere Ausg�nge besa� und Pakete
zwischen diesen vermittelt hat.
In dieser Aufgabe erweitert ihr dieses Konzept um mehrere Eigenschaften. Zum einen ist die Anzahl
an Ein- und Ausg�ngen nicht mehr statisch festgelegt, sondern wird variabel bei der Erzeugung des
switches angegeben. Zus�tzlich arbeiten die Absender schneller als die Empf�nger (d.h. auf unter-
schiedlichen clocks).
Baut daf�r ein SystemC-Programm, das (mindestens) folgende Klassen besitzt:
� packet ist die Datenstruktur, die eure Pakete beschreibt. Ein Paket enth�lt dabei eine Zieladresse
die angibt, auf welchen Ziel-Port es geschickt werden soll, ein priorit�ts-Flag das angibt ob das
Paket priorisiert (also vor anderen, nicht priorisierten Paketen) durchgeleitet werden soll sowie
das Datum selbst � letzteres wird als unsigned long long kodiert.
� switch erbt von sc_module und ist daf�r zust�ndig, Pakete von einem Eingang an einen anderen
Ausgang zu vermitteln. Daf�r besitzt die switch-Klasse n Eing�nge und m Ausg�nge sowie
einen Eingang f�r eine Clock.
� producer und consumer stellen die Klassen dar, die an den Eing�ngen bzw. Ausg�ngen des
switches Angeschlossen werden und generieren die Pakete bzw. nehmen diese ein Empfang.
Daf�r enthalten diese jeweils einen Aus- oder Eingang sowie einen Eingang f�r eine Clock.
Das Programm wird (aus der Kommandozeile) auf folgende Weise gestartet:
switch numIn numOut genSpeed inClock outClock bufferSize
Dabei beschreiben die Parameter folgende Eigenschaften:
� numIn und numOut legen die Anzahl der Ein- und Ausg�nge fest und liegt jeweils zwischen 0
und 127.
� genSpeed legt (in Prozent von 0 bis 100) die Wahrscheinlichkeit fest, mit der ein producer ein
Paket (pro Clock-Zyklus) generiert.
� inClock und outClock legen die clock-Geschwindigkeiten f�r producer, consumer und den
switch (der so schnell l�uft die die producer) in NS von 1 bis 100 fest. Es muss inClock =
outClock gelten.
� bufferSize steuert, wie gro� der fifo-Puffer pro Ausgang ist, um Pakete zwischenzuspeichern.
Da die consumer langsamer getaktet sein k�nnen als der switch k�nnen die Pakete nicht in
jedem Takt weitergegeben werden; entsprechend m�ssen sie teilweise gepuffert werden. Beachtet
f�r die Pufferung die Priorit�tsinformation der Pakete, die manche Pakete in der Queue vorne
einsortiert werden l�sst.
Die Signale sind per Templates auf eure packet-Typen zu spezifizieren (d.h. sc_in<packet> etc.).
Solltet ihr zus�tzliche Signale oder Ports (z.B. zur Kommunikation unter den Modulen) ben�tigen d�rft
ihr diese beliebig hinzuf�gen.
Ermittelt �ber verschiedene Testl�ufe, ab wann euer Switch anf�ngt Pakete zu verlieren. Welche Para-
meter spielen daf�r eine Rolle, welche nicht? Warum?
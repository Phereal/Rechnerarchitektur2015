Aufgabe 2 (8 Punkte)

Eine Arithemtisch-Logische Einheit (ALU) führt in einer CPU Berechnungen durch. Implementiert eine
Klasse alu, die von sc_module erbt und folgende Eigenschaften aufweist:
• Sie besitzt einen Clock-Eingang.
• SiebesitzteinenEinganginstructionfürdieauszuführendeInstruktionvomTypsc_uint<8>.
• Sie besitzt zwei Eingänge für die Daten dataA und dataB vom Typ sc_int<64>
• Sie besitzt einen Ausgang result für das Resultat vom Typ sc_int<64>
Die ALU soll folgende Operationen ausführen können (mit den entsprechenden Werten für den In-
struktionseingan in Klammern): Arithmetische Operationen: Addition (0x01), Subtraktion (0x02), Mul-
tiplikation (0x03), Division (0x04) sowie Modulo (0x05). Bitweise Operationen: Bitshifting (wobei der
zweite Operand die Distanz und die Richtung angibt) (0x10), bitweises OR (0x11), bitweises AND (0x12)
sowie bitweises XOR (0x13). Außerdem soll ein Vergleich (0x14) realisiert werden. Dieser verhält sich
wie folgt: das Ergebnis ist 0, wenn die Operanden gleich sind bzw. -1 oder 1 wenn der zweite Operand
kleiner bzw. größer ist.
Das Ergebnis der ALU soll im nächsten Takt an result anliegen.
Aufgabenstellung [Zusammengefasst]
##################################

* Durch ein NOC soll eine Bildfaltung durchgef�hrt werden
	+ [Formel ist hoerf�r in Aufgabenstellung enthalten]
* Das NOC hat dabei 8x8 [64] Pl�tzer f�r Module
* [Router] Jedes Modul hat einen eigenen Router. Jeder Router ist, wenn vorhanden mit allen anderen um ihn rum verbunden
* [CLK] Das System wird Synchron �ber eine gemeinsame Clock getaktet
* [RAM] Ein Modul ist das interne RAM, die Schnittstelle zum Hauptspeicher, in dem das Bild abgelegt ist 
	+ [halbe Taktgeschwindigkeit]
	+ [muss am Rand des NOC liegen]
* [CACHE] Caches werden genutzt um anfragen an den Hauptspeicher schneller bearbeiten zu k�nnen
* [COMPUTE] F�hren die Berechnung auf Grundlage der in der Aufgabe genannten Formel durch
	+ [Auf Berechnungsinformationen greifen Sie auf Grundlage von einem load() Befehl zu]
	+ [Das Ergebnis der Faltungsoperation wird per store() Befehl in den RAM geschrieben
	+ [�ber ein wait Statement wird definiert, wie lange eine Berechnung dauern kann]
* [Gatway] Das Gateway-Moudul stellt die Verkn�pfung des NOC nach au�en dar
	+ [per START Befehl wird die Berechnung gestartet, dabei liegt das Bild bereits im lokalen RAM vor]
	+ [das System meldet, dass die Berechnung abgeschlossen ist]


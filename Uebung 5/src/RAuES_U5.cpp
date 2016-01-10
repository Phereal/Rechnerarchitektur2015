/*
 * RAuES_U5.cpp
 * In dieser Klasse wird das Programm gestartet, und die einzelnen Router-Objekte werden erzeugt.
 * Jedes Router-Objekt verfügt über ein Modul-Objekt, welches RAM, Gaetway, Cache oder Compute sein kann.
 * Diese Module erben von der abstrakten Klasse "module", welches die Kommunikationsschnittstelle
 * zum Router-Objekt darstellt.
 *
 * Die Router-Objekte tauschen untereinander Objekte der Klasse "Paket" aus, diese beinhalten die
 * Bildinformationen die initial im RAM gespeichert sind. Der RAM enthält dabei die Speicherelemente
 * des alten Bildes und zusätzlich des (neu) zu entstehenden Bildes.
 * Die Speicherelemente des neuen Bildes werden während des Programmdurchlaufes durch
 * die Compute-Module mit Informationen gefüllt.
 *
 * Der Programmablauf wird über das Gateway-Modul gestartet, welches immer mit dem letzten Element
 * des Router-Objekt-Arrays verbunden ist. [Bei einem 8x8 Array das Element an Stelle [7][7].
 *
 * Das Erste Router-Objekt an Array-Position [0][0] ist immer das Router-Objekt, das mit dem
 * RAM-Modul verbunden ist. Das RAM-Modul verfügt über zwei getrennte Speicherbänke,
 * eine Speicherbank für das Originalbild und eine Speicherbank für das neu berechnete Bild.
 * Wenn das Programm gestartet wird, erhält das RAM-Modul vom Gateway-Modul ein Paket mit dem
 * OPCODE [rfi] (readfile) wodurch über die Klasse 'pgm' das betreffende Bild in das RAM
 * eingelesen wird. Wenn dieser Vorgang a *
 *  * Wenn Caches umgesetzt werden, erhält der Router als Informationsquelle in seinem Routing-Table nicht die Adresse des RAM-Moduls,
 * sondern die Adresse des nächstgelegenen Caches. Es ist Aufgabe des Caches, die Informationen aus dem RAM zu erhalten und im eigenen
 * Speicher vorzuhalten.bgeschlossen ist, sendet das RAM-Modul eine
 * Bestätiung über den Abschluss des Prozesses mit dem OPCODE [rff] (readfilefinished).
 *
 * Anschließend beginnt das Gateway-Modul mit der Berechnung.
 *
 * Das Gateway-Modul steuert dabei die einzelnen Compute-Module und teilt ihnen mit, welchen Pixel
 * sie als nächstes berechnen sollen. Dabei sind dem Gateway-Modul alle Adressen (der Compute-Module
 * bekannt, sodass der Execute Befehl direkt an das betreffende Compute-Modul gesendet werden kann.
 *
 * Sobald das Compute-Modul den Execute-Befehl erhalten hat, fängt dieses damit an, die
 * betreffenden Informationen über die Nachbarpixel aus den am nächsten Cache abzufragen.
 *
 * Wenn der Cache nicht über den entsprechenden Pixel verfügt, wird dieser aus dem RAM in
 * den Cache nachgeladen. Hierbei wird die einfache Verdrängungsstrategie FIFO angewendet.
 *
 * Befindet sich der entsprechende Pixel im Cache, wird dieser an das Compute-Modul gesendet.
 *
 * Wenn alle Informationen zur Verfügung stehen, wird die Berechnung durchgeführt und der
 * neu entstandene Pixel an den RAM gesendet.
 *
 * Gleichzeitig wird eine Meldung an das Gateway gesendet, dass die Berechnung abgeschlossen
 * wurde und eine neuer Execute-Befehl empfangen werden kann.
 *
 * Wenn alle Berechnungen durchgeführt wurden sendet das Gateway-Modul ein Paket mit dem
 * OPCODE [wfi] (writefile) an das RAM-Modul.
 *
 * Daraufhin schreibt das RAM-MODUL den Inhalt der zweiten Speicherbank über die Klasse 'pgm'
 * in eine Datei mit dem Namen 'noc.pgm'. Wenn dieser Vorgang abgeschlossen ist, sendet das
 * RAM-Modul ein Paket mit dem OPCODE [wff] (writefilefinished) an das Getway-Modul,
 * woraufhin der Programmablauf beendet wird.
 *
 * Generell wird bei jedem Kommunikationsvorgang eine Empfangsbestätigung gesendet, da wir
 * in dieser Lösung mit Sende- und Empfangsbuffer arbeiten.
 *
 * Die entsprechenden Pakete werden erst aus den SendeBuffern gelöscht, wenn die
 * Empfangsbestätigung eingetroffen ist. So kann garantiert werden, dass keine Pakete
 * verloren gehen.
 */

#include <iostream>

#include <systemc.h>

#include "pgm.h"
#include "router.h"

using namespace std;

int sc_main(int, char *[])
{
  // ------------------------------
  // Clock
  // Die Clock setzt den synchronen Systemtakt um, den die Aufgabenstellung verlangt.
  // Die langsamere Taktung des RAM-Modules wird dabei innerhalb der RAM-Klasse umgesetzt.
  // ------------------------------
  sc_clock clk("clk", 10, SC_NS, 0.5);

  // ------------------------------
  // NOC-Array
  // Dieses Array stellt das NOC dar. Wie aus der Aufgabenstellung werden hierbei
  // 8x8 Router-Objekt erzeugt, die jeweils über ein verbundenes Modul verfügen.
  // ------------------------------
//  int NOCBORDER = 8;
//  router noc[NOCBORDER][NOCBORDER];
  router ro("RouterTest", 0, 4, 10);

  // ------------------------------
  // Starte Simulation
  // ------------------------------
  sc_start();

  return 0;
}

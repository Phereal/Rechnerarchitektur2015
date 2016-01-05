/*
 * RAuES_U5.cpp
 * In dieser Klasse wird das Programm gestartet, und die einzelnen Router-Objekte werden erzeugt.
 * Jedes Router-Objekt verfügt über ein Modul-Objekt, welches RAM, Gatway, Cache oder Compute sein kann.
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
 * RAM-Modul verbunden ist.
 *
 * Das Gateway-Modul steuert dabei die einzelnen Compute-Module und teilt ihnen mit, welchen Pixel
 * sie als nächstes berechnen sollen. Dabei sind dem Gateway-Modul alle Adresse bekannt, sodass
 * der Execute Befehl direkt an das betreffende Compute-Modul gesendet werden kann.
 *
 * Sobald das Compute-Modul den Execute-Befehl erhalten hat, fängt dieses damit an, die
 * betreffenden Informationen über die Nachbarpixel abzufragen.
 * Wenn alle Informationen zur Verfügung stehen, wird die Berechnung durchgeführt und der
 * neu entstandene Pixel an den RAM gesendet.
 * Gleichzeitig wird eine Meldung an das Gateway gesendet, dass die Berechnung abgeschlossen
 * wurde und eine neuer Execute-Befehl empfangen werden kann.
 *
 * Für die Synchronisation der Berechnung wird der Modul
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
  sc_clock clk("clk", Clock, SC_NS, 0.5);

  // ------------------------------
  // NOC-Array
  // Dieses Array stellt das NOC dar. Wie aus der Aufgabenstellung werden hierbei
  // 8x8 Router-Objekt erzeugt, die jeweils über ein verbundenes Modul verfügen.
  // ------------------------------
  int NOCBORDER = 8;
  router noc[NOCBORDER][NOCBORDER];

  // ------------------------------
  // Starte Simulation
  // ------------------------------
  sc_start();

  return 0;
}

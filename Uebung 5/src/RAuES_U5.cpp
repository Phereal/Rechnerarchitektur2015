/*
 * RAuES_U5.cpp
 * In dieser Klasse wird das Programm gestartet, und die einzelnen Router-Objekte werden erzeugt.
 * Jedes Router-Objekt verfügt über ein Modul-Objekt, welches RAM, Gatway, Cache oder Compute sein kann.
 * Diese Module erben von der abstrakten Klasse "module", welche die Kommunikationsschnittstelle
 * zum Router-Objekt darstellt.
 *
 * Die Router-Objekte kommunizieren untereinander über Objekte der Klasse "pixel", diese beinhalten die
 * Bildinformationen die initial im RAM gespeichert sind.
 *
 * Der Programmablauf wird über das Gateway-Modul gestartet, welches immer mit dem letzten Element
 * des Router-Objekt-Arrays verbunden ist. [Bei einem 8x8 Array das Element an Stelle [7][7].
 *
 * Das Erste Router-Objekt an Array-Position [0][0] ist immer das Router-Objekt, das mit dem
 * RAM-Modul verbunden ist.
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

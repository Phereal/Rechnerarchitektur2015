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
#include <string>

#include <systemc.h>

#include "RAuES_U5.h"

#include "pgm.h"
#include "paket.h"
#include "router.h"
#include "module.h"
#include "ram.h"
#include "gateway.h"
#include "cache.h"
#include "compute.h"

using namespace std;


/*
 * Das Klasse waiter dient dazu, die Simulation nach 1000ms zu stoppen. Damit,
 * falls z.B. irgend ein unvorhersehbarer Fehler auftritt, die Anwendung nicht
 * unendlich laueft, werden diese Klasse verwendet.
 */
#ifdef K_DEBUG
SC_MODULE(waiter)
{
    void waiting()
    {
      wait(1000, SC_NS);
      sc_stop();
      cout << "Simulation wurde durch \"waiter\" gestoppt!" << endl;
    }

    SC_CTOR(waiter)
    {
      SC_THREAD(waiting);
    }
};
#endif



#ifdef K_DEBUG
SC_MODULE(starter)
{
  // Startsignal fuer gateway
  sc_out<bool> start;

  sc_in<bool> clk;

  void starting()
  {
    // halte start 10 takte lang auf 1
    if( waitCnt < 10 )
    {
      start.write(true);
      waitCnt++;
    }
    else
    {
      start.write(false);
    }
  }

  // Konstruktor der Klasse starter die die Methode starting() aufruft.
  SC_CTOR(starter)
  {
    SC_METHOD(starting);
    sensitive << clk.pos();
  }

  private:
  uint32_t waitCnt = 0;
};
#endif



int sc_main(int argc, char *argv[])
{
  std::string inFile;
  std::string outFile;

  // ------------------------------
  // Check main parameters
  // ------------------------------
  if(argc > 1)
  {
    inFile = argv[1];
  }
  if(argc > 2)
  {
    outFile = argv[2];
  }


  // ------------------------------
  // Clock
  // Die Clock setzt den synchronen Systemtakt um, den die Aufgabenstellung verlangt.
  // Die langsamere Taktung des RAM-Modules wird dabei innerhalb der RAM-Klasse umgesetzt.
  // ------------------------------
  sc_clock clk("clk", 10, SC_NS, 0.5);
  sc_clock clkSlow("clkSlow", 20, SC_NS, 0.5);



  // ------------------------------
  // NOC-Array
  // Dieses Array stellt das NOC dar. Wie aus der Aufgabenstellung werden hierbei
  // 8x8 Router-Objekt erzeugt, die jeweils über ein verbundenes Modul verfügen.
  const size_t K_NOC_SIZE = 8;
  const uint32_t K_ROUTER_BUFFER_SIZE = 10;
  const uint32_t K_RAM_BUFFER_SIZE = 10;
  const uint32_t K_GATEWAY_BUFFER_SIZE = 10;
  const uint32_t K_GATEWAY_PIXEL_BUFFER_SIZE = 10;
  const uint32_t K_COMPUTE_BUFFER_SIZE = 10;
  const uint32_t K_CACHE_BUFFER_SIZE = 10;



  // Signale
  sc_signal<paket> routeList[K_NOC_SIZE][K_NOC_SIZE][4];
  sc_signal<paket> moduleRouteList[K_NOC_SIZE][K_NOC_SIZE][2];
  sc_signal<bool> start;



  // ------------------------------
  // Erstellen der Objekte der Klassen
  // ------------------------------
  #ifdef K_DEBUG
  waiter w("Waiter");
  starter s("Starter");
  #endif
  router *routerList[K_NOC_SIZE][K_NOC_SIZE];
  module *moduleList[K_NOC_SIZE][K_NOC_SIZE];

  // router erzeugen
  PRINT_DEBUG("main - router erzeugen");
  for(size_t y = 0; y < K_NOC_SIZE; ++y)
  {
    for(size_t x = 0; x < K_NOC_SIZE; ++x)
    {
      uint8_t routen;
      string name = "Router[";
      name.append(to_string(y));
      name.append(",");
      name.append(to_string(x));
      name.append("]");
      uint8_t id = (uint8_t)y;
      id |= (uint8_t)((x << 4) & 0xF0);
      RoutingRichtung routeRichtungen[4]; // Verdrahtung imm im UZS, beginnend bei UP wenn moeglich

      if(y == 0)
      {
        if(x == 0)
        {
          // 1. Reihe, 1. Element
          routen = 2;
          routeRichtungen[0] = RoutingRichtung::RIGTH;
          routeRichtungen[1] = RoutingRichtung::DOWN;
        }
        else if(x == (K_NOC_SIZE - 1))
        {
          // 1. Reihe, letztes Element
          routen = 2;
          routeRichtungen[0] = RoutingRichtung::DOWN;
          routeRichtungen[1] = RoutingRichtung::LEFT;
        }
        else
        {
          // 1. Reihe, mittleres Element
          routen = 3;
          routeRichtungen[0] = RoutingRichtung::RIGTH;
          routeRichtungen[1] = RoutingRichtung::DOWN;
          routeRichtungen[2] = RoutingRichtung::LEFT;
        }
      }
      else if(y == (K_NOC_SIZE - 1))
      {
        if(x == 0)
        {
          // Letzte Reihe, 1. Element
          routen = 2;
          routeRichtungen[0] = RoutingRichtung::UP;
          routeRichtungen[1] = RoutingRichtung::RIGTH;
        }
        else if(x == (K_NOC_SIZE - 1))
        {
          // Letzte Reihe, letztes Element
          routen = 2;
          routeRichtungen[0] = RoutingRichtung::UP;
          routeRichtungen[1] = RoutingRichtung::LEFT;
        }
        else
        {
          // Letzte Reihe, mittleres Element
          routen = 3;
          routeRichtungen[0] = RoutingRichtung::UP;
          routeRichtungen[1] = RoutingRichtung::RIGTH;
          routeRichtungen[2] = RoutingRichtung::LEFT;
        }
      }
      else
      {
        if(x == 0)
        {
          // Mittlere Reihe, 1. Element
          routen = 3;
          routeRichtungen[0] = RoutingRichtung::UP;
          routeRichtungen[1] = RoutingRichtung::RIGTH;
          routeRichtungen[2] = RoutingRichtung::DOWN;
        }
        else if(x == (K_NOC_SIZE - 1))
        {
          // Mittlere Reihe, letztes Element
          routen = 3;
          routeRichtungen[0] = RoutingRichtung::UP;
          routeRichtungen[1] = RoutingRichtung::DOWN;
          routeRichtungen[2] = RoutingRichtung::LEFT;
        }
        else
        {
          // Mittlere Reihe, mittleres Element
          routen = 4;
          routeRichtungen[0] = RoutingRichtung::UP;
          routeRichtungen[1] = RoutingRichtung::RIGTH;
          routeRichtungen[2] = RoutingRichtung::DOWN;
          routeRichtungen[3] = RoutingRichtung::LEFT;
        }
      }

      // Komponente erzeugen
      routerList[y][x] = new router(name.c_str(), id, routen, routeRichtungen,
          K_ROUTER_BUFFER_SIZE);
    }
  }
  PRINT_DEBUG("main - router erzeugt");

  // module erzeugen
  PRINT_DEBUG("main - module erzeugen");
  size_t cacheCnt = 0;
  uint8_t cacheIdList[K_NOC_SIZE * K_NOC_SIZE] = {0}; // array mit max moeglicher groesse um das speichermanagement zu vereinfachen
  for(size_t y = 0; y < K_NOC_SIZE; ++y)
  {
    for(size_t x = 0; x < K_NOC_SIZE; ++x)
    {
      string name;
      uint8_t id = (uint8_t)y;
      id |= (uint8_t)((x << 4) & 0xF0);

      if((y == 0) && (x == 0))
      {
        // Ram at top left
        name = "Ram[";
        name.append(to_string(y));
        name.append(",");
        name.append(to_string(x));
        name.append("]");
        moduleList[y][x] = new ram(name.c_str(), id, K_RAM_BUFFER_SIZE, inFile, outFile);
        PRINT_DEBUG("main - ram erzeugt");
      }
      else if((y == (K_NOC_SIZE - 1)) && (x == (K_NOC_SIZE - 1)))
      {
        // Gateway at bottom right
        // wird spaeter erzeugt, da es mit einer liste aller compute-module ids erzeugt wird
      }
      else
      {
        if(((x == 2) || (x == 5))
            && ((y == 1) || (y == 3) || (y == 5) || (y == 7)))
        {
          // Cache at specific positions (see router.h)
          name = "Cache[";
          name.append(to_string(y));
          name.append(",");
          name.append(to_string(x));
          name.append("]");
          moduleList[y][x] = new cache(name.c_str(), id, K_CACHE_BUFFER_SIZE, 0); // 0 = ramId
          cacheIdList[cacheCnt++] = id; // id liste fuer compute
        }
        else
        {
          // Compute everywhere else
          // wird spaeter erzeugt, da es mit einer liste aller cache-module ids erzeugt wird
        }

      }
    }
  }
  PRINT_DEBUG("main - module erzeugt");

  // module erzeugen
  PRINT_DEBUG("main - compute erzeugen");
  size_t computeCnt = 0;
  uint8_t computeIdList[K_NOC_SIZE * K_NOC_SIZE] = {0}; // array mit max moeglicher groesse um das speichermanagement zu vereinfachen
  for(size_t y = 0; y < K_NOC_SIZE; ++y)
  {
    for(size_t x = 0; x < K_NOC_SIZE; ++x)
    {
      string name;
      uint8_t id = (uint8_t)y;
      id |= (uint8_t)((x << 4) & 0xF0);

      if((y == 0) && (x == 0))
      {
        // Ram at top left
        name = "Ram[";
        name.append(to_string(y));
        name.append(",");
        name.append(to_string(x));
        name.append("]");
        moduleList[y][x] = new ram(name.c_str(), id, K_RAM_BUFFER_SIZE, inFile, outFile);
        PRINT_DEBUG("main - ram erzeugt");
      }
      else if((y == (K_NOC_SIZE - 1)) && (x == (K_NOC_SIZE - 1)))
      {
        // Gateway at bottom right
        // wird spaeter erzeugt, da es mit einer liste aller compute-module ids erzeugt wird
      }
      else
      {
        if(((x == 2) || (x == 5))
            && ((y == 1) || (y == 3) || (y == 5) || (y == 7)))
        {
          // Cache at specific positions (see router.h)
          // wurde zuvor erzeugt
        }
        else
        {
          // Compute everywhere else
          name = "Compute[";
          name.append(to_string(y));
          name.append(",");
          name.append(to_string(x));
          name.append("]");
          uint8_t gwId = (uint8_t)7;
          gwId |= (uint8_t)((7 << 4) & 0xF0);
          moduleList[y][x] = new compute(name.c_str(), id, K_COMPUTE_BUFFER_SIZE, 0, gwId, cacheIdList, cacheCnt);
          computeIdList[computeCnt++] = id; // id liste fuer gateway
        }

      }
    }
  }
  PRINT_DEBUG("main - compute erzeugt");

  // Gateway at bottom right
  PRINT_DEBUG("main - gateway erzeugen");
  {
    string name = "Gateway[";
    name.append(to_string(K_NOC_SIZE - 1));
    name.append(",");
    name.append(to_string(K_NOC_SIZE - 1));
    name.append("]");
    uint8_t id = (uint8_t)(K_NOC_SIZE - 1);
    id |= (uint8_t)(((K_NOC_SIZE - 1) << 4) & 0xF0);
    moduleList[K_NOC_SIZE - 1][K_NOC_SIZE - 1] = new gateway(name.c_str(), id, K_GATEWAY_BUFFER_SIZE, 0, computeIdList, computeCnt, K_GATEWAY_PIXEL_BUFFER_SIZE); // 0 = ramId
  }
  PRINT_DEBUG("main - gateway erzeugt");

  // ------------------------------
  // Connect Signals
  // ------------------------------
  // Übergeben der Signale an die Klasse starter
  #ifdef K_DEBUG
  s.clk(clk);
  s.start(start);
  #endif

  PRINT_DEBUG("main - signale verbinden");
  for(size_t y = 0; y < K_NOC_SIZE; ++y)
  {
    for(size_t x = 0; x < K_NOC_SIZE; ++x)
    {
      PRINT_DEBUG("  main - [" + to_string(y) + "," + to_string(x) + "] verbinden");
      routerList[y][x]->clk(clk);

      routerList[y][x]->moduleOut(moduleRouteList[y][x][0]);
      moduleList[y][x]->routerIn(moduleRouteList[y][x][0]);

      routerList[y][x]->moduleIn(moduleRouteList[y][x][1]);
      moduleList[y][x]->routerOut(moduleRouteList[y][x][1]);

//      // todo ggf. in ram selbst regeln
//      if( (y == 0) && (x == 0) )
//      {
//        // ram hat langsameren clock
//        moduleList[y][x]->clk(clkSlow);
//      }
//      else
//      {
//        moduleList[y][x]->clk(clk);
//      }
      moduleList[y][x]->clk(clk);

      // start signal fuer gateway
      if( (y == (K_NOC_SIZE - 1)) && (x == (K_NOC_SIZE - 1)) )
      {
        ((gateway*)(moduleList[y][x]))->startIn(start);
      }

      if(y == 0)
      {
        if(x == 0)
        {
          // 1. Reihe, 1. Element (2)
          routerList[y][x]->routeOut[0](routeList[y][x][1]); // right
          routerList[y][x]->routeOut[1](routeList[y][x][2]); // down
          routerList[y][x]->routeIn[0](routeList[y][x + 1][3]); // right
          routerList[y][x]->routeIn[1](routeList[y + 1][x][0]); // down
        }
        else if(x == (K_NOC_SIZE - 1))
        {
          // 1. Reihe, letztes Element (2)
          routerList[y][x]->routeOut[0](routeList[y][x][2]); // down
          routerList[y][x]->routeOut[1](routeList[y][x][3]); // left
          routerList[y][x]->routeIn[0](routeList[y + 1][x][0]); // down
          routerList[y][x]->routeIn[1](routeList[y][x - 1][1]); // left
        }
        else
        {
          // 1. Reihe, mittleres Element (3)
          routerList[y][x]->routeOut[0](routeList[y][x][1]); // right
          routerList[y][x]->routeOut[1](routeList[y][x][2]); // down
          routerList[y][x]->routeOut[2](routeList[y][x][3]); // left
          routerList[y][x]->routeIn[0](routeList[y][x + 1][3]); // right
          routerList[y][x]->routeIn[1](routeList[y + 1][x][0]); // down
          routerList[y][x]->routeIn[2](routeList[y][x - 1][1]); // left
        }
      }
      else if(y == (K_NOC_SIZE - 1))
      {
        if(x == 0)
        {
          // Letzte Reihe, 1. Element (2)
          routerList[y][x]->routeOut[0](routeList[y][x][0]); // up
          routerList[y][x]->routeOut[1](routeList[y][x][1]); // right
          routerList[y][x]->routeIn[0](routeList[y - 1][x][2]); // up
          routerList[y][x]->routeIn[1](routeList[y][x + 1][3]); // right
        }
        else if(x == (K_NOC_SIZE - 1))
        {
          // Letzte Reihe, letztes Element (2)
          routerList[y][x]->routeOut[0](routeList[y][x][0]); // up
          routerList[y][x]->routeOut[1](routeList[y][x][3]); // left
          routerList[y][x]->routeIn[0](routeList[y - 1][x][2]); // up
          routerList[y][x]->routeIn[1](routeList[y][x - 1][1]); // left
        }
        else
        {
          // Letzte Reihe, mittleres Element (3)
          routerList[y][x]->routeOut[0](routeList[y][x][0]); // up
          routerList[y][x]->routeOut[1](routeList[y][x][1]); // right
          routerList[y][x]->routeOut[2](routeList[y][x][3]); // left
          routerList[y][x]->routeIn[0](routeList[y - 1][x][2]); // up
          routerList[y][x]->routeIn[1](routeList[y][x + 1][3]); // right
          routerList[y][x]->routeIn[2](routeList[y][x - 1][1]); // left
        }
      }
      else
      {
        if(x == 0)
        {
          // Mittlere Reihe, 1. Element (3)
          routerList[y][x]->routeOut[0](routeList[y][x][0]); // up
          routerList[y][x]->routeOut[1](routeList[y][x][1]); // right
          routerList[y][x]->routeOut[2](routeList[y][x][2]); // down
          routerList[y][x]->routeIn[0](routeList[y - 1][x][2]); // up
          routerList[y][x]->routeIn[1](routeList[y][x + 1][3]); // right
          routerList[y][x]->routeIn[2](routeList[y + 1][x][0]); // down
        }
        else if(x == (K_NOC_SIZE - 1))
        {
          // Mittlere Reihe, letztes Element (3)
          routerList[y][x]->routeOut[0](routeList[y][x][0]); // up
          routerList[y][x]->routeOut[1](routeList[y][x][2]); // down
          routerList[y][x]->routeOut[2](routeList[y][x][3]); // left
          routerList[y][x]->routeIn[0](routeList[y - 1][x][2]); // up
          routerList[y][x]->routeIn[1](routeList[y + 1][x][0]); // down
          routerList[y][x]->routeIn[2](routeList[y][x - 1][1]); // left
        }
        else
        {
          // Mittlere Reihe, mittleres Element (4)
          routerList[y][x]->routeOut[0](routeList[y][x][0]); // up
          routerList[y][x]->routeOut[1](routeList[y][x][1]); // right
          routerList[y][x]->routeOut[2](routeList[y][x][2]); // down
          routerList[y][x]->routeOut[3](routeList[y][x][3]); // left
          routerList[y][x]->routeIn[0](routeList[y - 1][x][2]); // up
          routerList[y][x]->routeIn[1](routeList[y][x + 1][3]); // right
          routerList[y][x]->routeIn[2](routeList[y + 1][x][0]); // down
          routerList[y][x]->routeIn[3](routeList[y][x - 1][1]); // left
        }
      }
      PRINT_DEBUG("  main - [" + to_string(y) + "," + to_string(x) + "] verbunden");
    }
  }
  PRINT_DEBUG("main - signale verbunden");

  // ------------------------------

  // ------------------------------
  // Starte Simulation
  // ------------------------------

  // Erstellen eine Impulsdiagrammes, dass mit dem Plugion GTK_wave betrachtet werden kann.
  sc_trace_file* Tf;
  Tf = sc_create_vcd_trace_file("waves");
  Tf->set_time_unit(1, SC_NS);
  sc_trace(Tf, clk, "clk");
  sc_trace(Tf, moduleRouteList[0][0][0].read().opcode, "ram - routerIn ");
  sc_trace(Tf, moduleRouteList[0][0][1].read().opcode, "ram - routerOut ");

  PRINT_DEBUG("main - simulation starten");
  sc_start();
  PRINT_DEBUG("main - simulation beendet");


  sc_close_vcd_trace_file(Tf);

  return 0;
}

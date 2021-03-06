/*
 * router.cpp
 *
 */

#include "router.h"
#include "paket.h"
#include "RAuES_U5.h"

router::router(sc_module_name name, uint8_t id, uint8_t routen,
    RoutingRichtung routeRichtungen[], uint32_t bufferSize) :
  sc_module(name), id(id), routen(routen), bufferSize(bufferSize)
{
  if(routen == 0)
  {
    // todo: ggf. andere Fehlerbehandlung oder Error throw
    throw "router::router(): routen == 0";
    //this->routen = 1;
  }

  if(bufferSize == 0)
  {
    // todo: ggf. andere Fehlerbehandlung oder Error throw
    throw "router::router(): bufferSize == 0";
    //this->bufferSize = this->routen;
  }

  this->routeIn = new sc_in<paket> [this->routen];
  this->routeOut = new sc_out<paket> [this->routen];
  this->routingTable = new RoutingRichtung[this->routen];

  this->sendeBuffer = new PaketBuffer*[this->routen];
  for(size_t i = 0; i < (size_t)this->routen; ++i)
  {
    this->routingTable[i] = routeRichtungen[i];
    this->sendeBuffer[i] = new PaketBuffer(this->bufferSize);
  }

  this->modulBuffer = new PaketBuffer(this->bufferSize);
  SC_HAS_PROCESS(router);

  SC_METHOD(receive);
  sensitive << clk.pos();

  SC_METHOD(send);
  sensitive << clk.pos();
}

void router::receive()
{
  // Pruefe ob ein Paket am Modul-Eingang anliegt (opcode 0x00 == leeres Paket)
  if(moduleIn.read().opcode != (uint8_t)0x00)
  {
    // Paket liegt an, lege es an die naechste frei Stelle des jeweiligen Sendebuffers
    // Falls der Buffer voll ist, wird das Paket verworfen
    PRINT_DEBUG("router - empfange modulpaket [" + to_string((uint8_t)(id & 0x0F)) + "," + to_string((uint8_t)((id >> 4) & 0x0F)) + "] mit opcode = " + to_string(moduleIn.read().opcode));
    route(moduleIn.read(), id);
  }

  // Pruefe jeden Eingang auf Pakete
  for(size_t i = 0; i < (size_t)routen; ++i)
  {
    // Pruefe ob ein Paket anliegt (opcode 0x00 == leeres Paket)
    if(routeIn[i].read().opcode != (uint8_t)0x00)
    {
      // Paket liegt an, lege es an die naechste frei Stelle des jeweiligen Sendebuffers
      // Falls der Buffer voll ist, wird das Paket verworfen
      PRINT_DEBUG("router - empfange routerpaket [" + to_string((uint8_t)(id & 0x0F)) + "," + to_string((uint8_t)((id >> 4) & 0x0F)) + "] mit opcode = " + to_string(routeIn[i].read().opcode));
      route(routeIn[i].read(), i);
    }
  }
}

void router::send()
{
  paket tmpPaket;
  paket leeresPaket;

  // Pruefe jeden SendeBuffer auf Pakete
  // Versuche ein Paket aus dem Modulbuffer zu holen
  if(modulBuffer->pop(tmpPaket))
  {
    // Paket aus dem Buffer in tmpPaket gespeichert, sende es
    moduleOut.write(tmpPaket);
    PRINT_DEBUG("router - sende modulpaket [" + to_string((uint8_t)(id & 0x0F)) + "," + to_string((uint8_t)((id >> 4) & 0x0F)) + "] mit opcode = " + to_string(tmpPaket.opcode));
    //    PRINT_DEBUG("  id = " + to_string(tmpPaket.id));
    //    PRINT_DEBUG("  op = " + to_string(tmpPaket.opcode));
    //    PRINT_DEBUG("  se = " + to_string(tmpPaket.sender));
    //    PRINT_DEBUG("  re = " + to_string(tmpPaket.receiver));
    //    PRINT_DEBUG("  xp = " + to_string(tmpPaket.xpos));
    //    PRINT_DEBUG("  yp = " + to_string(tmpPaket.ypos));
    //    PRINT_DEBUG("  cl = " + to_string(tmpPaket.color));
  }
  else
  {
    // kein Paket im Buffer, lege leeres Paket an den Ausgang
    moduleOut.write(leeresPaket);
  }

  // Pruefe jeden SendeBuffer auf Pakete
  for(size_t i = 0; i < (size_t)routen; ++i)
  {
    // Versuche ein Paket aus dem Sendebuffer zu holen
    if(sendeBuffer[i]->pop(tmpPaket))
    {
      // Paket aus dem Buffer in tmpPaket gespeichert, sende es
      routeOut[i].write(tmpPaket);
      PRINT_DEBUG("router - sende routerpaket [" + to_string((uint8_t)(id & 0x0F)) + "," + to_string((uint8_t)((id >> 4) & 0x0F)) + "] mit opcode = " + to_string(tmpPaket.opcode));
    }
    else
    {
      // kein Paket im Buffer, lege leeres Paket an den Ausgang
      routeOut[i].write(leeresPaket);
    }
  }
}

bool router::route(const paket &pkg, uint8_t quelleId)
{
  bool allesOk = false;
  RoutingRichtung paketRichtung; // Liste mit möglichen Richtungen fuer das Paket
  uint8_t zielId[2];
  uint8_t routerId[2];
  int16_t richtungsVektor[2];

  if(pkg.opcode != (uint8_t)0x00)
  {
    routerId[0] = (id & (uint8_t)0x0F); // y zeile
    routerId[1] = ((id >> 4) & (uint8_t)0x0F); // x spalte

    // Pruefe wohin das Paket geschickt werden soll
    if(pkg.receiver == id)
    {
      // Am Router angeschlossenes Modul ist Ziel, schreibe Modulbuffer
      // Paket wird verworfen wenn buffer voll.
      modulBuffer->push(pkg);
    }
    else
    {
      // An anderen Router, pruefe whoin genau
      zielId[0] = (pkg.receiver & (uint8_t)0x0F); // y zeile
      zielId[1] = ((pkg.receiver >> 4) & (uint8_t)0x0F); // x spalte

      richtungsVektor[0] = (int16_t)zielId[0] - (int16_t)routerId[0];
      richtungsVektor[1] = (int16_t)zielId[1] - (int16_t)routerId[1];

      if((richtungsVektor[0] > 0) && (richtungsVektor[1] > 0))
      {
        // Down Right
        paketRichtung = RoutingRichtung::DOWN;
      }
      else if((richtungsVektor[0] > 0) && (richtungsVektor[1] < 0))
      {
        // Down Left
        paketRichtung = RoutingRichtung::DOWN;
      }
      else if((richtungsVektor[0] > 0) && (richtungsVektor[1] == 0))
      {
        // Down
        paketRichtung = RoutingRichtung::DOWN;
      }
      else if((richtungsVektor[0] < 0) && (richtungsVektor[1] > 0))
      {
        // Up Right
        paketRichtung = RoutingRichtung::UP;
      }
      else if((richtungsVektor[0] < 0) && (richtungsVektor[1] < 0))
      {
        // Up Left
        paketRichtung = RoutingRichtung::UP;
      }
      else if((richtungsVektor[0] < 0) && (richtungsVektor[1] == 0))
      {
        // Up
        paketRichtung = RoutingRichtung::UP;
      }
      else if((richtungsVektor[0] == 0) && (richtungsVektor[1] > 0))
      {
        // Right
        paketRichtung = RoutingRichtung::RIGTH;
      }
      else if((richtungsVektor[0] == 0) && (richtungsVektor[1] < 0))
      {
        // Left
        paketRichtung = RoutingRichtung::LEFT;
      }
      else
      {
        // keine Richtung, oben bereits abgefangen
      }

      // Durchsuche routingtabelle nach gueltiger route
      // wenn keine route gefunden => droppe paket
      for(size_t i = 0; i < (size_t)routen; ++i)
      {
        if(paketRichtung == routingTable[i])
        {
          sendeBuffer[i]->push(pkg);
        }
      }
    }

  }

  // todo: ggf, wenn buffer voll auf anderen knoten routen
  // daher auch quelleId, um zu vermeiden, dass es an den gleichen router erneut gesendet wird.
  // ggf. alternativ anhand der richtung des empfängers entscheiden ob richtung valid

  return allesOk;
}

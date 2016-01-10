/*
 * router.cpp
 *
 */

#include "router.h"
#include "paket.h"

router::router(sc_module_name name, uint8_t id, uint8_t routen,
    uint32_t bufferSize) :
    sc_module(name), id(id), routen(routen), bufferSize(bufferSize)
{
  if(routen == 0)
  {
    // todo: ggf. andere Fehlerbehandlung oder Error throw
    this->routen = 1;
  }

  if(bufferSize == 0)
  {
    // todo: ggf. andere Fehlerbehandlung oder Error throw
    this->bufferSize = this->routen;
  }

  this->routeIn = new sc_in<paket> [this->routen];
  this->routeOut = new sc_out<paket> [this->routen];
  this->routingTable = new RoutingRichtung[this->routen];

  this->sendeBuffer = new PaketBuffer*[this->routen];
  for(size_t i = 0; i < (size_t)this->routen; ++i)
  {
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
    routerId[0] = (id & (uint8_t)0x0F);
    routerId[1] = ((id >> 4) & (uint8_t)0x0F);

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
      zielId[0] = (pkg.receiver & (uint8_t)0x0F);
      zielId[1] = ((pkg.receiver >> 4) & (uint8_t)0x0F);

      richtungsVektor[0] = (int16_t)zielId[0] - (int16_t)routerId[0];
      richtungsVektor[1] = (int16_t)zielId[1] - (int16_t)routerId[1];

      if((richtungsVektor[0] > 0) && (richtungsVektor[1] > 0))
      {
        // Right Up
      }
      else if((richtungsVektor[0] > 0) && (richtungsVektor[1] < 0))
      {
        // Right Down
      }
      else if((richtungsVektor[0] > 0) && (richtungsVektor[1] == 0))
      {
        // Right
      }
      else if((richtungsVektor[0] < 0) && (richtungsVektor[1] > 0))
      {
        // Left Up
      }
      else if((richtungsVektor[0] < 0) && (richtungsVektor[1] < 0))
      {
        // Left Down
      }
      else if((richtungsVektor[0] < 0) && (richtungsVektor[1] == 0))
      {
        // Left
      }
      else if((richtungsVektor[0] == 0) && (richtungsVektor[1] > 0))
      {
        // Up
      }
      else if((richtungsVektor[0] == 0) && (richtungsVektor[1] < 0))
      {
        // Down
      }
      else
      {
        // keine Richtung, oben bereits abgefangen
      }

      //todo
    }

  }

  // todo: ggf, wenn buffer voll auf anderen knoten routen
  // daher auch quelleId, um zu vermeiden, dass es an den gleichen router erneut gesendet wird.
  // ggf. alternativ anhand der richtung des empfängers entscheiden ob richtung valid

  //sendeBuffer[i]->push(routeIn[i].read());

  return allesOk;
}

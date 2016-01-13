/*
 * module.cpp
 *
 */

#include "module.h"

module::module(sc_module_name name, uint8_t id, uint32_t bufferSize) :
    sc_module(name), id(id), bufferSize(bufferSize)
{
  if(bufferSize == 0)
  {
    // todo: ggf. andere Fehlerbehandlung oder Error throw
    this->bufferSize = 5;
  }

  sendeBuffer = new PaketBuffer(this->bufferSize);

  SC_HAS_PROCESS(module);

  SC_METHOD(receive);
  sensitive << clk.pos();

  SC_METHOD(send);
  sensitive << clk.pos();
}

void module::receive()
{
  // Pruefe ob ein Paket am Eingang anliegt (opcode 0x00 == leeres Paket)
  if(routerIn.read().opcode != (uint8_t)K_OP_LEER)
  {
    // Paket liegt an, pruefe ob Empfangsbestaetigung
    if(routerIn.read().opcode == (uint8_t)K_OP_ACK)
    {
      // Empfangsbestaetigung
      // todo erstmal nicht implementiert
    }
    else
    {
      // Normales Paket, verarbeite es in process und sende empfangsbestaetigung
      paket tmpPkg = routerIn.read();
      // todo: erstmal kein ack
      //createAck(routerIn.read());
      if( process(tmpPkg) )
      {
        sendeBuffer->push( tmpPkg );
      }
    }
  }
}

void module::send()
{
  paket tmpPaket;
  paket leeresPaket;

  // Pruefe den SendeBuffer auf Pakete
  if(sendeBuffer->pop(tmpPaket))
  {
    // Paket aus dem Buffer in tmpPaket gespeichert, sende es
    routerOut.write(tmpPaket);
  }
  else
  {
    // kein Paket im Buffer, lege leeres Paket an den Ausgang
    routerOut.write(leeresPaket);
  }
}

void module::createAck(paket pkg)
{
  uint8_t tmp;

  tmp = pkg.receiver;
  pkg.receiver = pkg.sender;
  pkg.sender = tmp;
  pkg.opcode = (uint8_t)K_OP_ACK;

  sendeBuffer->push( pkg );
}


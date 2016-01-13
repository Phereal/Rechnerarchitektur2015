/*
 * gateway.cpp
 *
 *
 */

#include "gateway.h"

#include "paket.h"
#include "RAuES_U5.h"

gateway::gateway(sc_module_name name, uint8_t id, uint32_t bufferSize,
    uint8_t ramId, uint8_t computeList[], uint32_t computeCount,
    uint32_t pixelBufferSize) :
  module(name, id, bufferSize), ramId(ramId), state(Zustaende::IDLE),
      computeCount(computeCount), computeBusyCnt(0),
      pixelBufferSize(pixelBufferSize)
{
  PRINT_DEBUG("gateway - Konstruktor");

  if(computeCount == 0)
  {
    // todo error handling
    throw "gateway::gateway(): computeCount == 0";
  }

  if(pixelBufferSize == 0)
  {
    // todo error handling
    throw "gateway::gateway(): pixelBufferSize == 0";
    //this->pixelBufferSize = 10;
  }

  this->computeList = new ComputeList_T[this->computeCount];
  for(size_t i = 0; i < (size_t)this->computeCount; ++i)
  {
    this->computeList[i].id = computeList[i];
    this->computeList[i].busy = false;
  }

  this->pixelBuffer = new PaketBuffer(this->pixelBufferSize);
  SC_HAS_PROCESS(gateway);

  SC_METHOD(checkStart);
  sensitive << clk.pos();
  PRINT_DEBUG("gateway - Konstruktor ende");
}

void gateway::checkStart()
{
  // nur starten wenn es nicht bereits gestartet ist
  if(state == Zustaende::IDLE)
  {
    // Pruefe ob das startsignal aktiviert ist
    if(startIn.read())
    {
      // starte Berechnung
      paket pkg(K_OP_RFI, id, ramId, 0, 0, 0);
      sendeBuffer->push(pkg);
      state = Zustaende::WAIT_RAM_RRF;
      PRINT_DEBUG("gateway - Start");
    }
  }
}

bool gateway::process(paket &pkg)
{
  bool processOk = false;

  if(pkg.opcode != K_OP_LEER)
  {

    switch(state)
    {
      case Zustaende::IDLE:
        // sollte nicht passieren
        throw "gateway::process(): case Zustaende::IDLE";
        break;

      case Zustaende::WAIT_RAM_RRF:
        if((pkg.opcode == K_OP_RFF) && (pkg.sender == ramId))
        {
          pkg = paket(K_OP_NXT, id, ramId, 0, 0, 0);
          state = Zustaende::PROCESSING;
          processOk = true;
          PRINT_DEBUG("gateway - RFF erhalten");
        }
        else
        {
          // sollte nicht passieren
          throw "gateway::process(): case Zustaende::WAIT_RAM_RRF";
        }
        break;

      case Zustaende::PROCESSING:
        if((pkg.opcode == K_OP_NXA) && (pkg.sender == ramId))
        {
          PRINT_DEBUG("gateway - NXA erhalten");
          if(!pixelBuffer->full())
          {
            pixelBuffer->push(pkg);
            // kein setzen von processOk, sendeBuffer wird intern verwaltet
          }
          else
          {
            // todo errorhandling
            throw "gateway::process(): case Zustaende::PROCESSING, opcode == K_OP_NXA";
          }

          // sende weitere pixelanfragen an den ram (sofern noch platz im buffer ist)
          if(!pixelBuffer->full())
          {
            pkg = paket(K_OP_NXT, id, ramId, 0, 0, 0);
            if(!sendeBuffer->push(pkg))
            {
              // todo error
              throw "gateway::process(): case Zustaende::PROCESSING, opcode == K_OP_NXA, sendeBuffer voll 1";
            }
          }

          // sende alle verfuegbaren pixel an idle compute-module
          while((!pixelBuffer->empty()) && (!computeListAllBusy()))
          {
            pixelBuffer->pop(pkg);
            pkg = paket(K_OP_EXE, id, getAndSetNextIdleCompute(), pkg.xpos,
                pkg.ypos, pkg.color); // neues paket, damit id automatisch angelegt wird
            if(!sendeBuffer->push(pkg))
            {
              // todo error (z.B. throw, oder pkg zurueckschreiben und break
              throw "gateway::process(): case Zustaende::PROCESSING, opcode == K_OP_NXA, sendeBuffer voll 2";
            }
          }
        }
        else if(pkg.opcode == K_OP_FIN)
        {
          PRINT_DEBUG("gateway - FIN erhalten");
          setComputeIdle(pkg.sender);

          // sende weitere pixelanfragen an den ram (sofern noch platz im buffer ist)
          if(!pixelBuffer->full())
          {
            pkg = paket(K_OP_NXT, id, ramId, 0, 0, 0);
            if(!sendeBuffer->push(pkg))
            {
              // todo error
              throw "gateway::process(): case Zustaende::PROCESSING, opcode == K_OP_FIN, sendeBuffer voll 1";
            }
          }

          // sende alle verfuegbaren pixel an idle compute-module
          while((!pixelBuffer->empty()) && (!computeListAllBusy()))
          {
            pixelBuffer->pop(pkg);
            pkg = paket(K_OP_EXE, id, getAndSetNextIdleCompute(), pkg.xpos,
                pkg.ypos, pkg.color); // neues paket, damit id automatisch angelegt wird
            if(!sendeBuffer->push(pkg))
            {
              // todo error (z.B. throw, oder pkg zurueckschreiben und break
              throw "gateway::process(): case Zustaende::PROCESSING, opcode == K_OP_FIN, sendeBuffer voll 2";
            }
          }
        }
        else if((pkg.opcode == K_OP_END) && (pkg.sender == ramId))
        {
          PRINT_DEBUG("gateway - END erhalten");
          //ggf. entfernen, da ram die info zuerst bekommen haben kann
          if(pixelBuffer->empty() && computeListAllIdle())
          {
            pkg = paket(K_OP_WFI, id, ramId, 0, 0, 0);
            state = Zustaende::WAIT_RAM_WFF;
            processOk = true;
          }
          else
          {
            // todo error handling
            throw "gateway::process(): case Zustaende::PROCESSING, opcode == K_OP_END";
          }
        }
        else
        {
          // sollte nicht passieren
          throw "gateway::process(): case Zustaende::PROCESSING, opcode == else";
        }
        break;

      case Zustaende::WAIT_RAM_WFF:
        if((pkg.opcode == K_OP_WFF) && (pkg.sender == ramId))
        {
          PRINT_DEBUG("gateway - Beende Process");
          state = Zustaende::IDLE;
          sc_stop();
          // kein setzen von processOk, da kein paketsenden noetig
        }
        else
        {
          // sollte nicht passieren
          throw "gateway::process(): case Zustaende::WAIT_RAM_WFF";
        }
        break;

      default:
        // sollte nicht passieren
        throw "gateway::process(): case default";
        break;
    }
  }

  return processOk;
}

bool gateway::computeListAllBusy()
{
  bool allBusy = false;

  if(computeBusyCnt == computeCount)
  {
    allBusy = true;
  }

  return allBusy;
}

bool gateway::computeListAllIdle()
{
  bool allIdle = false;

  if(computeBusyCnt == 0)
  {
    allIdle = true;
  }

  return allIdle;
}

uint8_t gateway::getAndSetNextIdleCompute()
{
  uint8_t id = 0;

  for(size_t i = 0; i < computeCount; ++i)
  {
    if(!computeList[i].busy)
    {
      id = computeList[i].id;
      computeList[i].busy = true;
      computeBusyCnt++;
    }
  }

  return id;
}

void gateway::setComputeIdle(uint8_t id)
{
  for(size_t i = 0; i < computeCount; ++i)
  {
    if(computeList[i].id == id)
    {
      computeList[i].busy = false;
      computeBusyCnt--;
    }
  }
}

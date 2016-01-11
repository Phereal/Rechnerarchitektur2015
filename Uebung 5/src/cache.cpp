/*
 * cache.cpp
 *
 * erbt von module.cpp
 */

#include "cache.h"

using namespace std;

cache::cache(sc_module_name name, uint8_t id, uint32_t bufferSize) : module(name, id, bufferSize)
{
  if(initialize == false) {
    init();
    initialize = true;
  }
  pakethandler();
  sensitive << clk.pos();
}
//Überschreiben der process-Methode der Elternklasse Module
bool cache::process(paket &pkg){
  enable = false;
  //Paket einlesen
  i_id = pkg.id;
  i_opcode = pkg.opcode;
  i_sender = pkg.sender;
  i_receiver = pkg.receiver;
  i_xpos = pkg.xpos;
  i_ypos = pkg.ypos;
  i_color = pkg.color;

  //Paket senden
  pkg.id = o_id;
  pkg.opcode = o_opcode;
  pkg.sender = o_sender;
  pkg.receiver = o_receiver;
  pkg.xpos = o_xpos;
  pkg.ypos = o_ypos;
  pkg.color = o_color;
        return enable;
}

void cache::pakethandler() {
  //Es werden nur OPCodes behandelt, die auch durch das RAM-Modul verarbeitet werden können.
  // TODO die entsprechenden cases für cache füllen.
  switch(i_opcode){
    case 0x00: break;//[emp]
    case 0x01: break;//[exe]
    case 0x02: break;//[fin]
    case 0x03: break;//[c_req]
    case 0x04: break;//[r_req]
    case 0x05: break;//[ack]
    case 0x06: break;//[ic_pay]
    case 0x07: break;//[ir_pay]
    case 0x08: break;//[o_pay]
    case 0x09: break;//[rfi]
    case 0x0A: break;//[rff]
    case 0x0B: break;//[wfi]
    case 0x0C: break;//[wff]
    case 0x0D: break;//[nxt]
    case 0x0E: break;//[nxa]
    default: break;
  }
}

// Initialisierung der FIFO-Queue
void cache::init(){
  while(cache_list.size() != 0){
    cache_list.pop();
  }
}

/*
 * cache.cpp
 *
 * erbt von module.cpp
 *
 * Der Cache hat die Aufgabe Pixel lokal abzuspeichern und diese den Compute-Modulen zur Verfügung zu stellen.
 * Dabei speichert er die bekannten Pixel in einer FIFO-Queue. Die FIFO-Queue wird mit pusch() und pop()
 * Befehlen bedient. Dabei besteht sie aus einem selbst erstellten Datentyp namens Pixel,
 * der die X- bzw. Y-Position und den Color Wert speichern kann.
 *
 * Wenn eine Anfrage eines Compute-Moduls eingeht, wird die gesamte Queue nach den entsprechenden
 * X-Pos bzw. Y-Pos durchsucht und sollte dieser Pixel gefunden werden, wird er dem Compute-Modul
 * zugestellt. Wenn der Pixel nicht gefunden wird, stellt das Cache-Modul eine Anfrage an
 * das RAM-Modul um den Pixel zu beziehen.
 *
 * Das dieses einholen eines Pixels aus dem RAM sehr lange dauern kann, muss die Anfrage des
 * Compute-Moduls in einer REQUEST-Queue zwischengespeichert werden. Diese Reqeust Queue
 * wird bei jedem Takt durchlaufen und [jetzt] bekannte Pixel werden den wartenden
 * zugestellt. Anschließend wird der Eintrag aus der Queue entfernt.
 *
 * Wenn eine angefragte Information aus dem RAM eintrifft, wird sie direkt in die FIFO-Qeueu
 * geschrieben.
 *
 *
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
  //Es werden nur OPCodes behandelt, die auch durch das Cache-Modul verarbeitet werden können.
  switch(i_opcode){
    case 0x00: break;//[emp] -- leeres Paket (/)
    case 0x01: break;//[exe] -- Berechnungsauftrag (Gateway|Compute)
    case 0x02: break;//[fin] -- Berechnung abgeschlossen (Compute|Gateway)
    case 0x03: //[c_req] -- Pixel Anfrage an den Cache (Compute|Cache)
      //Überprüfe ob der Pixel sich im Cache befindet
      //Wenn ja, füge die Anfrage direkt in die Request Queue ein.
      //Wenn nein, füge  die Anfrage direkt in die Request Queue ein und sende eine Anfrage an den RAM
      if(checkPixelIsInCache(i_xpos, i_ypos)){
        addRequestToQueue(i_id, i_sender, i_receiver, i_xpos, i_ypos);
      }
      else{
        getPixelFromRAM(i_xpos, i_ypos);
        addRequestToQueue(i_id, i_sender, i_receiver, i_xpos, i_ypos);
      }
      break;
    case 0x04: break;//[r_req] -- Pixel Anfrage an den RAM (Cache|RAM)
    case 0x05: break;//[ack] -- Empfangsbestätigung (/)
    case 0x06: break;//[ic_pay] -- Pixel vom Cache an Compute (Cache|Compute)
    case 0x07: //[ir_pay] -- Pixel vom RAM an Cache (RAM|Cache)
      //Füge den erhaltenen Pixel der FIFO-Queue hinzu
      writePixelToCache(i_xpos, i_ypos, i_color);
      break;
    case 0x08: break;//[o_pay] -- Berechneter Pixel an den RAM (Compute|RAM)
    case 0x09: break;//[rfi] -- Bild einzulesen (Gateway|RAM)
    case 0x0A: break;//[rff] -- Bild fertig eingelesen (RAM|Gateway)
    case 0x0B: break;//[wfi] -- Zielbild schreiben (Gateway|RAM)
    case 0x0C: break;//[wff] -- Zielbild schreiben abgeschlossen (RAM|Gateway)
    case 0x0D: break;//[nxt] -- Sende nächsten zu berechnenden Pixel (Gateway|RAM)
    case 0x0E: break;//[nxa] -- Nächster zu berechnender Pixel (RAM|Gateway)
    default: break;
  }
}

// Initialisierung der FIFO-Queues
void cache::init(){
  while(cache_list.size() != 0){
    cache_list.pop();
  }
  while(request_list.size() != 0){
    request_list.pop();
  }
}

void cache::addRequestToQueue(unsigned int id, unsigned int sender, unsigned int receiver, unsigned int xpos, unsigned int ypos){
  request tmp = {id,sender,receiver,xpos,ypos};
  request_list.push(tmp);
}

void cache::deleteRequestFromQueue(unsigned int id, unsigned int sender, unsigned int receiver, unsigned int xpos, unsigned int ypos){
  std::queue<request> tmp_request_list;
  //Durchlaufe die Request List und suche nach dem zu löschenden Element
  while(request_list.size() != 0){
    request tmpRequest = request_list.front();
    if(tmpRequest.id == id && tmpRequest.sender == sender && tmpRequest.receiver == receiver && tmpRequest.xpos == xpos && tmpRequest.ypos == ypos){
      //Tue nichts, also schreibe füge den Request nicht der tmp_request_list wieder hinzu!
      request_list.pop();
    }
    else {
      tmp_request_list.push(tmpRequest);
      request_list.pop();
    }
  }
  request_list = tmp_request_list;
}

bool cache::checkPixelIsInCache(unsigned int xpos, unsigned int ypos){
  bool isInCache = false;
  std::queue<pixel> tmp_cache_list;
  //Durchlaufe den Cache und durchsuche nach dem richtigen Pixel
  while(cache_list.size() != 0){
    pixel tmpPixel = cache_list.front();
    if(tmpPixel.xpos == xpos && tmpPixel.ypos == ypos){
      isInCache = true;
    }
    tmp_cache_list.push(tmpPixel);
    cache_list.pop();
  }
  cache_list = tmp_cache_list;
  return isInCache;
}

void cache::getPixelFromRAM(unsigned int xpos, unsigned int ypos){

}

void cache::writePixelToCache(unsigned int xpos, unsigned int ypos, unsigned char color){
  if(cache_list.size() == CACHESIZE) {
    cache_list.pop();
  }
  pixel tmp = {ypos, ypos, color};
  cache_list.push(tmp);
}

unsigned char cache::readPixelFromCache(unsigned int xpos, unsigned int ypos){
  unsigned char color = 0;
  std::queue<pixel> tmp_cache_list;
  //Durchlaufe den Cache und durchsuche nach dem richtigen Pixel
  while(cache_list.size() != 0){
    pixel tmpPixel = cache_list.front();
    if(tmpPixel.xpos == xpos && tmpPixel.ypos == ypos){
      color = tmpPixel.color;
    }
    tmp_cache_list.push(tmpPixel);
    cache_list.pop();
  }
  cache_list = tmp_cache_list;
  return color;
}


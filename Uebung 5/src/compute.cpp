/*
 * compute.cpp
 *
 * erbt von module.cpp
 *
 *
 */

#include "compute.h"

#include <systemc.h>

compute::compute(sc_module_name name, uint8_t id, uint32_t bufferSize,
    uint8_t ramId, uint8_t gwId, uint8_t cacheList[], uint32_t cacheCount) :
  module(name, id, bufferSize), ramId(ramId), gwId(gwId),
      cacheCount(cacheCount)
{

  if(cacheCount == 0)
  {
    // todo error handling
    throw "compute::compute(): cacheCount == 0";
  }

  this->cacheList = new uint8_t[this->cacheCount];
  for(size_t i = 0; i < (size_t)this->cacheCount; ++i)
  {
    this->cacheList[i] = cacheList[i];
  }

  if(initialize == false)
  {
    init();
    initialize = true;
  }
}

//Überschreiben der process-Methode der Elternklasse Module
bool compute::process(paket &pkg)
{
  enable = false;
  //Paket einlesen
  i_id = pkg.id;
  i_opcode = pkg.opcode;
  i_sender = pkg.sender;
  i_receiver = pkg.receiver;
  i_xpos = pkg.xpos;
  i_ypos = pkg.ypos;
  i_color = pkg.color;

  pakethandler();
  taskHandler();

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

void compute::pakethandler()
{
  //Es werden nur OPCodes behandelt, die auch durch das compute-Modul verarbeitet werden können.
  switch(i_opcode)
  {
    case 0x00:
      break; //[emp] -- leeres Paket (/)
    case 0x01: //[exe] -- Berechnungsauftrag (Gateway|Compute)
      taskReceived = true;
      xpos[4] = i_xpos;
      ypos[4] = i_ypos;
      break;
    case 0x02:
      break; //[fin] -- Berechnung abgeschlossen (Compute|Gateway)
    case 0x03:
      break; //[c_req] -- Pixel Anfrage an den Cache (Compute|Cache)
    case 0x04:
      break; //[r_req] -- Pixel Anfrage an den RAM (Cache|RAM)
    case 0x05:
      break; //[ack] -- Empfangsbestätigung (/)
    case 0x06: //[ic_pay] -- Pixel vom Cache an Compute (Cache|Compute)
      break;
    case 0x07:
      break; //[ir_pay] -- Pixel vom RAM an Cache (RAM|Cache)
    case 0x08:
      break; //[o_pay] -- Berechneter Pixel an den RAM (Compute|RAM)
    case 0x09:
      break; //[rfi] -- Bild einzulesen (Gateway|RAM)
    case 0x0A:
      break; //[rff] -- Bild fertig eingelesen (RAM|Gateway)
    case 0x0B:
      break; //[wfi] -- Zielbild schreiben (Gateway|RAM)
    case 0x0C:
      break; //[wff] -- Zielbild schreiben abgeschlossen (RAM|Gateway)
    case 0x0D:
      break; //[nxt] -- Sende nächsten zu berechnenden Pixel (Gateway|RAM)
    case 0x0E:
      break; //[nxa] -- Nächster zu berechnender Pixel (RAM|Gateway)
    case 0x10:
      break; //[brd] -- Sende Bildgrenzen (Compute|RAM)
    case 0x11: //[brr] -- Empfange Bildgrenzen (RAM|Compute)
      receiveBorders();
      break;
    default:
      break;
  };
}

// Initialisierung
void compute::init()
{
  for(int i = 0; i < 5; ++i)
  {
    xpos[i] = 0;
    ypos[i] = 0;
    color[i] = 0;
  }
  bordersRequested = false;
  bordersReceived = false;
  neighboursCalculated = false;
  pixelRequested = false;
  taskReceived = false;
}

void compute::calcNeighbours()
{
  // Berechne die X-Nachbarn
  xpos[1] = i_xpos;
  xpos[4] = i_xpos;
  xpos[7] = i_xpos;
  if(i_xpos - 1 >= 0)
  {
    xpos[0] = i_xpos - 1;
    xpos[3] = i_xpos - 1;
    xpos[6] = i_xpos - 1;
  }
  else
  {
    xpos[0] = 0;
    xpos[3] = 0;
    xpos[6] = 0;
  }
  if(i_xpos + 1 <= width)
  {
    xpos[2] = i_xpos + 1;
    xpos[5] = i_xpos + 1;
    xpos[8] = i_xpos + 1;
  }
  else
  {
    xpos[2] = width;
    xpos[5] = width;
    xpos[8] = width;
  }

  // Berechne die Y-Nachbarn
  ypos[3] = i_ypos;
  ypos[4] = i_ypos;
  ypos[5] = i_ypos;
  if(i_ypos - 1 >= 0)
  {
    ypos[0] = i_ypos - 1;
    ypos[1] = i_ypos - 1;
    ypos[2] = i_ypos - 1;
  }
  else
  {
    ypos[0] = 0;
    ypos[1] = 0;
    ypos[2] = 0;
  }
  if(i_ypos + 1 <= height)
  {
    ypos[6] = i_ypos + 1;
    ypos[7] = i_ypos + 1;
    ypos[8] = i_ypos + 1;
  }
  else
  {
    ypos[6] = height;
    ypos[7] = height;
    ypos[8] = height;
  }
}

void compute::taskHandler()
{
  if(!taskReceived)
  {
    if(!bordersRequested)
    {
      getBorders();
    }
    if(!bordersReceived)
    {
      //tue nichts, denn solange die Bildgrenzen nicht erhalten sind können nie Nachbarpixel nicht berechnet werden!
    }
    else
    {
      //Bildgrenzen erhalten, Nachbarpixel können berechnet werden!
      if(!neighboursCalculated)
      {
        calcNeighbours();
      }
      //Wenn die Nachbarpixel berechnet wurden, können die umliegenden Pixel aus dem Cache angefragt werden!
      if(!pixelRequested)
      {
        requestPixel();
      }
      if(!pixelReceived){

      }
    }
  }
}

void compute::requestPixel()
{
  //Pixel 0
  paket pkg0(0x03, id, getMinCacheId(), xpos[0], ypos[0], 0);
  sendeBuffer->push(pkg0);

  //Pixel 1
  paket pkg1(0x03, id, getMinCacheId(), xpos[1], ypos[1], 0);
  sendeBuffer->push(pkg1);
  //Pixel 2
  paket pkg2(0x03, id, getMinCacheId(), xpos[2], ypos[2], 0);
  sendeBuffer->push(pkg2);

  //Pixel 3
  paket pkg3(0x03, id, getMinCacheId(), xpos[3], ypos[3], 0);
  sendeBuffer->push(pkg3);

  //Pixel 4
  paket pkg4(0x03, id, getMinCacheId(), xpos[4], ypos[4], 0);
  sendeBuffer->push(pkg4);

  //Pixel 5
  paket pkg5(0x03, id, getMinCacheId(), xpos[5], ypos[5], 0);
  sendeBuffer->push(pkg5);

  //Pixel 6
  paket pkg6(0x03, id, getMinCacheId(), xpos[6], ypos[6], 0);
  sendeBuffer->push(pkg6);

  //Pixel 7
  paket pkg7(0x03, id, getMinCacheId(), xpos[7], ypos[7], 0);
  sendeBuffer->push(pkg7);

  //Pixel 8
  paket pkg8(0x03, id, getMinCacheId(), xpos[8], ypos[8], 0);
  sendeBuffer->push(pkg8);

  pixelRequested = true;
  enable = true;
}

void compute::getBorders()
{
  paket pkg(0x10, id, getMinCacheId(), 0, 0, 0);
  sendeBuffer->push(pkg);

  bordersRequested = true;
  enable = true;
}

void compute::receiveBorders()
{
  width = i_xpos;
  height = i_ypos;
  bordersReceived = true;
}

/*int compute::calcPixel(int matrix[MATRIX_SIZE][MATRIX_SIZE], int xCoord,
 int yCoord)
 {
 int sum = 0;

 for(int i = 1; i <= MATRIX_SIZE; i++)
 {
 for(int j = 1; j <= MATRIX_SIZE; j++)
 {
 int M = matrix[i - 1][j - 1]; //-1, da Arrays ab 0 zählen.

 //Da wir Integer teilen erhalten wir bei der folgenden Rechnung immer die
 //Werte der 9 Pixel, die den Quellpixel und die ihn umgebenden Pixel beschreiben.

 int currentTargetX = (xCoord + i - MATRIX_SIZE / 2);
 int currentTargetY = (yCoord + j - MATRIX_SIZE / 2);

 int currentPixel = getValueAt(currentTargetX, currentTargetY); //getValueAt() ist ein Platzhalter! Ersetzen.

 sum += M * currentPixel; //Führe die eigentliche Formel durch und addierte den Summen-Berechnungsschritt:

 //Bevor wir eine weitere Schleife erlauben, pausieren wir die Programmausführung so lange,
 //wie die Formelausrechnung auf der Hardware ungefähr dauern würde.

 //TODO wait-statement

 }
 }
 return std::min(std::max(sum, 0), 255); //Auf 0-255 auf / abrunden.
 }*/

uint8_t compute::getMinCacheId()
{
  uint8_t myId[2];
  uint8_t zielId[2];
  int16_t minEntfernung = 32767; // max wert
  uint8_t minCache = 0;

  myId[0] = (id & (uint8_t)0x0F); // y zeile
  myId[1] = ((id >> 4) & (uint8_t)0x0F); // x spalte

  for(size_t i = 0; i < cacheCount; ++i)
  {
    int16_t tmpEntfernung[2]; // max wert
    zielId[0] = (cacheList[i] & (uint8_t)0x0F); // y zeile
    zielId[1] = ((cacheList[i] >> 4) & (uint8_t)0x0F); // x spalte

    // entfernung bestimmen
    tmpEntfernung[0] = zielId[0] - myId[0];
    tmpEntfernung[1] = zielId[1] - myId[1];

    // betrag bestimmen
    if(tmpEntfernung[0] < 0)
    {
      tmpEntfernung[0] = tmpEntfernung[0] * (-1);
    }
    if(tmpEntfernung[1] < 0)
    {
      tmpEntfernung[1] = tmpEntfernung[1] * (-1);
    }

    // entfernung kleiner als bisheriger wert? Dann ersete Wert und id
    if((tmpEntfernung[0] + tmpEntfernung[1]) < (minEntfernung))
    {
      minEntfernung = tmpEntfernung[0] + tmpEntfernung[1];
      minCache = cacheList[i];
    }
  }

  // fehler wenn kein cache gefunden
  if(minCache == 0)
  {
    throw "compute::getMinCacheId(): minCache == 0";
  }

  return minCache;
}

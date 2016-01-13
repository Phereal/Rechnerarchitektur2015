/*
 * PaketBuffer.cpp
 *
 *  Created on: 10.01.2016
 *      Author: steffen
 */

#include "PaketBuffer.h"

PaketBuffer::PaketBuffer(uint32_t bufferSize) :
    bufferSize(bufferSize), readPointer(0), writePointer(0), freeCnt(bufferSize)
{
  if(bufferSize == 0)
  {
    // todo: ggf. andere Fehlerbehandlung
    this->bufferSize = 10;
    this->freeCnt = this->bufferSize;
  }

  this->buffer = new paket[this->bufferSize];
}



bool PaketBuffer::push(const paket& p)
{
  bool allesOk = false;

  if(freeCnt != 0)
  {
    buffer[writePointer] = p;
    writePointer++;
    freeCnt--;
    allesOk = true;
  }

  return allesOk;
}



bool PaketBuffer::pop(paket &p)
{
  bool allesOk = false;

  if(freeCnt != bufferSize)
  {
    p = buffer[readPointer];
    readPointer++;
    freeCnt++;
    allesOk = true;
  }

  return allesOk;
}



bool PaketBuffer::full()
{
  bool isFull = false;

  if( freeCnt == 0 )
  {
    isFull = true;
  }

  return isFull;
}



bool PaketBuffer::empty()
{
  bool isEmpty = false;

  if( freeCnt == bufferSize )
  {
    isEmpty = true;
  }

  return isEmpty;
}


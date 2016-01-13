/*
 * Buffer.h
 *
 *  Created on: 10.01.2016
 *      Author: steffen
 */

#ifndef PAKETBUFFER_H_
#define PAKETBUFFER_H_

#include "paket.h"

class PaketBuffer
{
  public:
    PaketBuffer(uint32_t bufferSize);
    bool push(const paket& p);
    bool pop(paket& p);
    bool full();
    bool empty();

  private:
    uint32_t bufferSize;
    uint32_t readPointer;
    uint32_t writePointer;
    uint32_t freeCnt;
    paket *buffer;
};

#endif /* PAKETBUFFER_H_ */

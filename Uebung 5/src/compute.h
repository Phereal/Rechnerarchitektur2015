/*
 * compute.h
 *
 *  Created on: 05.01.2016
 *      Author: stefan
 */

#ifndef COMPUTE_H_
#define COMPUTE_H_

#include <systemc.h>
#include <queue>
#include <iostream>

#include "RAuES_U5.h"
#include "module.h"
#include "paket.h"

class compute : public module
{
  public:
    compute(sc_module_name name, uint8_t id, uint32_t bufferSize,
        uint8_t ramId, uint8_t gwId, uint8_t cacheList[], uint32_t cacheCount);

  private:
    //Methoden
    void pakethandler();
    void init();

    void getBorders();
    void receiveBorders();
    void taskHandler();
    void requestPixel();
    void calcPixel();
    void calcNeighbours();
    void receivePixel();
    void sendResult();
    uint8_t getMinCacheId();

  private:
    //Variablen
    uint8_t ramId;
    uint8_t gwId;
    uint32_t cacheCount;
    uint8_t *cacheList;

    bool initialize = false;
    bool enable = false;
    bool bordersRequested = false;
    bool bordersReceived = false;
    bool neighboursCalculated = false;
    bool pixelRequested = false;
    bool taskReceived = false;
    bool pixelReceived[9] = {false, false, false, false, false, false, false,
        false, false};
    bool calcExecuted = false;
    bool pixelCalculated = false;
    bool resultSend = false;
    bool initExecuted = false;

    unsigned int width = 0;
    unsigned int height = 0;

    unsigned int xpos[8];
    unsigned int ypos[8];
    unsigned char color[8];

    //lokale Speicherung des eingehenden Paketes
    uint32_t i_id;
    uint8_t i_opcode;
    uint8_t i_sender;
    uint8_t i_receiver;

    uint32_t i_xpos;
    uint32_t i_ypos;
    uint8_t i_color;

    //lokale Speicherung des ausgehenden Paketes
    uint32_t o_id;
    uint8_t o_opcode;
    uint8_t o_sender;
    uint8_t o_receiver;

    uint32_t o_xpos;
    uint32_t o_ypos;
    uint8_t o_color;

    virtual bool process(paket &pkg);
};

#endif /* COMPUTE_H_ */

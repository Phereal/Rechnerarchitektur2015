/*
* compute.h
*
*  Created on: 05.01.2016
*      Author: stefan
*/

#ifndef COMPUTE_H_
#define COMPUTE_H_

#define MATRIX_SIZE 3

//Wir simulieren Rechenzeit durch wait-statements, die bestimmt viele Clock-Zyklen warten:
#define WAIT_CLOCKS_ADD 1
#define WAIT_CLOCKS_SUB 1
#define WAIT_CLOCKS_MUL 3
#define WAIT_CLOCKS_DIV 25

#include <systemc.h>
#include "module.h"
#include <queue>
#include <iostream>


class compute : public module
{
    compute(sc_module_name name, uint8_t id, uint32_t bufferSize);

    void pakethandler();
    void addRequestToQueue(unsigned int id, unsigned int sender, unsigned int receiver, unsigned int xpos, unsigned int ypos);
    void deleteRequestFromQueue(unsigned int id, unsigned int sender, unsigned int receiver, unsigned int xpos, unsigned int ypos);

    struct pixel { unsigned int xpos; unsigned int ypos; unsigned char color; };
    std::queue<pixel> cache_list;
    struct request {
        unsigned int id; unsigned int sender; unsigned int receiver;
        unsigned int xpos; unsigned int ypos;
    };
    std::queue<request> request_list;

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

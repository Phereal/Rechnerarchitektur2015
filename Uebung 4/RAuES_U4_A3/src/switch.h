/*
 * switch.h
 *
 *  Created on: 14.12.2015
 *      Author: steffen
 */

#ifndef SWITCH_H_
#define SWITCH_H_

// C/C++ Header
// SystemC Header
#include <systemc.h>
// Custom Header
#include "packet.h"

// Capital S since switch is a keyword
SC_MODULE(Switch)
{
    // inputs
    sc_in<bool> clk;
    sc_in<packet> *in;
    //outputs
    sc_out<packet> *out;

    // Constructor
    Switch(sc_module_name name, int n, int m, int bufferSize);

    void distribute();

    private:
    int n;
    int m;
    int bufferSize;
    packet *buffer;
};

#endif /* SWITCH_H_ */

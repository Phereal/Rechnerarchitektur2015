/*
 * consumer.h
 *
 *  Created on: 14.12.2015
 *      Author: steffen
 */

#ifndef CONSUMER_H_
#define CONSUMER_H_

// C/C++ Header
// SystemC Header
#include <systemc.h>
// Custom Header
#include "packet.h"


SC_MODULE(consumer)
{
    sc_in<bool> clk;
    sc_in<packet> in;

    // Constructor
    SC_CTOR(consumer)
    {
      SC_METHOD(receive);
      sensitive << clk;
    }

    void receive()
    {
      cout << "[" << name() << "] " << in << "\n";
    }
};

#endif /* CONSUMER_H_ */

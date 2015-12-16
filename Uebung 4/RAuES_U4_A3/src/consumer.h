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

    sc_out<bool> busy;
    //    sc_in<bool> dataPending;
    //    sc_out<bool> dataCleared;

    // Constructor
    SC_CTOR(consumer)
	{
	SC_METHOD(receive);
	sensitive << clk.pos();

	//      SC_METHOD(tellCleared);
	//      sensitive << in;
	}

    void receive()
	{
	cout << "[" << name() << "] " << in << "\n";
	busy = false;
	//      if(dataPending)
	//      {
	//        cout << "[" << name() << "] " << in << "\n";
	//        cleared = true;
	//      }
	}

    //    void tellCleared()
    //    {
    //      cleared = false;
    //
    //    }


private:
    bool cleared = true;
    };

#endif /* CONSUMER_H_ */

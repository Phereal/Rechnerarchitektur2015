/*
 * Producer.h
 *
 *  Created on: 14.12.2015
 *      Author: steffen
 */

#ifndef PRODUCER_H_
#define PRODUCER_H_

// C/C++ Header
#include <climits> /* max int */
// SystemC Header
#include <systemc.h>
// Custom Header
#include "packet.h"

SC_MODULE(producer)
{
    sc_in<bool> clk;
    //sc_in<bool> dataReceived;

    sc_out<packet> out;
    sc_out<bool> dataPending;

    // Constructor
    producer(sc_module_name name, int genSpeed, int maxAddress);

    void sendPackageRandomly();
//    void ackDataReceived();

  private:
    int genSpeed;
    int maxAddressCnt;

    bool getRandomBool() const;
    int getRandomAddress() const;
    int getRandomDatum() const;
};

#endif /* PRODUCER_H_ */

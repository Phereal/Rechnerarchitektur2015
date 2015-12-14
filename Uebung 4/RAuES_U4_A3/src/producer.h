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
    sc_out<packet> out;

    // Constructor
    producer(sc_module_name name, int genSpeed);

    void sendPackageRandomly();

  private:
    const static int K_MAX_ADDRESS = std::numeric_limits<int>::max();
    int genSpeed;

    bool getRandomBool() const;
    int getRandomAddress() const;
    int getRandomDatum() const;
};

#endif /* PRODUCER_H_ */

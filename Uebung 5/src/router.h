/*
 * router.h
 *
 *  Created on: 05.01.2016
 *      Author: stefan
 */

#ifndef ROUTER_H_
#define ROUTER_H_

#include <systemc.h>

SC_MODULE(router)
{
    sc_in<bool> clk;
    sc_in<packet> in;

    sc_out<packet> out;
    sc_out<bool> dataPending;

    // Constructor
    router(sc_module_name name, int genSpeed, int maxAddress);

    void sendPackageRandomly();
//    void ackDataReceived();

  private:
    int genSpeed;
    int maxAddressCnt;

    bool getRandomBool() const;
    int getRandomAddress() const;
    int getRandomDatum() const;
};

#endif /* ROUTER_H_ */

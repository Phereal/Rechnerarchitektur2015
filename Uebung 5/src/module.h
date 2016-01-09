/*
 * module.h
 *
 *  Created on: 05.01.2016
 *      Author: stefan
 */

#ifndef MODULE_H_
#define MODULE_H_

#include <systemc.h>
#include "paket.h"

SC_MODULE(Switch)
{
    // inputs
    sc_in<bool> clk;

    // interface to producer
    sc_in<paket> routerIn;
    sc_out<paket> routerOut;
    //sc_out<bool> *pReceived;

    // Constructor
    Module(sc_module_name name, int sendBufferSize);


    // private Hilfsvariablen
    private:
    int sendBfferSize; // Groesse des Buffers je Ausgang (gesetzt per Konstruktor)

};

#endif /* MODULE_H_ */

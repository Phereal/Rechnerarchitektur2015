/*
 * switch.cpp
 *
 *  Created on: 14.12.2015
 *      Author: steffen
 */

// C/C++ Header
// Class Header
#include "switch.h"

Switch::Switch(sc_module_name name, int n, int m, int bufferSize) :
    sc_module(name), n(n), m(m), bufferSize(bufferSize)
{
  in = new sc_in<packet>[n];
  out = new sc_out<packet>[m];
  buffer = new packet[bufferSize];

  SC_HAS_PROCESS(Switch);
  SC_METHOD(distribute);
  sensitive << clk;
}

void Switch::distribute()
{

}

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
    //All packet in- and outputs
    in = new sc_in<packet> [n];
    out = new sc_out<packet> [m];

    // Is set to true if there is a pending packet on that address.
    pPending = new sc_in<bool> [n];

    //pReceived = new sc_out<bool>[n];

    cBusy = new sc_in<bool> [m];

    buffer = new packet*[m];
    bufferCount = new int[m];
    for (int i = 0; i < m; ++i)
	{
	buffer[i] = new packet[bufferSize];
	bufferCount[i] = 0;
	}

    SC_HAS_PROCESS( Switch);
    SC_METHOD( distribute);
    sensitive << clk.pos();
    }

void Switch::distribute()
    {
    // check every producer
    for (int i = 0; i < n; ++i)
	{
	// producer produced something we havent distributed yet?
	if (pPending[i].read())
	    {
	    // try to save packet in buffer first (buffer not full)
	    if (insertPackageIntoBuffer(i, in[i].read().addr))
		{
		// packet was buffered successfully!
		// pop packet from buffer and send it to sc_out.

		// todo
		}
	    else
		{
		//packet could not be buffered due to the buffer being full.
		//do nothing or display a warning.
		// todo
		}

	    // check if address in range
	    if (in[i].read().addr < m)
		{
		// address in range, check if consumer had enough time to process the last datum
		if (!cBusy[in[i].read().addr].read())
		    {
		    // free, check if data in fifo
		    if (bufferCount[in[i].read().addr] > 0)
			{
			// send data from buffer
			out[cBusy[in[i].read().addr]] = ReadPackageFromBuffer(
				cBusy[in[i].read().addr]);
			//pReceived[ cBusy[in[i].read().addr] ] = true;
			}
		    }
		else
		    {
		    // consumer is busy, do nothing or display a warning.
		    }
		}
	    else
		{
		// address out of range!
		// this causes an exception.

		// todo
		}
	    }
	}
    }

bool Switch::insertPackageIntoBuffer(int pIdx, int cIdx)
    {
    bool result = false;

    if (bufferCount[cIdx] < bufferSize)
	{
	// todo priority packet insertion
	buffer[cIdx][bufferCount[cIdx]] = in[pIdx].read();
	bufferCount[cIdx] += 1;

	result = true;
	}

    return result;
    }

packet Switch::ReadPackageFromBuffer(int cIdx)
    {
    // caller has to make sure that buffer is not empty
    packet tmpPkg = buffer[cIdx][0];

    for (int i = 1; i < bufferSize; ++i)
	{
	buffer[cIdx][i - 1] = buffer[cIdx][i];
	bufferCount[cIdx] -= 1;
	}

    if (bufferCount[cIdx] < 0)
	{
	bufferCount[cIdx] = 0;
	}

    return tmpPkg;
    }

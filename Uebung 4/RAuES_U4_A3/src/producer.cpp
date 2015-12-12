#include "packet.h"
#include <systemc.h>

SC_MODULE (producer)
    {
    sc_in_clk clock; // Clock input of the design
    sv_in<int> genSpeed
    sc_out<packet> out; //gesendete Pakete

    //Bringe genSpeed in den Bereich von 0 bis 100
    void outputGen()
	{
	int tempGenSpeed = genSpeed;
	if (tempGenSpeed < 0)
	    {
	    tempGenSpeed = 0;
	    }
	else if (100 < tempGenSpeed)
	    {
	    tempGenSpeed = 100;
	    }
	}

    //und nun zufällig pakete generieren:

    };

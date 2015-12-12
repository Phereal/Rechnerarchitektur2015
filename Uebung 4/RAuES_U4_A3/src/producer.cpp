#include "packet.h"
#include <systemc.h>

SC_MODULE (producer)
    {
    sc_in_clk clock; // Clock input of the design
    sc_in<int> genSpeed;
    sc_out<packet> out; //gesendete Pakete

    //Bringe genSpeed in den Bereich von 0 bis 100
    int genSpeedRoundToPercent()
	{
	int tempGenSpeed = genSpeed.read();
	if (tempGenSpeed < 0)
	    {
	    tempGenSpeed = 0;
	    }
	else if (100 < tempGenSpeed)
	    {
	    tempGenSpeed = 100;
	    }
	return tempGenSpeed;
	}

    //Und nun zufällig pakete generieren:
    void randomlySendPacket()
	{
	if((rand() % 100) < genSpeedRoundToPercent())
	    {
	    //out = packet(...)
	    }


	}

    };

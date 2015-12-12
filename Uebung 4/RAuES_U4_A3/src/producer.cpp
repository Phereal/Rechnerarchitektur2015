#include "packet.h"
#include <systemc.h>

SC_MODULE (producer)
    {
    sc_in_clk clock; // Clock input of the design
    sc_in<int> genSpeed; //%-Chance, dass ein Paket generiert wird.
    sc_in<int> addressRange; //Höchste Adresse, die zur Generierung gewählt werden darf.
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

    //Gibt true oder false zurück
    bool getRandomBool()
	{
	return rand() %2 == 1;
	}

    //Und nun zufällig pakete generieren:
    void randomlySendPacket()
	{
	if((rand() % 100) < genSpeedRoundToPercent())
	    {
	    int randomAddress = rand() % addressRange;
	    out = packet(randomAddress, rand(), getRandomBool());
	    }
	}
    };

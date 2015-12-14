#ifndef PRODUCER_H_F
#define PRODUCER_H_F
#ifdef old

//producer erstellt abhängig von addressRange pro Clock-cycle zufällig packets.
//Die packets haben zufällige Werte und haben zufällig eine Flag.
//Die höchste Adresse wird dem Producer über ein Signal geliefert.
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
      if(tempGenSpeed < 0)
      {
        tempGenSpeed = 0;
      }
      else if(100 < tempGenSpeed)
      {
        tempGenSpeed = 100;
      }
      return tempGenSpeed;
    }

    //Gibt true oder false zurück
    bool getRandomBool()
    {
      return rand() % 2 == 1;
    }

    //Generiere ein Paket.
    void randomlySendPacket()
    {
      if((rand() % 100) < genSpeedRoundToPercent())
      {
        int randomAddress = rand() % addressRange;
        out = packet(randomAddress, rand(), getRandomBool());
      }
    }

    //KONSTRUKTUR
    SC_CTOR(producer)
    {
      SC_METHOD(randomlySendPacket);
      sensitive << clock;
    }
};
#endif

#endif

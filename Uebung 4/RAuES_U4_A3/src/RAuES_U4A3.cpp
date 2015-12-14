#include <iostream>
#include <cstdint>
#include <cstdlib> /* srand */
#include <ctime>   /* time */


#include <systemc.h>

#include "packet.h"
#include "producer.h"
#include "consumer.h"
#include "switch.h"

using namespace std;

#define K_DEBUG


/*
 * Das Klasse waiter dient dazu, die Simulation nach 1ms zu stoppen.
 */
#ifdef K_DEBUG
SC_MODULE(waiter)
{

    void waiting()
    {
      wait(1, SC_MS);
      sc_stop();
    }

    SC_CTOR(waiter)
    {
      SC_THREAD(waiting);
    }
};
#endif


int sc_main(int argc, char *argv[])
{
  int numIn, numOut, genSpeed, inClock, outClock, bufferSize;


  // ------------------------------
  // check for right parameter count
  // ------------------------------
  if(argc != 7)
  {
    return -1;
  }


  // ------------------------------
  // Convert parameters from strings to integers
  // ------------------------------
  numIn = stoi(argv[1]);
  numOut = stoi(argv[2]);
  genSpeed = stoi(argv[3]);
  inClock = stoi(argv[4]);
  outClock = stoi(argv[5]);
  bufferSize = stoi(argv[6]);


  // ------------------------------
  // Check parameters
  // ------------------------------
  if((numIn < 0) || (numIn > 127))
  {
    return -1;
  }
  if((numOut < 0) || (numOut > 127))
  {
    return -1;
  }
  if((genSpeed < 0) || (genSpeed > 100))
  {
    return -1;
  }
  if((inClock < 1) || (inClock > 100))
  {
    return -1;
  }
  if((outClock < 1) || (outClock > 100))
  {
    return -1;
  }
  if(inClock < outClock)
  {
    return -1;
  }
  if(bufferSize < 0)
  {
    return -1;
  }


  // ------------------------------
  // initialize random seed for producer
  // ------------------------------
  srand (time(NULL));


  // ------------------------------
  // clocks
  // ------------------------------
  sc_clock clkIn("clkIn", inClock, SC_NS, 0.5);
  sc_clock clkOut("clkOut", outClock, SC_NS, 0.5);


  // ------------------------------
  // signals
  // ------------------------------
  sc_signal<packet> inList[numIn];
  sc_signal<packet> outList[numOut];

  //sc_signal<bool> pReceived[numIn];
  sc_signal<bool> pPending[numIn];

  sc_signal<bool> cBusy[numOut];


  // ------------------------------
  // Erstellen der Objekte der Klassen
  // ------------------------------
  #ifdef K_DEBUG
  waiter w("Waiter");
  #endif
  producer *pList[numIn];
  consumer *cList[numOut];
  Switch sw("Switch", numIn, numOut, bufferSize);

  for(int i = 0; i < numIn; ++i)
  {
    string name = "Producer";
    name.append(to_string(i));
    pList[i] = new producer(name.c_str(), genSpeed, numOut);
  }

  for(int i = 0; i < numOut; ++i)
  {
    string name = "Consumer";
    name.append(to_string(i));
    cList[i] = new consumer(name.c_str());
  }


  // ------------------------------
  // Connect Signals
  // ------------------------------
  // connect switch clock
  sw.clk(clkIn);
  // connect signals to producer and switch inputs
  for(int i = 0; i < numIn; ++i)
  {
    pList[i]->clk(clkIn);
    pList[i]->out(inList[i]);
    //pList[i]->dataReceived(pReceived[i]);
    pList[i]->dataPending(pPending[i]);

    sw.in[i](inList[i]);
    //sw.pReceived[i](pReceived[i]);
    sw.pPending[i](pPending[i]);
  }
  // connect signals to consumer and switch outputs
  for(int i = 0; i < numOut; ++i)
  {
    cList[i]->clk(clkOut);
    cList[i]->in(outList[i]);
    cList[i]->busy(cBusy[i]);
    sw.out[i](outList[i]);
    sw.cBusy[i](cBusy[i]);
  }


  // ------------------------------
  // Erstellen eine Impulsdiagrammes, dass mit dem Plugion GTK_wave betrachtet werden kann.
  // Hierdurch wird ein Debugging möglich.
  // ------------------------------
  sc_trace_file* Tf;
  Tf = sc_create_vcd_trace_file("waves");
  Tf->set_time_unit(1, SC_NS);
  sc_trace(Tf, clkIn, "clkIn");
  sc_trace(Tf, clkOut, "clkOut");


  // ------------------------------
  // Start simulation
  // ------------------------------
  sc_start();


  // ------------------------------
  // Close file of the wave diagramm
  // ------------------------------
  sc_close_vcd_trace_file(Tf);


  return 0;
}

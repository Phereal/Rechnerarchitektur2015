#include <iostream>
#include <cstdint>

#include <systemc.h>

#include "packet.h"

using namespace std;


int sc_main(int argc, char *argv[])
{
  int numIn, numOut, genSpeed, inClock, outClock, bufferSize;

  if( argc != 7 )
  {
    return -1;
  }

  numIn       = stoi(argv[1]);
  numOut      = stoi(argv[2]);
  genSpeed    = stoi(argv[3]);
  inClock     = stoi(argv[4]);
  outClock    = stoi(argv[5]);
  bufferSize  = stoi(argv[6]);

  if( (numIn < 0) || (numIn > 127) )
  {
    return -1;
  }
  if( (numOut < 0) || (numOut > 127) )
  {
    return -1;
  }
  if( (genSpeed < 0) || (genSpeed > 100) )
  {
    return -1;
  }
  if( (inClock < 1) || (inClock > 100) )
  {
    return -1;
  }
  if( (outClock < 1) || (outClock > 100) )
  {
    return -1;
  }
  if( inClock < outClock )
  {
    return -1;
  }
  if( bufferSize < 0 )
  {
    return -1;
  }

  // c
  sc_clock clkIn("clkIn", inClock, SC_NS, 0.5);
  sc_clock clkOut("clkOut", outClock, SC_NS, 0.5);

  return 0;
}

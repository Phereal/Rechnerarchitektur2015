/*
 * Producer.cpp
 *
 *  Created on: 14.12.2015
 *      Author: steffen und daniel!
 */

// C / C++ Header
#include <cstdlib> /* rand */
// Class Header
#include "producer.h"

producer::producer(sc_module_name name, int genSpeed, int maxAddressCnt) :
    sc_module(name), genSpeed(genSpeed), maxAddressCnt(maxAddressCnt)
{
  SC_HAS_PROCESS(producer);
  SC_METHOD(sendPackageRandomly);
  sensitive << clk.pos();

  //SC_METHOD(ackDataReceived);
  //sensitive << dataReceived;
}

void producer::sendPackageRandomly()
{
  if((rand() % 100) < genSpeed)
  {
    out = packet(getRandomAddress(), getRandomDatum(), getRandomBool());
    dataPending = true;
  }
}

//void producer::ackDataReceived()
//{
//  if(dataReceived)
//  {
//    dataPending = false;
//  }
//}

//Erzeugt zufällig true oder false.
bool producer::getRandomBool() const
{
  return ((rand() % 2) == 1);
}

//Erzeugt eine zufällige Adresse zwischen 0 und maxAdressCnt.
int producer::getRandomAddress() const
{
  return (rand() % maxAddressCnt);
}

//Erzeugt ein zufälliges Datum zwischen -RAND_MAX und RAND_MAX.
int producer::getRandomDatum() const
{
  if(getRandomBool())
  {
    return (rand());
  }
  else
  {
    return (rand() * (-1));
  }
}

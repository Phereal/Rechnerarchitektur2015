/*
 * Producer.cpp
 *
 *  Created on: 14.12.2015
 *      Author: steffen
 */

// C/C++ Header
#include <cstdlib> /* rand */
// Class Header
#include "producer.h"

producer::producer(sc_module_name name, int genSpeed) :
    sc_module(name), genSpeed(genSpeed)
{
  SC_HAS_PROCESS(producer);
  SC_METHOD(sendPackageRandomly);
  sensitive << clk;
}

void producer::sendPackageRandomly()
{
  if((rand() % 100) < genSpeed)
  {
    out = packet(getRandomAddress(), getRandomDatum(), getRandomBool());
  }
}

bool producer::getRandomBool() const
{
  return ((rand() % 2) == 1);
}

int producer::getRandomAddress() const
{
  return (rand() % K_MAX_ADDRESS);
}

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

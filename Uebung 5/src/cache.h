/*
 * cache.h
 *
 *  Created on: 05.01.2016
 *      Author: stefan
 */

#ifndef CACHE_H_
#define CACHE_H_

#include <systemc.h>
#include <vector>
#include "module.h"

SC_MODULE(cache)
{
  cache(sc_module_name name);

  unsigned char sendPixel(int width, int height);
  void getPixel();
  void init();

  private:
  std::vector<std::vector<unsigned char> > in_image;

};

#endif /* CACHE_H_ */

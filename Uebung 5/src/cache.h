/*
 * cache.h
 *
 *  Created on: 05.01.2016
 *      Author: stefan
 */

#ifndef CACHE_H_
#define CACHE_H_

#include <systemc.h>
#include "module.h"
#include <queue>
#include <iostream>

class cache : public module
{
  cache(sc_module_name name, uint8_t id, uint32_t bufferSize);

  unsigned char readPixel(int width, int height);
  void getPixel(int width, int height);
  void pakethandler();
  void init();

  private:
  bool enable = false;
  bool initialize = false;
  struct pixel {int width; int heigth; unsigned char color;};
  std::queue<pixel> cache_list;

  //lokale Speicherung des eingehenden Paketes
  uint32_t i_id;
  uint8_t i_opcode;
  uint8_t i_sender;
  uint8_t i_receiver;

  uint32_t i_xpos;
  uint32_t i_ypos;
  uint8_t i_color;

  //lokale Speicherung des ausgehenden Paketes
  uint32_t o_id;
  uint8_t o_opcode;
  uint8_t o_sender;
  uint8_t o_receiver;

  uint32_t o_xpos;
  uint32_t o_ypos;
  uint8_t o_color;

  virtual bool process(paket &pkg);

};

#endif /* CACHE_H_ */

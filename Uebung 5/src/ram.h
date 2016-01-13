/*
 * ram.h
 *
 *  Created on: 05.01.2016
 *      Author: stefan
 */

#ifndef RAM_H_
#define RAM_H_

#include <systemc.h>
#include "pgm.h"
#include <vector>
#include "module.h"

class ram : public module
{
  public:
  ram(sc_module_name name, uint8_t id, uint32_t bufferSize);

  private: //Methoden
  unsigned char readPixel();
  void writePixel();
  void readPGM();
  void writePGM();
  bool nxtPixel();
  void pakethandler();
  void init();

  private: //Variablen
  bool enable = false;
  bool initialize = false;
  int width = 0;
  int height = 0;
  int depth = 0;
  std::string infile = "dfki.pgm";
  std::string outfile = "refoo.pgm";
  std::vector<std::vector<unsigned char> > in_image;
  std::vector<std::vector<unsigned char> > out_image;
  std::vector<std::vector<bool> > nxt_map;

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

#endif /* RAM_H_ */

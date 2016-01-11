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

SC_MODULE(ram)
{
  sc_in<bool> clk;
  ram(sc_module_name name);

  unsigned char readPixel(int width, int height);
  void writePixel(int width, int height, unsigned char data);
  void readPGM();
  void writePGM();
  void nxtPixel(int &width, int &heigth);
  void init();
  void process();

  private:
  int width = 0;
  int height = 0;
  int depth = 0;
  std::string infile = "/home/stefan/Rechnerarchitektur2015/Uebung 5/src/dfki.pgm";
  std::string outfile = "/home/stefan/Rechnerarchitektur2015/Uebung 5/src/refoo.pgm";
  std::vector<std::vector<unsigned char> > in_image;
  std::vector<std::vector<unsigned char> > out_image;
  std::vector<std::vector<bool> > nxt_map;

};

#endif /* RAM_H_ */

#include <iostream>
#include <systemc.h>

SC_MODULE(alu)
{

    sc_in<int> in;
    sc_out<int> res;

    SC_CTOR(alu)
    {
      sensitive << in;

    }
};

int sc_main(int, char *[])
{
  alu alui("alu");

  sc_signal<int> number;

  alui.in(number);


  sc_start();

}

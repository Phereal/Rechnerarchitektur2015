#include <iostream>
#include <systemc.h>
using namespace std;



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


SC_MODULE(Switch)
{
    sc_in<bool> clk;
    sc_in<sc_uint<8> > instruction;
    sc_in<sc_int<64> > dataA;
    sc_in<sc_int<64> > dataB;

    sc_out<sc_int<64> > result;

    void inst()
    {
    }

    SC_CTOR(Switch)
    {
      SC_METHOD(inst);

      sensitive << clk.pos();
    }
};

int sc_main(int, char *[])
{
  // clock
  sc_clock clk("clk", 10, SC_NS, 0.5);

  // inputs
  sc_signal<sc_uint<8> > instruction;
  sc_signal<sc_int<64> > dataA;
  sc_signal<sc_int<64> > dataB;

  // outputs
  sc_signal<sc_int<64> > result;

  // modules
  waiter w("Waiter");
  alu alui("alu");
  tester test("Tester");

  // module alu
  alui.clk(clk);
  alui.instruction(instruction);
  alui.dataA(dataA);
  alui.dataB(dataB);
  alui.result(result);


  // tracer
  sc_trace_file* Tf;
  Tf = sc_create_vcd_trace_file("waves");
  Tf->set_time_unit(1, SC_NS);
  sc_trace(Tf, clk, "clk");
  sc_trace(Tf, instruction, "instruction");
  sc_trace(Tf, dataA, "dataA");
  sc_trace(Tf, dataB, "dataB");
  sc_trace(Tf, result, "result");

  sc_start();

  sc_close_vcd_trace_file(Tf);

  return 0;

}


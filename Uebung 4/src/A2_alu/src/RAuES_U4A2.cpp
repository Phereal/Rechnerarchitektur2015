#include <iostream>
#include <systemc.h>
using namespace std;


// InstructionSet
#define IS_ADD  (0x01)
#define IS_SUB  (0x02)
#define IS_MUL  (0x03)
#define IS_DIV  (0x04)
#define IS_MOD  (0x05)
#define IS_BS   (0x10)
#define IS_BOR  (0x11)
#define IS_BAND (0x12)
#define IS_BXOR (0x13)
#define IS_COMP (0x14)


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

SC_MODULE(tester)
{
    sc_out<sc_uint<8> > instruction;
    sc_out<sc_int<64> > dataA;
    sc_out<sc_int<64> > dataB;

    sc_in<bool> clk;
    sc_in<sc_int<64> > result;

    void send_values()
    {
      instruction = 0x00;
      wait(10, SC_NS);
      instruction = IS_DIV;
      dataA = 0x10;
      dataB = 0x00;
      wait(10, SC_NS);
    }

    void read_result()
    {
      cout << result << endl;
    }

    SC_CTOR(tester)
    {

      SC_THREAD(send_values);
      SC_METHOD(read_result);
    }
};

SC_MODULE(alu)
{
    sc_in<bool> clk;
    sc_in<sc_uint<8> > instruction;
    sc_in<sc_int<64> > dataA;
    sc_in<sc_int<64> > dataB;

    sc_out<sc_int<64> > result;

    void inst()
    {
      switch( instruction.read() )
      {
        // Addition
        case IS_ADD:
          result.write(dataA.read() + dataB.read());
          break;

          // Substraktion
        case IS_SUB:
          result.write(dataA.read() - dataB.read());
          break;

          // Multiplikation
        case IS_MUL:
          result.write(dataA.read() * dataB.read());
          break;

          // Division
        case IS_DIV:
          if( dataB.read() )
          {
            result.write(dataA.read() / dataB.read());
          }
          // todo
          // else
          break;

          // Modulo
        case IS_MOD:
          result.write(dataA.read() % dataB.read());
          break;

          // Bit Shifting
        case IS_BS:
          result.write(dataA.read() << dataB.read());
          break;

          // Bit Or
        case IS_BOR:
          result.write((dataA.read() | dataB.read()));
          break;

          // Bit And
        case IS_BAND:
          result.write((dataA.read() & dataB.read()));
          break;

          // Bit Xor
        case IS_BXOR:
          result.write((dataA.read() ^ dataB.read()));
          break;

          // Vergleich
        case IS_COMP:
          if(dataA.read() == dataB.read())
          {
            result.write(0);
          }
          else if(dataA.read() > dataB.read())
          {
            result.write(-1);
          }
          else
          {
            result.write(1);
          }
          break;

        default:
          // todo
          //result.write(0);
          break;
      }
    }

    SC_CTOR(alu)
    {
      SC_METHOD(inst);

      sensitive << clk.pos();
    }
}
;

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

  // module tester
  test.clk(clk);
  test.instruction(instruction);
  test.dataA(dataA);
  test.dataB(dataB);
  test.result(result);

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


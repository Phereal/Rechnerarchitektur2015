#include <iostream>
#include <string>
#include <cstdint>
#include <exception>
#include <systemc.h>
using namespace std;

#define K_DEBUG
#ifdef K_DEBUG
  #define PRINT_DEBUG(a, b, c, d) { cout << (a) << to_string((int64_t)(b)) << ", " << to_string((int64_t)(c)) << " = " << to_string((int64_t)(d)) << endl; }
#else
  #define PRINT_DEBUG(a, b, c, d) {}
#endif

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

      void testing()
      {
        // Initialisierung der Testarrays
        sc_int<64> testInputA[10] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
        sc_int<64> testInputB[10] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09};
        sc_int<64> testOutput[10];

        // Testing the addition
        testOutput[0] = 0x00;
        testOutput[1] = 0x02;
        testOutput[2] = 0x04;
        testOutput[3] = 0x06;
        testOutput[4] = 0x08;
        testOutput[5] = 0x0A;
        testOutput[6] = 0x0C;
        testOutput[7] = 0x0E;
        testOutput[8] = 0x10;
        testOutput[9] = 0x12;
        cout << " Begin testing the addition! " << endl;
        for (int i = 0; i < 10; ++i)
        {
          instruction = 0x00;
          wait(10, SC_NS);
          instruction = IS_ADD;
          dataA = testInputA[i];
          dataB = testInputB[i];
          wait(10, SC_NS);
          if( result != testOutput[i] )
          {
            throw string("ERROR: " + to_string((int64_t)result.read()) + " != " + to_string((int64_t)testOutput[i]) + " at i=" + to_string(i)).c_str();
          }
        }
        cout << " End testing the addition! " << endl;

        // Testing the substraction
        testOutput[0] = 0x00;
        testOutput[1] = 0x00;
        testOutput[2] = 0x00;
        testOutput[3] = 0x00;
        testOutput[4] = 0x00;
        testOutput[5] = 0x00;
        testOutput[6] = 0x00;
        testOutput[7] = 0x00;
        testOutput[8] = 0x00;
        testOutput[9] = 0x00;
        cout << " Begin testing the substraction! " << endl;
        for (int i = 0; i < 10; ++i)
        {
          instruction = 0x00;
          wait(10, SC_NS);
          instruction = IS_SUB;
          dataA = testInputA[i];
          dataB = testInputB[i];
          wait(10, SC_NS);
          if( result != testOutput[i] )
          {
            throw string("ERROR: " + to_string((int64_t)result.read()) + " != " + to_string((int64_t)testOutput[i]) + " at i=" + to_string(i)).c_str();
          }
        }
        cout << " End testing the substraction! " << endl;

        // Testing the multiplication
        testOutput[0] = 0x00;
        testOutput[1] = 0x01;
        testOutput[2] = 0x04;
        testOutput[3] = 0x09;
        testOutput[4] = 0x10;
        testOutput[5] = 0x19;
        testOutput[6] = 0x24;
        testOutput[7] = 0x31;
        testOutput[8] = 0x40;
        testOutput[9] = 0x51;
        cout << " Begin testing the multiplication! " << endl;
        for (int i = 0; i < 10; ++i)
        {
          instruction = 0x00;
          wait(10, SC_NS);
          instruction = IS_MUL;
          dataA = testInputA[i];
          dataB = testInputB[i];
          wait(10, SC_NS);
          if( result != testOutput[i] )
          {
            throw string("ERROR: " + to_string((int64_t)result.read()) + " != " + to_string((int64_t)testOutput[i]) + " at i=" + to_string(i)).c_str();
          }
        }
        cout << " End testing the multiplication! " << endl;

        // Testing the division
        testOutput[0] = 0x00;
        testOutput[1] = 0x01;
        testOutput[2] = 0x01;
        testOutput[3] = 0x01;
        testOutput[4] = 0x01;
        testOutput[5] = 0x01;
        testOutput[6] = 0x01;
        testOutput[7] = 0x01;
        testOutput[8] = 0x01;
        testOutput[9] = 0x01;
        cout << " Begin testing the division! " << endl;
        for (int i = 0; i < 10; ++i)
        {
          instruction = 0x00;
          wait(10, SC_NS);
          instruction = IS_DIV;
          dataA = testInputA[i];
          dataB = testInputB[i];
          wait(10, SC_NS);
          if( result != testOutput[i] )
          {
            throw string("ERROR: " + to_string((int64_t)result.read()) + " != " + to_string((int64_t)testOutput[i]) + " at i=" + to_string(i)).c_str();
          }
        }
        cout << " End testing the division! " << endl;

        // Testing the MOD
        testOutput[0] = 0x00;
        testOutput[1] = 0x00;
        testOutput[2] = 0x00;
        testOutput[3] = 0x00;
        testOutput[4] = 0x00;
        testOutput[5] = 0x00;
        testOutput[6] = 0x00;
        testOutput[7] = 0x00;
        testOutput[8] = 0x00;
        testOutput[9] = 0x00;
        cout << " Begin testing the modulo! " << endl;
        for (int i = 0; i < 10; ++i)
        {
          instruction = 0x00;
          wait(10, SC_NS);
          instruction = IS_MOD;
          dataA = testInputA[i];
          dataB = testInputB[i];
          wait(10, SC_NS);
          if( result != testOutput[i] )
          {
            throw string("ERROR: " + to_string((int64_t)result.read()) + " != " + to_string((int64_t)testOutput[i]) + " at i=" + to_string(i)).c_str();
          }
        }
        cout << " End testing the modulo! " << endl;

        // Testing the BITSHIFT with left shifting

        // #INPUTS
        testInputA[0] = 0x00;
        testInputA[1] = 0x01;
        testInputA[2] = 0x01;
        testInputA[3] = 0x01;
        testInputA[4] = 0x01;
        testInputA[5] = 0x01;
        testInputA[6] = 0x01;
        testInputA[7] = 0x01;
        testInputA[8] = 0x01;
        testInputA[9] = 0x01;

        testInputB[0] = 0x00;
        testInputB[1] = 0x01;
        testInputB[2] = 0x02;
        testInputB[3] = 0x03;
        testInputB[4] = 0x04;
        testInputB[5] = 0x05;
        testInputB[6] = 0x06;
        testInputB[7] = 0x07;
        testInputB[8] = 0x08;
        testInputB[9] = 0x09;

        // #OUTPUT
        testOutput[0] = 0x00;
        testOutput[1] = 0x02;
        testOutput[2] = 0x04;
        testOutput[3] = 0x08;
        testOutput[4] = 0x10;
        testOutput[5] = 0x20;
        testOutput[6] = 0x40;
        testOutput[7] = 0x80;
        testOutput[8] = 0x100;
        testOutput[9] = 0x200;
        cout << " Begin testing the BITSHIFT left shift! " << endl;
        for (int i = 0; i < 10; ++i)
        {
          instruction = 0x00;
          wait(10, SC_NS);
          instruction = IS_BS;
          dataA = testInputA[i];
          dataB = testInputB[i];
          wait(10, SC_NS);
          if( result != testOutput[i] )
          {
            throw string("ERROR: " + to_string((int64_t)result.read()) + " != " + to_string((int64_t)testOutput[i]) + " at i=" + to_string(i)).c_str();
          }
        }
        cout << " End testing the BITSHIFT left shift! " << endl;
        // Testing the BITSHIFT with right shifting

        // #INPUTS
        testInputA[0] = 0x00;
        testInputA[1] = 0x01;
        testInputA[2] = 0x01;
        testInputA[3] = 0x01;
        testInputA[4] = 0x01;
        testInputA[5] = 0x01;
        testInputA[6] = 0x01;
        testInputA[7] = 0x01;
        testInputA[8] = 0x01;
        testInputA[9] = 0x01;

        testInputB[0] = 0x00;
        testInputB[1] = -0x01;
        testInputB[2] = -0x02;
        testInputB[3] = -0x03;
        testInputB[4] = -0x04;
        testInputB[5] = -0x05;
        testInputB[6] = -0x06;
        testInputB[7] = -0x07;
        testInputB[8] = -0x08;
        testInputB[9] = -0x09;

        // #OUTPUT
        testOutput[0] = 0x00;
        testOutput[1] = -0x8000000000000000;
        testOutput[2] = 0x4000000000000000;
        testOutput[3] = 0x2000000000000000;
        testOutput[4] = 0x1000000000000000;
        testOutput[5] = 0x800000000000000;
        testOutput[6] = 0x400000000000000;
        testOutput[7] = 0x200000000000000;
        testOutput[8] = 0x100000000000000;
        testOutput[9] = 0x80000000000000;
        cout << " Begin testing the BITSHIFT right shift! " << endl;
        for (int i = 0; i < 10; ++i)
        {
          instruction = 0x00;
          wait(10, SC_NS);
          instruction = IS_BS;
          dataA = testInputA[i];
          dataB = testInputB[i];
          wait(10, SC_NS);
          if( result != testOutput[i] )
          {
            throw string("ERROR: " + to_string((int64_t)result.read()) + " != " + to_string((int64_t)testOutput[i]) + " at i=" + to_string(i)).c_str();
          }
        }
        cout << " End testing the BITSHIFT right shift! " << endl;

        // Testing the Bitwise OR

        // #INPUTS
        testInputA[0] = 0x00;
        testInputA[1] = 0xAAAAAA;
        testInputA[2] = 0xAAAAAA;
        testInputA[3] = 0xAAAAAA;
        testInputA[4] = 0xAAAAAA;
        testInputA[5] = 0xAAAAAA;
        testInputA[6] = 0xAAAAAA;
        testInputA[7] = 0xAAAAAA;
        testInputA[8] = 0xAAAAAA;
        testInputA[9] = 0xAAAAAA;

        testInputB[0] = 0x00;
        testInputB[1] = 0x01;
        testInputB[2] = 0x02;
        testInputB[3] = 0x03;
        testInputB[4] = 0x04;
        testInputB[5] = 0x05;
        testInputB[6] = 0x06;
        testInputB[7] = 0x07;
        testInputB[8] = 0x08;
        testInputB[9] = 0x09;

        // #OUTPUT
        testOutput[0] = 0x00;
        testOutput[1] = 0xAAAAAB;
        testOutput[2] = 0xAAAAAA;
        testOutput[3] = 0xAAAAAB;
        testOutput[4] = 0xAAAAAE;
        testOutput[5] = 0xAAAAAF;
        testOutput[6] = 0xAAAAAE;
        testOutput[7] = 0xAAAAAF;
        testOutput[8] = 0xAAAAAA;
        testOutput[9] = 0xAAAAAB;
        cout << " Begin testing the Bitwise OR! " << endl;
        for (int i = 0; i < 10; ++i)
        {
          instruction = 0x00;
          wait(10, SC_NS);
          instruction = IS_BOR;
          dataA = testInputA[i];
          dataB = testInputB[i];
          wait(10, SC_NS);
          if( result != testOutput[i] )
          {
            throw string("ERROR: " + to_string((int64_t)result.read()) + " != " + to_string((int64_t)testOutput[i]) + " at i=" + to_string(i)).c_str();
          }
        }
        cout << " End testing the Bitwise OR! " << endl;

        // Testing the Bitwise AND
        // #INPUTS
        testInputA[0] = 0x00;
        testInputA[1] = 0xFFFFFF;
        testInputA[2] = 0xFFFFFF;
        testInputA[3] = 0xFFFFFF;
        testInputA[4] = 0xFFFFFF;
        testInputA[5] = 0xFFFFFF;
        testInputA[6] = 0xFFFFFF;
        testInputA[7] = 0xFFFFFF;
        testInputA[8] = 0xFFFFFF;
        testInputA[9] = 0xFFFFFF;

        testInputB[0] = 0x00;
        testInputB[1] = 0x01;
        testInputB[2] = 0x10;
        testInputB[3] = 0x100;
        testInputB[4] = 0x1000;
        testInputB[5] = 0x1001;
        testInputB[6] = 0x1010;
        testInputB[7] = 0x1011;
        testInputB[8] = 0x1100;
        testInputB[9] = 0x1101;

        // #OUTPUT
        testOutput[0] = 0x00;
        testOutput[1] = 0x01;
        testOutput[2] = 0x10;
        testOutput[3] = 0x100;
        testOutput[4] = 0x1000;
        testOutput[5] = 0x1001;
        testOutput[6] = 0x1010;
        testOutput[7] = 0x1011;
        testOutput[8] = 0x1100;
        testOutput[9] = 0x1101;
        cout << " Begin testing the Bitwise AND! " << endl;
        for (int i = 0; i < 10; ++i)
        {
          instruction = 0x00;
          wait(10, SC_NS);
          instruction = IS_BAND;
          dataA = testInputA[i];
          dataB = testInputB[i];
          wait(10, SC_NS);
          if( result != testOutput[i] )
          {
            throw string("ERROR: " + to_string((int64_t)result.read()) + " != " + to_string((int64_t)testOutput[i]) + " at i=" + to_string(i)).c_str();
          }
        }
        cout << " End testing the Bitwise AND! " << endl;

        // Testing the Bitwise XOR
        // #INPUTS
        testInputA[0] = 0x00;
        testInputA[1] = 0xFFFFFF;
        testInputA[2] = 0xFFFFFF;
        testInputA[3] = 0xFFFFFF;
        testInputA[4] = 0xFFFFFF;
        testInputA[5] = 0xFFFFFF;
        testInputA[6] = 0xFFFFFF;
        testInputA[7] = 0xFFFFFF;
        testInputA[8] = 0xFFFFFF;
        testInputA[9] = 0xFFFFFF;

        testInputB[0] = 0x00;
        testInputB[1] = 0x01;
        testInputB[2] = 0x10;
        testInputB[3] = 0x100;
        testInputB[4] = 0x1000;
        testInputB[5] = 0x1001;
        testInputB[6] = 0x1010;
        testInputB[7] = 0x1011;
        testInputB[8] = 0x1100;
        testInputB[9] = 0x1101;

        // #OUTPUT
        testOutput[0] = 0x00;
        testOutput[1] = 0xFFFFFE;
        testOutput[2] = 0xFFFFEF;
        testOutput[3] = 0xFFFEFF;
        testOutput[4] = 0xFFEFFF;
        testOutput[5] = 0xFFEFFE;
        testOutput[6] = 0xFFEFEF;
        testOutput[7] = 0xFFEFEE;
        testOutput[8] = 0xFFEEFF;
        testOutput[9] = 0xFFEEFE;
        cout << " Begin testing the Bitwise XOR! " << endl;
        for (int i = 0; i < 10; ++i)
        {
          instruction = 0x00;
          wait(10, SC_NS);
          instruction = IS_BXOR;
          dataA = testInputA[i];
          dataB = testInputB[i];
          wait(10, SC_NS);
          if( result != testOutput[i] )
          {
            throw string("ERROR: " + to_string((int64_t)result.read()) + " != " + to_string((int64_t)testOutput[i]) + " at i=" + to_string(i)).c_str();
          }
        }
        cout << " End testing the Bitwise XOR! " << endl;

        // Testing the Comparator
        // #INPUTS
        testInputA[0] = 0x00;
        testInputA[1] = 0x01;
        testInputA[2] = 0x10;
        testInputA[3] = 0x100;
        testInputA[4] = 0x1000;
        testInputA[5] = 0x10000;
        testInputA[6] = 0x10000;
        testInputA[7] = 0x10000;
        testInputA[8] = 0x10000;
        testInputA[9] = 0x10000;

        testInputB[0] = 0x00;
        testInputB[1] = 0x01;
        testInputB[2] = 0x10;
        testInputB[3] = 0x100;
        testInputB[4] = 0x1000;
        testInputB[5] = 0x10001;
        testInputB[6] = 0x10010;
        testInputB[7] = 0x00011;
        testInputB[8] = 0x00100;
        testInputB[9] = 0x00101;

        // #OUTPUT
        testOutput[0] = 0x00;
        testOutput[1] = 0x00;
        testOutput[2] = 0x00;
        testOutput[3] = 0x00;
        testOutput[4] = 0x00;
        testOutput[5] = 0x01;
        testOutput[6] = 0x01;
        testOutput[7] = -0x01;
        testOutput[8] = -0x01;
        testOutput[9] = -0x01;
        cout << " Begin testing the Comparator! " << endl;
        for (int i = 0; i < 10; ++i)
        {
          instruction = 0x00;
          wait(10, SC_NS);
          instruction = IS_COMP;
          dataA = testInputA[i];
          dataB = testInputB[i];
          wait(10, SC_NS);
          if( result != testOutput[i] )
          {
            throw string("ERROR: " + to_string((int64_t)result.read()) + " != " + to_string((int64_t)testOutput[i]) + " at i=" + to_string(i)).c_str();
          }
        }
        cout << " End testing the  Comparator! \n" << endl;
        cout << " ############################ \n" << endl;
        cout << " End of the Testbench!  \n" << endl;
        sc_stop();
      }

      SC_CTOR(tester)
      {
        SC_THREAD(testing);
      }
};

SC_MODULE(alu)
{
    int64_t tmpResult = 0;

    sc_in<bool> clk;
    sc_in<sc_uint<8> > instruction;
    sc_in<sc_int<64> > dataA;
    sc_in<sc_int<64> > dataB;

    sc_out<sc_int<64> > result;

    void inst()
    {
      switch(instruction.read())
      {
        // Addition
        case IS_ADD:
          tmpResult = dataA.read() + dataB.read();
          result.write(tmpResult);
          PRINT_DEBUG("Add: ", dataA.read(), dataB.read(), tmpResult);
          break;

          // Substraktion
        case IS_SUB:
          tmpResult = dataA.read() - dataB.read();
          result.write(tmpResult);
          PRINT_DEBUG("Sub: ", dataA.read(), dataB.read(), tmpResult);
          break;

          // Multiplikation
        case IS_MUL:
          tmpResult = dataA.read() * dataB.read();
          result.write(tmpResult);
          PRINT_DEBUG("Mul: ", dataA.read(), dataB.read(), tmpResult);
          break;

          // Division
        case IS_DIV:
          // check for division through 0
          if(dataB.read())
          {
            tmpResult = dataA.read() / dataB.read();
            result.write(tmpResult);
            PRINT_DEBUG("Div: ", dataA.read(), dataB.read(), tmpResult);
          }
          else
          {
            tmpResult = 0x00;
            result.write(tmpResult);
            PRINT_DEBUG("Div Error: ", dataA.read(), dataB.read(), tmpResult);
          }
          break;

          // Modulo
        case IS_MOD:
          if(dataB.read())
          {
            tmpResult = dataA.read() % dataB.read();
            result.write(tmpResult);
            PRINT_DEBUG("Mod: ", dataA.read(), dataB.read(), tmpResult);
          }
          else
          {
            tmpResult = 0x00;
            result.write(tmpResult);
            PRINT_DEBUG("Mod Error: ", dataA.read(), dataB.read(), tmpResult);
          }
          break;

          // Bit Shifting
        case IS_BS:
          tmpResult = dataA.read() << dataB.read();
          result.write(tmpResult);
          PRINT_DEBUG("Shift: ", dataA.read(), dataB.read(), tmpResult);
          break;

          // Bit Or
        case IS_BOR:
          tmpResult = (dataA.read() | dataB.read());
          result.write(tmpResult);
          PRINT_DEBUG("Or: ", dataA.read(), dataB.read(), tmpResult);
          break;

          // Bit And
        case IS_BAND:
          tmpResult = (dataA.read() & dataB.read());
          result.write(tmpResult);
          PRINT_DEBUG("And: ", dataA.read(), dataB.read(), tmpResult);
          break;

          // Bit Xor
        case IS_BXOR:
          tmpResult = (dataA.read() ^ dataB.read());
          result.write(tmpResult);
          PRINT_DEBUG("Xor: ", dataA.read(), dataB.read(), tmpResult);
          break;

          // Vergleich
        case IS_COMP:
          // return 0 if the same
          if(dataA.read() == dataB.read())
          {
            tmpResult = 0;
          }
          //
          else if(dataA.read() > dataB.read())
          {
            tmpResult = -1;
          }
          else
          {
            tmpResult = 1;
          }
          result.write(tmpResult);
          PRINT_DEBUG("Comp: ", dataA.read(), dataB.read(), tmpResult);
          break;

        default:
          // todo
          //result.write(0);
          tmpResult = 0;
          result.write(tmpResult);
          PRINT_DEBUG("Default: ", dataA.read(), dataB.read(), tmpResult);
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
/*
 * RAuES_U4A2.cpp
 *
 *  Created on: 12.12.2015
 *      Author: Stefan && Steffen && Daniel
 */

#include <iostream>
#include <string>
#include <cstdint>
#include <exception>
#include <systemc.h>
using namespace std;

/*
 * Debug-Definitionen
 * Per Schalter K_DEBUG werden die in der Alu ausgefuehrten Befehle in die
 * Standartausgabe gedruckt. Die Moeglichkeit das per Schalter zu steuern wurde
 * so gewaehlt, um zur Entwicklungszeit und beim Ausfuehren der Tests auch
 * manuell nachvollziehen zu koennen was die alu macht. Weiterhin wird mit
 * aktivem Schalter die Testbench ausgefuehrt.
 * Da fuer die eigentliche Abgabe von uns weder die prints noch die Testbench
 * gewuenscht ist (da der Pruefer vrsl. eine eigene Testbench, mit ggf. eigenen
 * prints hat und somit Fehlverhalten und eine unuebersichtliche Konsole
 * vermieden wird).
 */

//#define K_DEBUG
#ifdef K_DEBUG
  #define PRINT_DEBUG(a, b, c, d) { cout << (a) << to_string((int64_t)(b)) << ", " << to_string((int64_t)(c)) << " = " << to_string((int64_t)(d)) << endl; }
#else
  #define PRINT_DEBUG(a, b, c, d) {}
#endif

/*
 * InstructionSet
 * Die folgenden Definitionen dienen einem leichteren Verständis der Operationen,
 * sodass sich der Hex-Wert nicht gemerkt werden muss. Außerdem muss man bei
 * Aenderung der Befehle nur an einer Stelle Aenderungen vornehmen.
 */
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

/*
 * Das Klasse waiter dient dazu, die Simulation nach 1ms zu stoppen. Damit,
 * falls z.B. irgend ein unvorhersehbarer Fehler auftritt, die Anwendung nicht
 * unendlich laueft, werden diese Klasse verwendet. 1ms reicht fuer unsere Tests
 * aus. Fuer die Abgabe wird das Modul deaktiviert und die Alu laueft unendlich.
 * Die Klasse basiert auf den Tutorium-Beispielen.
 */
#ifdef K_DEBUG
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
#endif

/*
 * Die Klasse 'tester' stellt die Testbench des Programms dar. Hiermit werden
 * für jede Operation mindestens 10 Testfälle durchgeführt und mit erwarteten
 * Ergebnissen verglichen. Sollte dabei ein Ergebnis nicht den erwarteten Werten
 * entsprechen, wird über eine if-Klausel eine Exception geworfen und das
 * Programm mit einer Fehlermeldung abgebrochen.
 *
 * In der Fehlermeldung wird die Codezeile genannt, in dem der Fehler
 * aufgetreten ist, sowie die Werte für dataA und dataB sowie für result.
 *
 * Am Ende der Testbench wird eine Benachrichtigung ausgegeben, dass die
 * Testbench erfolgreich ausgeführt wurde und das Programm wird über ein
 * 'sc_stop()' beendet.
 */
#ifdef K_DEBUG
SC_MODULE(tester)
{
    // Die Übergabe der Signale für instruction, dataA und dataB sowie clk und result.
    sc_out<sc_uint<8> > instruction;
    sc_out<sc_int<64> > dataA;
    sc_out<sc_int<64> > dataB;

    sc_in<bool> clk;
    sc_in<sc_int<64> > result;

      // Die Methode 'testing()' beinhaltet alle Testfälle und überprüft diese
      // anhand von erwarteten Ausgaben auf richtige Funktionalität.
      void testing()
      {
        /*
         * Initialisierung der Testarrays für dataA und dataB. Der Array testOutput wird instanziert aber erst in den einzelnen Testschritten
         * für die verschiedenen Operation mit Werten initialisiert. Später weden auch für spezielle Operationen andere Werte für die Eingabe-
         * Arrays eingegeben.
         */
        sc_int<64> testInputA[10] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,-0x08,-0x09};
        sc_int<64> testInputB[10] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,-0x08,-0x09};
        sc_int<64> testOutput[10];

        /*
         *  Testing the addition
         *  Die Addition wird getestet, dabei wird über eine for-Schleife jewils die Werte für dataA und dataB addiert und
         *  der sich ergebende Wert 'result' wird mit dem erwarteten Wert aus dem Array testOutput[] verglichen.
         *
         *  Wenn die Werte nicht übereinstimmen, wird eine Exception geworfen. Es werden sowohl positive als auch negative Eingaben addiert,
         *  eine Grenzwertbetrachtung [was passiert bei einem Überlauf/Unterlauf des Bitvektors?] wird hier augrund der Größe der möglichen Zahlen nicht durchgeführt!
         *  2⁶⁴ = 1,844674407×10¹⁹
         *
         *  Vor jedem Wechsel der Operationen, wird 'instruction' einmal neu initialisiert, um sicher zu gehen, dass die alte Operation nicht mehr
         *  ausgeführt wird.
         */
        testOutput[0] = 0x00;
        testOutput[1] = 0x02;
        testOutput[2] = 0x04;
        testOutput[3] = 0x06;
        testOutput[4] = 0x08;
        testOutput[5] = 0x0A;
        testOutput[6] = 0x0C;
        testOutput[7] = 0x0E;
        testOutput[8] = -0x10;
        testOutput[9] = -0x12;
        cout << " Begin testing the addition! " << endl;
        instruction = 0x00;
        wait(10, SC_NS);
        for (int i = 0; i < 10; ++i)
        {
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

        /*
         *  Testing the substraction
         *  Die Subtraktion wird getestet, dabei wird über eine for-Schleife jewils die Werte für dataA und dataB subtrahiert und
         *  der sich ergebende Wert 'result' wird mit dem erwarteten Wert aus dem Array testOutput[] verglichen.
         *
         *  Wenn die Werte nicht übereinstimmen, wird eine Exception geworfen. Es werden sowohl positive als auch negative Eingaben subtrahiert,
         *  eine Grenzwertbetrachtung [was passiert bei einem Überlauf/Unterlauf des Bitvektors?] wird hier augrund der Größe der möglichen Zahlen nicht durchgeführt!
         *  2⁶⁴ = 1,844674407×10¹⁹
         *
         *  Vor jedem Wechsel der Operationen, wird 'instruction' einmal neu initialisiert, um sicher zu gehen, dass die alte Operation nicht mehr
         *  ausgeführt wird.
         */
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
        instruction = 0x00;
        wait(10, SC_NS);
        for (int i = 0; i < 10; ++i)
        {
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

        /*
         *  Testing the multiplication
         *  Die Multiplikation wird getestet, dabei wird über eine for-Schleife jewils die Werte für dataA und dataB multipliziert und
         *  der sich ergebende Wert 'result' wird mit dem erwarteten Wert aus dem Array testOutput[] verglichen.
         *
         *  Wenn die Werte nicht übereinstimmen, wird eine Exception geworfen. Es werden sowohl positive als auch negative Eingaben multipliziert,
         *  eine Grenzwertbetrachtung [was passiert bei einem Überlauf/Unterlauf des Bitvektors?] wird hier augrund der Größe der möglichen Zahlen nicht durchgeführt!
         *  2⁶⁴ = 1,844674407×10¹⁹
         *
         *  Vor jedem Wechsel der Operationen, wird 'instruction' einmal neu initialisiert, um sicher zu gehen, dass die alte Operation nicht mehr
         *  ausgeführt wird.
         */
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
        instruction = 0x00;
        wait(10, SC_NS);
        for (int i = 0; i < 10; ++i)
        {
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

        /*
         *  Testing the division
         *  Die Division wird getestet, dabei wird über eine for-Schleife jewils die Werte für dataA und dataB dividiert und
         *  der sich ergebende Wert 'result' wird mit dem erwarteten Wert aus dem Array testOutput[] verglichen.
         *
         *  Wenn die Werte nicht übereinstimmen, wird eine Exception geworfen. Es werden sowohl positive als auch negative Eingaben dividiert,
         *  eine Grenzwertbetrachtung [was passiert bei einem Überlauf/Unterlauf des Bitvektors?] wird hier augrund der Größe der möglichen Zahlen nicht durchgeführt!
         *  2⁶⁴ = 1,844674407×10¹⁹
         *
         *  Vor jedem Wechsel der Operationen, wird 'instruction' einmal neu initialisiert, um sicher zu gehen, dass die alte Operation nicht mehr
         *  ausgeführt wird.
         */
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
        instruction = 0x00;
        wait(10, SC_NS);
        for (int i = 0; i < 10; ++i)
        {
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

        /*
         *  Testing the modulo
         *  Die Modulo-Operation wird getestet, dabei wird über eine for-Schleife jewils die Werte für dataA und dataB dividiert und
         *  der sich ergebende Rest 'result' wird mit dem erwarteten Wert aus dem Array testOutput[] verglichen.
         *
         *  Wenn die Werte nicht übereinstimmen, wird eine Exception geworfen. Es werden sowohl positive als auch negative Eingaben 'gemodulot',
         *  eine Grenzwertbetrachtung [was passiert bei einem Überlauf/Unterlauf des Bitvektors?] wird hier augrund der Größe der möglichen Zahlen nicht durchgeführt!
         *  2⁶⁴ = 1,844674407×10¹⁹
         *
         *  Vor jedem Wechsel der Operationen, wird 'instruction' einmal neu initialisiert, um sicher zu gehen, dass die alte Operation nicht mehr
         *  ausgeführt wird.
         */

        // #INPUTS
        testInputA[0] = 0x00;
        testInputA[1] = 0x01;
        testInputA[2] = 0x11;
        testInputA[3] = 0x101;
        testInputA[4] = 0x1001;
        testInputA[5] = 0x10001;
        testInputA[6] = 0x100001;
        testInputA[7] = -0x1000001;
        testInputA[8] = -0x10000001;
        testInputA[9] = -0x100000001;

        testInputB[0] = 0x00;
        testInputB[1] = 0x01;
        testInputB[2] = 0x10;
        testInputB[3] = 0x100;
        testInputB[4] = 0x1000;
        testInputB[5] = 0x10000;
        testInputB[6] = 0x100000;
        testInputB[7] = 0x1000000;
        testInputB[8] = 0x10000000;
        testInputB[9] = 0x100000000;

        // #OUTPUTS
        testOutput[0] = 0x00;
        testOutput[1] = 0x00;
        testOutput[2] = 0x01;
        testOutput[3] = 0x01;
        testOutput[4] = 0x01;
        testOutput[5] = 0x01;
        testOutput[6] = 0x01;
        testOutput[7] = -0x01;
        testOutput[8] = -0x01;
        testOutput[9] = -0x01;
        cout << " Begin testing the modulo! " << endl;
        instruction = 0x00;
        wait(10, SC_NS);
        for (int i = 0; i < 10; ++i)
        {
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

        /*
         *  Testing the BITSHIFT left shift!
         *  Die Bitshift Operation wird getestet, dabei wird über eine for-Schleife jewils die Werte von dataA um den Wert von dataB geshiftet und
         *  der sich ergebende Wert 'result' wird mit dem erwarteten Wert aus dem Array testOutput[] verglichen.
         *
         *  Wir sind dabei ausgegangen, dass bei positiven Werten nach links geshiftet wird und bei negativen Werten in dataB nach rechts geshiftet wird.
         *  Der Überlauf, wenn man einen positiven Wert hat und nach rechts shiftet wurde dabei auch betrachtet.
         *
         *  Wenn die Werte nicht übereinstimmen, wird eine Exception geworfen. Es werden sowohl positive als auch negative Eingaben beim Bitshifting betrachtet,
         *  eine Grenzwertbetrachtung [was passiert bei einem Überlauf des Bitvektors?] wird hier augrund der Größe der möglichen Zahlen nicht durchgeführt!
         *  2⁶⁴ = 1,844674407×10¹⁹
         *
         *  Vor jedem Wechsel der Operationen, wird 'instruction' einmal neu initialisiert, um sicher zu gehen, dass die alte Operation nicht mehr
         *  ausgeführt wird.
         */

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
        instruction = 0x00;
        wait(10, SC_NS);
        for (int i = 0; i < 10; ++i)
        {
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

        /*
         *  Testing the BITSHIFT right shift!
         *  Die Bitshift Operation wird getestet, dabei wird über eine for-Schleife jewils die Werte von dataA um den Wert von dataB geshiftet und
         *  der sich ergebende Wert 'result' wird mit dem erwarteten Wert aus dem Array testOutput[] verglichen.
         *
         *  Wir sind dabei ausgegangen, dass bei positiven Werten nach links geshiftet wird und bei negativen Werten in dataB nach rechts geshiftet wird.
         *  Der Überlauf, wenn man einen positiven Wert hat und nach rechts shiftet wurde dabei auch betrachtet.
         *
         *  Wenn die Werte nicht übereinstimmen, wird eine Exception geworfen. Es werden sowohl positive als auch negative Eingaben beim Bitshifting betrachtet,
         *  eine Grenzwertbetrachtung [was passiert bei einem Überlauf des Bitvektors?] wird hier augrund der Größe der möglichen Zahlen nicht durchgeführt!
         *  2⁶⁴ = 1,844674407×10¹⁹
         *
         *  Vor jedem Wechsel der Operationen, wird 'instruction' einmal neu initialisiert, um sicher zu gehen, dass die alte Operation nicht mehr
         *  ausgeführt wird.
         */

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
        instruction = 0x00;
        wait(10, SC_NS);
        for (int i = 0; i < 10; ++i)
        {
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

        /*
         *  Testing the Bitwise OR!
         *  Die Bitwise OR Operation wird getestet, dabei wird über eine for-Schleife jeweils die Werte von dataA um den Wert von dataB verodert und
         *  der sich ergebende Wert 'result' wird mit dem erwarteten Wert aus dem Array testOutput[] verglichen.
         *
         *  Wenn die Werte nicht übereinstimmen, wird eine Exception geworfen. Es werden nur positive Werte überprüft, da bei einem OR egal ist ob der Wert der Zahl
         *  positiv oder negativ ist, da die einzelnen Bits nur verodert werden.
         *  Eine Grenzwertbetrachtung [was passiert bei einem Überlauf/Unterlauf des Bitvektors?] wird hier aufgrund der Größe der möglichen Zahlen nicht durchgeführt!
         *  2⁶⁴ = 1,844674407×10¹⁹
         *
         *  Vor jedem Wechsel der Operationen, wird 'instruction' einmal neu initialisiert, um sicher zu gehen, dass die alte Operation nicht mehr
         *  ausgeführt wird.
         */

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
        instruction = 0x00;
        wait(10, SC_NS);
        for (int i = 0; i < 10; ++i)
        {
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

        /*
               *  Testing the Bitwise AND!
               *  Die Bitwise AND Operation wird getestet, dabei wird über eine for-Schleife jeweils die Werte von dataA um den Wert von dataB verandet und
               *  der sich ergebende Wert 'result' wird mit dem erwarteten Wert aus dem Array testOutput[] verglichen.
               *
               *  Wenn die Werte nicht übereinstimmen, wird eine Exception geworfen. Es werden nur positive Werte überprüft, da bei einem AND egal ist ob der Wert der Zahl
               *  positiv oder negativ ist, da die einzelnen Bits nur verandet werden.
               *  Eine Grenzwertbetrachtung [was passiert bei einem Überlauf/Unterlauf des Bitvektors?] wird hier aufgrund der Größe der möglichen Zahlen nicht durchgeführt!
               *  2⁶⁴ = 1,844674407×10¹⁹
               *
               *  Vor jedem Wechsel der Operationen, wird 'instruction' einmal neu initialisiert, um sicher zu gehen, dass die alte Operation nicht mehr
               *  ausgeführt wird.
               */

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
        instruction = 0x00;
        wait(10, SC_NS);
        for (int i = 0; i < 10; ++i)
        {
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

        /*
               *  Testing the Bitwise XOR!
               *  Die Bitwise XOR Operation wird getestet, dabei wird über eine for-Schleife jeweils die Werte von dataA um den Wert von dataB verxort und
               *  der sich ergebende Wert 'result' wird mit dem erwarteten Wert aus dem Array testOutput[] verglichen.
               *
               *  Wenn die Werte nicht übereinstimmen, wird eine Exception geworfen. Es werden nur positive Werte überprüft, da bei einem XOR egal ist ob der Wert der Zahl
               *  positiv oder negativ ist, da die einzelnen Bits nur verxort werden.
               *  Eine Grenzwertbetrachtung [was passiert bei einem Überlauf/Unterlauf des Bitvektors?] wird hier aufgrund der Größe der möglichen Zahlen nicht durchgeführt!
               *  2⁶⁴ = 1,844674407×10¹⁹
               *
               *  Vor jedem Wechsel der Operationen, wird 'instruction' einmal neu initialisiert, um sicher zu gehen, dass die alte Operation nicht mehr
               *  ausgeführt wird.
               */

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
        instruction = 0x00;
        wait(10, SC_NS);
        for (int i = 0; i < 10; ++i)
        {
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

        /*
               *  Testing the Comparator!
               *  Die Comparator Operation wird getestet, dabei wird über eine for-Schleife jeweils die Werte von dataA um den Wert von dataBverglichen
               *  und entsprechend der Aufgabenstellung wird 'result' mit dem erwarteten Werten aus dem Array testOutput[] verglichen.
               *  [Die Umsetzung ist in der anderen Klasse beschrieben]
               *
               *  Wenn die Werte nicht übereinstimmen, wird eine Exception geworfen.
               *
               *  Vor jedem Wechsel der Operationen, wird 'instruction' einmal neu initialisiert, um sicher zu gehen, dass die alte Operation nicht mehr
               *  ausgeführt wird.
               */

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
        instruction = 0x00;
        wait(10, SC_NS);
        for (int i = 0; i < 10; ++i)
        {
          instruction = IS_COMP;
          dataA = testInputA[i];
          dataB = testInputB[i];
          wait(10, SC_NS);
          if( result != testOutput[i] )
          {
            throw string("ERROR: " + to_string((int64_t)result.read()) + " != " + to_string((int64_t)testOutput[i]) + " at i=" + to_string(i)).c_str();
          }
        }
        // Ende der Testbench und Signalisierung der selben. Über den Befehl sc_stop() wird die Ausführung gestoppt und das Programm beendet.
        cout << " End testing the  Comparator! \n" << endl;
        cout << " ############################ \n" << endl;
        cout << " End of the Testbench!  \n" << endl;
        sc_stop();
      }
      // Konstruktor der Klasse tester die die Methode testing() aufruft.
      SC_CTOR(tester)
      {
        SC_THREAD(testing);
      }
};
#endif

/*
 * Die Klasse alu stellt die Methode inst bereit, in der die einzelnen Berechnungen ausführt werden.
 * Das ganze geschieht relativ selbsertklärend und man benötigt aufgrund der in C++ gut implementierten
 * Bitvektor-Operationen wenige kniffe, um die Berechnungen richtig durchzuführen!
 * Es werden einfach die verschiedenen Befehlscodes (signal instruction) per
 * switch unterschieden. Sollte ein Befehl nicht erkannt werden, wird 0 am
 * Ausgang result angelegt.
 *
 */
SC_MODULE(alu)
{
  // lokale Variable, in der das Ergebnis der Berechnung zwischengespeichert wird.
  // Wurde angelegt, um das berechnete Ergebnis ohne weitere Rechnung an die
  // print-Ausgabe weiterzugeben.
    int64_t tmpResult = 0;

    // Die Ein- und Ausgaenge fuer instruction, clk, dataA und dataB sowie result.
    sc_in<bool> clk;
    sc_in<sc_uint<8> > instruction;
    sc_in<sc_int<64> > dataA;
    sc_in<sc_int<64> > dataB;

    sc_out<sc_int<64> > result;

    // fuert die eigentliche alu aus
    void inst()
    {
      // Ein Switch auf instruction um die jeweilige Operation auszuwerten.
      switch(instruction.read())
      {
        // Addition der Werte von dataA und dataB und Ausgabe in result
        case IS_ADD:
          tmpResult = dataA.read() + dataB.read();
          result.write(tmpResult);
          PRINT_DEBUG("Add: ", dataA.read(), dataB.read(), tmpResult);
          break;

        // Substraktion der Werte von dataA und dataB und Ausgabe in result
        case IS_SUB:
          tmpResult = dataA.read() - dataB.read();
          result.write(tmpResult);
          PRINT_DEBUG("Sub: ", dataA.read(), dataB.read(), tmpResult);
          break;

          // Multiplikation der Werte von dataA und dataB und Ausgabe in result
        case IS_MUL:
          tmpResult = dataA.read() * dataB.read();
          result.write(tmpResult);
          PRINT_DEBUG("Mul: ", dataA.read(), dataB.read(), tmpResult);
          break;

          // Division der Werte von dataA und dataB und Ausgabe in result
          // [Bei Division durch 0 wird 0x00 an result ausgegeben]
        case IS_DIV:
          // check for division through 0
          if(dataB.read())
          {
            tmpResult = dataA.read() / dataB.read();
            result.write(tmpResult);
            PRINT_DEBUG("Div: ", dataA.read(), dataB.read(), tmpResult);
          }
          else // In jedem anderen Fall oder wenn dataB 0x00 ist, wird an result 0x00 ausgegeben.
          {
            tmpResult = 0x00;
            result.write(tmpResult);
            PRINT_DEBUG("Div Error: ", dataA.read(), dataB.read(), tmpResult);
          }
          break;

        // Modulo der Werte von dataA und dataB und Ausgabe in result
        // [Bei Division durch 0 wird 0x00 an result ausgegeben]
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

        // Bit Shifting des Wertes von dataA um den Wert von dataB. Bei positiven
        // Werten wird nach links geshifet, bei negativen Werten nach rechts.
        case IS_BS:
          tmpResult = dataA.read() << dataB.read();
          result.write(tmpResult);
          PRINT_DEBUG("Shift: ", dataA.read(), dataB.read(), tmpResult);
          break;

        // Bitweises OR der Werte von dataA und dataB, Ausgabe per 'result'
        case IS_BOR:
          tmpResult = (dataA.read() | dataB.read());
          result.write(tmpResult);
          PRINT_DEBUG("Or: ", dataA.read(), dataB.read(), tmpResult);
          break;

        // Bitweises AND der Werte von dataA und dataB, Ausgabe per 'result'
        case IS_BAND:
          tmpResult = (dataA.read() & dataB.read());
          result.write(tmpResult);
          PRINT_DEBUG("And: ", dataA.read(), dataB.read(), tmpResult);
          break;

        // Bitweises XOR der Werte von dataA und dataB, Ausgabe per 'result'
        case IS_BXOR:
          tmpResult = (dataA.read() ^ dataB.read());
          result.write(tmpResult);
          PRINT_DEBUG("Xor: ", dataA.read(), dataB.read(), tmpResult);
          break;

        // Vergleich der Werte von dataA und dataB, Ausgabe per 'result'.
        // Wenn Gleichheit dann result = 0, wenn links größer dann result = 1,
        // wenn rechts größer dann result = -1.
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

        // Der Default Fall, wenn die Operation nicht den bekannten entspricht
        // wird 0 an result ausgegeben. Andere Fehlerbehandlungen waren nicht
        // erforderlich durch die Aufgabenstellung.
        default:
          tmpResult = 0;
          result.write(tmpResult);
          PRINT_DEBUG("Default: ", dataA.read(), dataB.read(), tmpResult);
          break;
      }
    }

    // Kostruktor der Klasse mit Sesitivität auf die positive Flanke von clk.
    SC_CTOR(alu)
    {
      SC_METHOD(inst);

      sensitive << clk.pos();
    }
}
;

int sc_main(int, char *[])
{
  // clock T=10ns
  sc_clock clk("clk", 10, SC_NS, 0.5);

  // inputs
  sc_signal<sc_uint<8> > instruction;
  sc_signal<sc_int<64> > dataA;
  sc_signal<sc_int<64> > dataB;

  // outputs
  sc_signal<sc_int<64> > result;

  // Erstellen der Objekte der Klassen
  // waiter und tester nur wenn benoetigt (fuer eigene Tests)
  alu alui("alu");
  #ifdef K_DEBUG
  waiter w("Waiter");
  tester test("Tester");
  #endif

  // Übergeben der Variablen an die Klasse alu
  alui.clk(clk);
  alui.instruction(instruction);
  alui.dataA(dataA);
  alui.dataB(dataB);
  alui.result(result);

  // Übergeben der Variablen an die Klasse tester
  #ifdef K_DEBUG
  test.clk(clk);
  test.instruction(instruction);
  test.dataA(dataA);
  test.dataB(dataB);
  test.result(result);
  #endif

  // Erstellen eine Impulsdiagrammes, dass mit dem Plugion GTK_wave betrachtet werden kann.
  // Hierdurch wird ein Debugging möglich. Die erstellte datei wurde von uns
  // manuell betrachtet um das zeitliche Verhalten unserer Alu zu testen.
  sc_trace_file* Tf;
  Tf = sc_create_vcd_trace_file("waves");
  Tf->set_time_unit(1, SC_NS);
  sc_trace(Tf, clk, "clk");
  sc_trace(Tf, instruction, "instruction");
  sc_trace(Tf, dataA, "dataA");
  sc_trace(Tf, dataB, "dataB");
  sc_trace(Tf, result, "result");

  // starte die siumulation
  sc_start();

  sc_close_vcd_trace_file(Tf);

  return 0;

}

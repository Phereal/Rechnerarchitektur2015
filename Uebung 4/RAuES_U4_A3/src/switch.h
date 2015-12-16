/*
 * switch.h
 *
 *  Created on: 14.12.2015
 *      Author: steffen
 */

#ifndef SWITCH_H_
#define SWITCH_H_

// C/C++ Header
// SystemC Header
#include <systemc.h>
// Custom Header
#include "packet.h"

// Capital S since switch is a keyword
SC_MODULE(Switch)
{
    // inputs
    sc_in<bool> clk;

    // interface to producer
    sc_in<packet> *in;
    sc_in<bool> *pPending;
    //sc_out<bool> *pReceived;

    // interface to consumer
    sc_in<bool> *cBusy;
    sc_out<packet> *out;

    // Constructor
    Switch(sc_module_name name, int n, int m, int bufferSize);

    void distribute();

    // private Hilfsvariablen
    private:
    int n; // Anzahl Eingaenge (gesetzt per Konstruktor)
    int m; // Anzahl Ausgaenge (gesetzt per Konstruktor)
    int bufferSize; // Groesse des Buffers je Ausgang (gesetzt per Konstruktor)
    // pointer auf den Buffer (2-dim. Array, [m][bufferSize])
    // pointer, da die dimensionen erst mit dem Konstruker uebergeben werden,
    // speicher wird also vor der simulation im konstruktor allokiert.
    packet **buffer;
    // speichert die Anzahl der belegten Plaetze fuer jeden Ausgangsbuffer
    int *bufferCount;

    // hilfsmethoden die den Zugriff auf den Buffer regeln (Einhaltung des Fifo prinzips)
    bool insertPackageIntoBuffer(int producer, int consumer);
    packet ReadPackageFromBuffer(int cIdx);
};

#endif /* SWITCH_H_ */

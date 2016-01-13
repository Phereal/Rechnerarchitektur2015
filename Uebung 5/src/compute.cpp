/*
* compute.cpp
*
* erbt von module.cpp
*
*
*/

#include "compute.h"

#include <systemc.h>

compute::compute(sc_module_name name, uint8_t id, uint32_t bufferSize) : module(name, id, bufferSize)
{
    pakethandler();
    sensitive << clk.pos();
}


//Überschreiben der process-Methode der Elternklasse Module
bool compute::process(paket &pkg) {
    //Paket einlesen
    i_id = pkg.id;
    i_opcode = pkg.opcode;
    i_sender = pkg.sender;
    i_receiver = pkg.receiver;
    i_xpos = pkg.xpos;
    i_ypos = pkg.ypos;
    i_color = pkg.color;

    //Paket senden
    pkg.id = o_id;
    pkg.opcode = o_opcode;
    pkg.sender = o_sender;
    pkg.receiver = o_receiver;
    pkg.xpos = o_xpos;
    pkg.ypos = o_ypos;
    pkg.color = o_color;
    return false;
}

void compute::pakethandler() {
    //Es werden nur OPCodes behandelt, die auch durch das compute-Modul verarbeitet werden können.
    switch (i_opcode) {
    case 0x00: break;//[emp] -- leeres Paket (/)
    case 0x01: break;//[exe] -- Berechnungsauftrag (Gateway|Compute)
    case 0x02: break;//[fin] -- Berechnung abgeschlossen (Compute|Gateway)
    case 0x03: break;//[c_req] -- Pixel Anfrage an den Cache (Compute|Cache)
    case 0x04: break;//[r_req] -- Pixel Anfrage an den RAM (Cache|RAM)
    case 0x05: break;//[ack] -- Empfangsbestätigung (/)
    case 0x06: break;//[ic_pay] -- Pixel vom Cache an Compute (Cache|Compute)
    case 0x07: break;//[ir_pay] -- Pixel vom RAM an Cache (RAM|Cache)
    case 0x08: break;//[o_pay] -- Berechneter Pixel an den RAM (Compute|RAM)
    case 0x09: break;//[rfi] -- Bild einzulesen (Gateway|RAM)
    case 0x0A: break;//[rff] -- Bild fertig eingelesen (RAM|Gateway)
    case 0x0B: break;//[wfi] -- Zielbild schreiben (Gateway|RAM)
    case 0x0C: break;//[wff] -- Zielbild schreiben abgeschlossen (RAM|Gateway)
    case 0x0D: break;//[nxt] -- Sende nächsten zu berechnenden Pixel (Gateway|RAM)
    case 0x0E: break;//[nxa] -- Nächster zu berechnender Pixel (RAM|Gateway)
    default: break;
    };
}

void compute::addRequestToQueue(unsigned int id, unsigned int sender, unsigned int receiver, unsigned int xpos, unsigned int ypos) {
    request tmp = { id,sender,receiver,xpos,ypos };
    request_list.push(tmp);
}

void compute::deleteRequestFromQueue(unsigned int id, unsigned int sender, unsigned int receiver, unsigned int xpos, unsigned int ypos) {
    std::queue<request> tmp_request_list;
    //Durchlaufe die Request List und suche nach dem zu löschenden Element
    while (request_list.size() != 0) {
        request tmpRequest = request_list.front();
        if (tmpRequest.id == id && tmpRequest.sender == sender && tmpRequest.receiver == receiver && tmpRequest.xpos == xpos && tmpRequest.ypos == ypos) {
            //Tue nichts, also schreibe füge den Request nicht der tmp_request_list wieder hinzu!
            request_list.pop();
        }
        else {
            tmp_request_list.push(tmpRequest);
            request_list.pop();
        }
    }
    request_list = tmp_request_list;
}

int recalcPixel(int matrix[MATRIX_SIZE][MATRIX_SIZE], int xCoord, int yCoord)
{
    int sum = 0;

    for (int i = 1; i <= MATRIX_SIZE; i++)
    {
        for (int j = 1; j <= MATRIX_SIZE; j++)
        {
            int M = matrix[i - 1][j - 1]; //-1, da Array ab 0 zählen.


                                          //Da wir Integer teilen erhalten wir bei der folgenden Rechnung immer die
                                          //Werte der 9 Pixel, die den Quellpixel und die ihn umgebenden Pixel beschreiben.

            int currentTargetX = (xCoord + i - MATRIX_SIZE / 2);
            int currentTargetY = (yCoord + j - MATRIX_SIZE / 2);

            int currentPixel = getValueAt(currentTargetX, currentTargetY); //getValueAt() ist ein Platzhalter! Ersetzen.

            sum += M * currentPixel;                                    //Führe die eigentliche Formel durch und addierte den Summen-Berechnungsschritt:
        }
    }
    return std::min(std::max(sum, 0), 255);
}
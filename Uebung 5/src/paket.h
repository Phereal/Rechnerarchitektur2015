/*
 * paket.h
 *
 * Ein Paket dient als eine Kommunikationseinheit zwischen den einzelnen Routern bzw. Modulen und
 * beinhaltet dabei neben Statusinformationen [ID; OPCODE; SENDER; RECEIVER; PAYLOAD] ggf. auch einen
 * Bildwert für einen Pixel.
 *
 * Bei dem Payload handelt es sich hier laut Aufgabenstellung um einen Pixel.
 * Jeder Pixel verfügt über eine X- bzw. Y Position auf dem Bild sowie einen Wert,
 * der die Farbgebung des Pixels beschreibt.
 *
 * Für den Payload wird also ein Datentyp namens pixel erstellt
 * pixel = [xpos(int)][ypos(int)][Color(byte)]
 *
 * Jeder Pixel kann einen Wert zwischen 0 und 255 annehmen. Es gibt hierbei keine Über- oder Unterläufe.
 * Dies bedeutet, dass 255 + 1 = 255 ist und 0 - 1 = 0 ist.
 *
 * Ein Paket hat immer einen Senderouter [sender] und einen Empfangsrouter [receiver], damit die Pakete
 * adressiert werden können.
 * Die Adresse ergibt sich aus der horizontalen x und vertikalen y position im noc.
 * x wird durch die untersten 4 Bit(LSB, 0-3), und y durch die obersten 4 Bit (MSB, 4-7) dargestellt.
 *
 *
 * Ein Paket kann dabei in unterschiedlichen Varianten genutzt werden [OPCODE]:
 *
 * 0. Leeres Paket 0x00
 *
 * 1. Als Berechnungsauftrag des Gateway-Moduls für ein Compute-Modul [exe] 0x01
 *
 * 2. Als Bestätigung des Compute-Moduls, dass die Berechnung durchgeführt wurde an das Gateway-Modul [fin] 0x02
 *
 * 3. Als Empfangsanfrage für ein Paket vom Compute-Modul an den Cache [c_req] (Cache-request) 0x03
 *
 * 4  Als Empfangsanfrage für ein Paket vom Cache-Modul an das RAM-Modul [r_req] (RAM-request) 0x04
 *
 * 5. Als Empfangsbestätigung für ein Paket mit der betreffenden Paket-ID [ack] (acknowledge) 0x05
 *
 * 6. Als Informationsträger aufgrund einer Empfangsanfrage an den Cache-Modul vom Compute-Modul  [ic_pay] 0x06
 *
 * 7. Als Informationsträger aufgrund einer Empfangsanfrage an das RAM-Modul vom Cache-Modul [ir_pay] 0x07
 *
 * 8. Als Ergebnis einer Berechnung des Compute-Moduls an das Gateway-Modul [o_pay] 0x08
 *
 * 9. Als Aufforderung des Gateway-Moduls an das RAM-Modul den Inhalt der Bilddatei einzulesen [rfi] 0x09
 *
 * 10. Als Information, des RAM-Moduls, an das Gateway-Modul, das das einlesen beendet ist [rff] 0x0A
 *
 * 11. Als Aufforderung des Gateway-Moduls an das RAM-Modul den Inhalt der zweiten Speicherbank
 *     in die Datei 'noc.pgm' zu schreiben [wfi] 0x0B
 *
 * 12. Als Information, des RAM-Moduls, an das Gateway-Modul, das das schreiben beendet ist [wff] 0x0C
 *
 * 13. Als Aufforderung des Gateway-Moduls an das RAM-Modul die Information über den nächsten zu bearbeitenden
 *     Pixel zu senden [nxt) 0x0D
 *
 * 14. Als Paket des RAM-Moduls an das Gateway Modul mit dem Inhalt des nächsten zu bearbeitenden Pixels. [nxa] 0x0E
 *
 * Da nicht sichergestellt werden kann, dass das Paket empfangen wird [Buffer des Empfängers ist voll],
 * müssen wir hier mit einer Empfangsbestätigung arbeiten. Die Empfangsbestätigung muss über eine eindeutige
 * Identifikation verfügen, da sonst nicht zugeordnet werden kann, welches Acknowledge welchem Paket zugeordnet
 * werden kann. Auf die empfangsbrestätigung selber wird keine neue Empfangsbestätigung gesendet.
 *
 * Die ID ist ein HASH-Wert, der aus OPCODE, SENDER und RECEIVER gebildet wird. Empfangsbestätigungen werden immer
 * mit dem gleichen HASH-Wert gesendet, wie die Pakete die empfangen wurden.
 */

#ifndef PAKET_H_
#define PAKET_H_

#include <iostream>
#include <systemc.h>
#include <utility>
#include <cstdint>

using namespace std;

#define K_OP_LEER (0x00)
#define K_OP_ACK (0x05)

//An Tutoriums-Datei dist.ccp angelehnt
struct paket
{
    uint32_t id;
    uint8_t opcode;
    uint8_t sender;
    uint8_t receiver;

    uint32_t xpos;
    uint32_t ypos;
    uint8_t color;

    // must implement '=' '==' and '<<' (nice would be 'sc_trace')

    paket(uint8_t opcode, uint8_t sender, uint8_t receiver, uint32_t xpos, uint32_t ypos, uint8_t color) :
      id(0), opcode(opcode), sender(sender), receiver(receiver), xpos(xpos), ypos(ypos), color(color)
    {
      id |= (uint32_t)opcode | ((uint32_t)sender << 8u) | ((uint32_t)receiver << 16u);
    }

    paket() :
      id(0), opcode(0), sender(0), receiver(0), xpos(0), ypos(0), color(0)
    {
    }

    inline bool operator==(const paket &rhs) const
    {
      return (rhs.id == id && rhs.xpos == xpos && rhs.ypos == ypos && rhs.color == color);
    }

    inline paket &operator=(const paket &rhs)
    {
      id = rhs.id;
      opcode = rhs.opcode;
      sender = rhs.sender;
      receiver = rhs.receiver;
      xpos = rhs.xpos;
      ypos = rhs.ypos;
      color = rhs.color;
      return *this;
    }

    inline friend void sc_trace(sc_trace_file *tf, const paket &v,
        const std::string &NAME)
    {
      sc_trace(tf, v.id, NAME + ".id");
      sc_trace(tf, v.opcode, NAME + ".opcode");
      sc_trace(tf, v.sender, NAME + ".sender");
      sc_trace(tf, v.receiver, NAME + ".receiver");
      sc_trace(tf, v.xpos, NAME + ".xpos");
      sc_trace(tf, v.ypos, NAME + ".ypos");
      sc_trace(tf, v.color, NAME + ".color");
    }

    inline friend ostream &operator<<(ostream &os, paket const &v)
    {
      os << "(" << v.id << "," << v.opcode << "," << v.sender << "," << v.receiver << "," << v.xpos << "," << v.ypos << "," << v.color << ")";
      return os;
    }
};


#endif /* PAKET_H_ */

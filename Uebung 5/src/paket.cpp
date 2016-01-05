/*
 * paket.cpp
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
 *
 * Ein Paket kann dabei in unterschiedlichen Varianten genutzt werden [OPCODE]:
 *
 * 1. Als Berechnungsauftrag des Gateway-Moduls für ein Compute-Modul [exe] 0x00
 * 2. Als Bestätigung des Compute-Moduls, dass die Berechnung durchgeführt wurde an das Gateway-Modul [fin] 0x01
 *
 * 3. Als Empfangsanfrage für ein Paket vom Compute-Modul an den Cache [c_req] (Cache-request) 0x02
 * 4  Als Empfangsanfrage für ein Paket vom Cache-Modul an das RAM-Modul [r_req] (RAM-request) 0x03
 *
 * 5. Als Empfangsbestätigung für ein Paket mit der betreffenden Paket-ID [ack] (acknowledge) 0x04
 *
 * 6. Als Informationsträger aufgrund einer Empfangsanfrage an den Cache-Modul vom Compute-Modul  [ic_pay] 0x05
 * 7. Als Informationsträger aufgrund einer Empfangsanfrage an das RAM-Modul vom Cache-Modul [ir_pay] 0x06
 *
 * 8. Als Ergebnis einer Berechnung des Compute-Moduls an das Gateway-Modul [o_pay] 0x07
 *
 * 9. Als Aufforderung des Gateway-Moduls an das RAM-Modul den Inhalt der Bilddatei einzulesen [rfi] 0x08
 *
 * 10. Als Information, des RAM-Moduls, an das Gateway-Modul, das das einlesen beendet ist [rff] 0x09
 *
 * 11. Als Aufforderung des Gateway-Moduls an das RAM-Modul den Inhalt der zweiten Speicherbank
 *     in die Datei 'noc.pgm' zu schreiben [wfi] 0x10
 *
 * 12. Als Information, des RAM-Moduls, an das Gateway-Modul, das das schreiben beendet ist [wff] 0x11
 *
 * Da nicht sichergestellt werden kann, dass das Paket empfangen wird [Buffer des Empfängers ist voll],
 * müssen wir hier mit einer Empfangsbestätigung arbeiten. Die Empfangsbestätigung muss über eine eindeutige
 * Identifikation verfügen, da sonst nicht zugeordnet werden kann, welches Acknowledge welchem Paket zugeordnet
 * werden kann. Auf die empfangsbrestätigung selber wird keine neue Empfangsbestätigung gesendet.
 *
 * Die ID ist ein HASH-Wert, der aus OPCODE, SENDER und RECEIVER gebildet wird. Empfangsbestätigungen werden immer
 * mit dem gleichen HASH-Wert gesendet, wie die Pakete die empfangen wurden.
 */

#include "paket.h"

#include <systemc.h>

paket::paket()
{
  // TODO Auto-generated constructor stub

}

paket::~paket()
{
  // TODO Auto-generated destructor stub
}

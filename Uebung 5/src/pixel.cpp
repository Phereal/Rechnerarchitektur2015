/*
 * paket.cpp
 *
 * Ein Paket dient als eine Kommunikationseinheit zwischen den einzelnen Routern bzw. Modulen und
 * beinhaltet dabei neben Statusinformationen [OPCODE; SENDER; RECEIVER; PAYLOAD] ggf. auch einen
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
 * 1. Als Berechnungsauftrag des Gateway-Moduls für ein Compute-Modul [exe] 0x00
 * 2. Als Bestätigung des Compute-Moduls, dass die Berechnung durchgeführt wurde [fin] 0x01
 * 3. Als Empfangsanfrage für ein Paket [req] (request) 0x02
 * 2. Als Empfangsbestätigung für ein Paket [ack] (acknowledge) 0x03
 * 3. Als Informationsträger aufgrund einer Empfangsanfrage [i_pay] 0x04
 * 4. oder als Ergebnis einer Berechnung [o_pay] 0x05
 *
 * Da nicht sichergestellt werden kann, dass das Paket empfangen wird [Buffer des Empfängers ist voll],
 * müssen wir hier mit einer Empfangsbestätigung arbeiten.
 */

#include "paket.h"

paket::paket()
{
  // TODO Auto-generated constructor stub

}

paket::~paket()
{
  // TODO Auto-generated destructor stub
}

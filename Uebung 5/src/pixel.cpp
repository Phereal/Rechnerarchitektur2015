/*
 * paket.cpp
 *
 * Ein Paket dient als eine Kommunikationseinheit zwischen den einzelnen Routern bzw. Modulen und
 * beinhaltet dabei neben Statusinformationen ggf. auch einen Payload.
 *
 * Bei dem Payload handelt es sich hier laut Aufgabenstellung um einen Pixel.
 * Jeder Pixel verfügt über eine X- bzw. Y Position auf dem Bild. Diese müssen auch innerhalb des
 * Paketes zur Bestimmung des des Payloads gespeichert werden, dazu werden die Variablen [xpos] und [ypos] genutzt.
 *
 * Jeder Pixel kann einen Wert zwischen 0 und 255 annehmen. Es gibt hierbei keine Über- oder Unterläufe.
 * Dies bedeutet, dass 255 + 1 = 255 ist und 0 - 1 = 0 ist.
 *
 * Ein Paket hat immer einen Senderouter [sender] und einen Empfangsrouter [receiver], damit die Pakete
 * adressiert werden können.
 *
 * Ein Paket kann dabei in unterschiedlichen Varianten genutzt werden:
 * 1. Als Empfangsanfrage für eine Information [req] (request) [pay] = null
 * 2. Als Empfangsbestätigung für eine Information [ack] (acknowledge) [pay] = null
 * 3. Als Informationsträger aufgrund einer Empfangsanfrage [pay] (payload) und [new] = false
 * 4. oder als Ergebnis einer Berechnung [pay] (payload) und [new] = true
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

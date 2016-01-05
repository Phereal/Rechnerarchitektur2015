/*
 * gateway.cpp
 *
 * erbt von module.cpp
 *
 * Das Gateway-Modul steuert die Compute-Module und verteilt die Berechnungen auf sie. Dabei werden in einem Buffer
 * immer die Adressen der nächsten zu berechnenden Pixel vorgehalten. Wenn ein Pixel fertig berechnet ist, wird
 * wird dieser aus dem Buffer enfernt und eine Anfrage an das RAM-Modul gestellt und dieses sendet die Adresse des
 * nächsten nicht bearbeiteten Pixels.
 * Dabei werden alle Pixeladressen die bereits berechnet werden mit einem Flag im Buffer markiert, damit sie nicht doppelt
 * berechnet werden.
 *
*/

#include "gateway.h"

#include <systemc.h>

gateway::gateway()
{
  // TODO Auto-generated constructor stub

}

gateway::~gateway()
{
  // TODO Auto-generated destructor stub
}

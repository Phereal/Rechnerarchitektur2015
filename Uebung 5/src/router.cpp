/*
 * router.cpp
 *
 * Ein Router stellt die grundsätzliche Kommunikationsfähigkeit des NOC bereit.
 *
 * Er kann:
 * 1. Pakete von anderen Routern oder seinem verbundenen Modul empfangen [receive] und im Empfangsbuffer zwischenspeichern.
 *    Es werden immer alle ankommenden Pakete im Empfangsbuffer gespeichert.
 *    Falls mehrere Pakete auf verschiedenen Leitungen ankommen, wird das Paket zuerst in den Buffer gespeichert,
 *    das an der Leitung liegt die zuerst abgefragt wird [i_buffer].
 * 2. Die Pakete aus dem Empfangsbuffer den verschiedenen SendeBuffern (für jede Leitung ein Buffer, kleiner als
 *    EmpfangsBuffer) zuordnen.
 * 3. Pakete auf Grundlage der eigenen Routing-Tabelle aus den Sendebuffern [ox_buffer[ (x für Ident der Leitung)
 *    auf mehreren Sendeleitungen auf die steigende Taktflanke gleichzeitig zustellen [send].
 *
 * Dabei liest der Router nur die Empfangsadresse aus und führt auf dieser Grundlage anhand der Routing Tablle eine Operation aus.
 * Er sendet selbst keine Empfangsbestätigung. Dies ist den betreffenden Modulen überlassen. Es handelt sich sozusagen um ein passives 'Gerät',
 * das nur empfangen, zwischenspeichern und senden kann.
 *
 * Die Adressierung der einzelnen Router-Knoten ergibt sich aus den Array Positionen,
 * daraus ergibt sich auch, welches Modul durch den jeweiligen Router-Knoten erzeugt wird.
 *
 * RAM######CP#######CP#######CP#######CP#######CP#######CP#######CP######
 * ([0][0]) ([0][1]) ([0][2]) ([0][3]) ([0][4]) ([0][5]) ([0][6]) ([0][7])
 * CP#######CP#######CASH#####CP#######CP#######CASH#####CP#######CP######
 * ([1][0]) ([1][1]) ([1][2]) ([1][3]) ([1][4]) ([1][5]) ([1][6]) ([1][7])
 * CP#######CP#######CP#######CP#######CP#######CP#######CP#######CP######
 * ([2][0]) ([2][1]) ([2][2]) ([2][3]) ([2][4]) ([2][5]) ([2][6]) ([2][7])
 * CP#######CP#######CASH#####CP#######CP#######CASH#####CP#######CP######
 * ([3][0]) ([3][1]) ([3][2]) ([3][3]) ([3][4]) ([3][5]) ([3][6]) ([3][7])
 * CP#######CP#######CP#######CP#######CP#######CP#######CP#######CP######
 * ([4][0]) ([4][1]) ([4][2]) ([4][3]) ([4][4]) ([4][5]) ([4][6]) ([4][7])
 * CP#######CP#######CASH#####CP#######CP#######CASH#####CP#######CP######
 * ([5][0]) ([5][1]) ([5][2]) ([5][3]) ([5][4]) ([5][5]) ([5][6]) ([5][7])
 * CP#######CP#######CP#######CP#######CP#######CP#######CP#######CP######
 * ([6][0]) ([6][1]) ([6][2]) ([6][3]) ([6][4]) ([6][5]) ([6][6]) ([6][7])
 * CP#######CP#######CASH#####CP#######CP#######CASH#####CP#######GATEWAY#
 * ([7][0]) ([7][1]) ([7][2]) ([7][3]) ([7][4]) ([7][5]) ([7][6]) ([7][7])
 *
 * Der Gateway wurde als letztes Element im Array gewählt, weil er den Compute-Modulen mit geringem Index zuerst ihre Aufgaben zuweist und diese
 * anschließend mit dem fetchen der Bildinformationen aus dem Cash beschäftigt sind und somit Aufträge eventuell nicht direkt weiterleiten könnten.
 * Auf dieser Basis wurde auch der RAM positioniert, da die Compute-Elemente, die bereits ihren Auftrag haben ihre Informationen aus den nächsten
 * Cache beziehen.
 *
 * Die schönste Möglichkeit, wäre es den RAM direkt in der Mitte der Matrix zu platzieren, dies ist allerdings durch die Aufgabenstellung untersagt.
 *
 * Der eingesetzte Routing-Algorithmus verteilt die Pakete dabei auf Grundlage ihm bekannten Routing-Tabelle
 * (der kürztmöglichste Weg zum Ziel) in den entsprechenden SendeBuffer.
 *
 * Wenn die Empfangsadresse des Paketes im Empfangsbuffer mit der eigenen Adresse übereinstimmt, muss das Paket an das Verbundene Modul
 * gesendet werden.
 *
 * Die Verbundenen Module werden entsprechen der oberen Matrix hier im Konstruktor der Klasse erzeugt.
 */

#include "router.h"
#include "paket.h"

#include <systemc.h>

router::router()
{
  // TODO Auto-generated constructor stub

}

router::~router()
{
  // TODO Auto-generated destructor stub
}



producer::producer(sc_module_name name, int genSpeed, int maxAddressCnt) :
    sc_module(name), genSpeed(genSpeed), maxAddressCnt(maxAddressCnt)
{
  SC_HAS_PROCESS(producer);
  SC_METHOD(sendPackageRandomly);
  sensitive << clk.pos();

  //SC_METHOD(ackDataReceived);
  //sensitive << dataReceived;
}

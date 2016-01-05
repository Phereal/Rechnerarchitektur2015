/*
 * router.cpp
 *
 * Ein Router stellt die grundsätzliche Kommunikationsfähigkeit des NOC bereit.
 *
 * Er kann:
 * 1. Pakete von anderen Routern oder seinem verbundenen Modul empfangen [receive]
 * 2. Pakete in einem eigenen Buffer zwischenspeichern, falls mehrere Pakete auf verschiedenen Leitungen ankommen,
 *    die nicht gleichzeitig durch das Modul verarbeitet werden könnnen [buffer]
 * 3. Pakete an sein verbundenes Modul weiterleiten
 * 4. Pakete direkt, auf Grundlage seines Routing-Tables, weiterleiten [forward]
 *
 * Pakete für sein Modul anfordern [request]
 *
 * bereitstellen und auch anliegende Pakete von seinem Modul an andere Router senden.
 *
 * Wenn ein Router nicht Empfänger eines Paketes ist und das eigene Modul keine Pakete
 * senden möchte, kann das empfangene Paket direkt weitergeleitet werden, selbst wenn sich
 * bereits Pakete im Buffer befinden.
 *
 * Es kann pro Taktzyklus immer nur ein Paket gesendet werden, selbst wenn mehrere
 */

#include "router.h"

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

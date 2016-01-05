/*
 * router.cpp
 *
 * Ein Router stellt die grundsätzliche Kommunikationsfähigkeit des NOC bereit.
 *
 * Er kann:
 * 1. Pakete von anderen Routern oder seinem verbundenen Modul empfangen [receive] und im Empfangsbuffer zwischenspeichern,
 *    falls mehrere Pakete auf verschiedenen Leitungen ankommen [i_buffer].
 * 2. Pakete aus dem Empfangsspeicher [i_buffer] in den Sendebuffer [o_buffer] verschieben.
 * 3. Pakete auf Grundlage der eigenen Routing-Tabelle aus dem Sendebuffer auf mehrere Leitungen gleichzeitig zustellen
 *    [send]
 * 4. Pakete im Senderbuffer zwischenspeichern, falls gerade auf der betreffenden Leitung gesendet wird [o_buffer]
 *
 * Dabei liest der Router nur die Empfangsadresse aus und führt auf diese Grundlage eine Operation aus. Er sendet selbst keine
 * Empfangsbestätigung. Dies ist den betreffenden Modulen überlassen. Es handelt sich sozusagen um ein passives 'Gerät'.
 *
 * Wenn Caches umgesetzt werden, erhält der Router als Informationsquelle in seinem Routing-Table nicht die Adresse des RAM-Moduls,
 * sondern die Adresse des nächstgelegenen Caches. Es ist Aufgabe des Caches, die Informationen aus dem RAM zu erhalten und im eigenen
 * Speicher vorzuhalten.
 *
 *
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

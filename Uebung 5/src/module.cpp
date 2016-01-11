/*
 * module.cpp
 *
 */

#include "module.h"

module::module(sc_module_name name, uint8_t id, uint32_t bufferSize) :
    sc_module(name), id(id), bufferSize(bufferSize)
{
  if(bufferSize == 0)
  {
    // todo: ggf. andere Fehlerbehandlung oder Error throw
    this->bufferSize = 5;
  }

  sendeBuffer = new PaketBuffer(this->bufferSize);
}

void module::receive()
{
  // Pruefe ob ein Paket am Eingang anliegt (opcode 0x00 == leeres Paket)
  if(routerIn.read().opcode != (uint8_t)K_OP_LEER)
  {

    // Paket liegt an, pruefe ob Empfangsbestaetigung
    if(routerIn.read().opcode == (uint8_t)K_OP_ACK)
    {
      // Empfangsbestaetigung
      // todo erstmal nicht implementiert
    }
    else
    {
      // Normales Paket, verarbeite es in process und sende empfangsbestaetigung
      paket tmpPkg = routerIn.read();
      // todo: erstmal kein ack
      //createAck(routerIn.read());
      if( process(tmpPkg) )
      {
        sendeBuffer->push( tmpPkg );
      }
    }
  }
}

void module::send()
{
  paket tmpPaket;
  paket leeresPaket;

  // Pruefe den SendeBuffer auf Pakete
  if(sendeBuffer->pop(tmpPaket))
  {
    // Paket aus dem Buffer in tmpPaket gespeichert, sende es
    routerOut.write(tmpPaket);
  }
  else
  {
    // kein Paket im Buffer, lege leeres Paket an den Ausgang
    routerOut.write(leeresPaket);
  }
}

void module::createAck(paket pkg)
{
  uint8_t tmp;

  tmp = pkg.receiver;
  pkg.receiver = pkg.sender;
  pkg.sender = tmp;
  pkg.opcode = (uint8_t)K_OP_ACK;

  sendeBuffer->push( pkg );
}

//module::module(sc_module_name name, int sendBufferSize) :
//    sc_module(name), bufferSize(bufferSize)
//    {
//    //Anbindung zum Router
//    sc_in < paket > routerIn;
//    sc_out < paket > routerOut;
//
//    //Puffer als Vektor (für einfache Elementeinfügung und FIFO-ähnliches Verhalten)
//    std::vector < paket > sendBuffer;
//    //Zähler für gesendete Pakete.
//    pair<*paket, int> pendingPaketList[bufferSize];
//
//    SC_HAS_PROCESS( module);
//    SC_METHOD    (refresh)
//    sensitive << clk.pos();
//
//    // TODO Auto-generated constructor stub
//
//    }
//
//module::~module()
//    {
//    // TODO Auto-generated destructor stub
//    }
//
///*
// * Wird jeden Clock ausgeführt.
// * -Das älteste, nicht gesendete Paket aus dem sendBuffer entfernen und an Router übermitteln.
// * -Das dazugehörige gesendete Paket aus dem sendBuffer entfernen, wenn die Sendebestätigung an routerIn angliegt.
// */
//void module::refresh()
//    {
//
//    /*Wenn Pakete fertig sind folgendes implementieren:
//     *1.: 	Überprüfe, ob eine Sendebestätigung eingetroffen ist.
//     *	Wenn ja, entferne das zugehörige Paket aus dem Puffer, sodass andere Pakete nachrücken.
//     *2.:	Erhöhe den Antwortzeit-zähler aller gesendeten Pakete im Puffer.
//     *	Überprüfe, ob Elemente bereits länger als 100 Zyklen auf Bestätigung warten.
//     *	Wenn ja, entferne deren sent-Flag, damit diese erneut gesendet werden.
//     *3.:	Sende das älteste Paket ohne sent-Flag.
//     */
//
//    //Empfangenes Paket auf Sendebestätigung überprüfen
//    bool confirmationReceived; // = Paket-Bedingung, die es als Bestätigung markiert
//
//    if (confirmationReceived)
//	{
//	//Zugehöriges Paket aus Sende-Puffer entfernen.
//	//TODO Fertig stellen, wenn Paket fertig ist.
//	}
//    else
//	{
//	//Führe Modul-Spezifischen compute out.
//	compute();
//	}
//
//    //Nun müssen die Antwortzeit aller gesendeten und noch nicht angekommenen Pakete verarbeitet werden.
//    for (int i = 0; i < pendingPaketList.size(); i++)
//	{
//	pendingPaketList[i].second++;
//
//	if (pendingPaketList[i].second > 99)
//	    {
//	    //sent-Flag von dem Paket entfernen, damit es im nächste Paket nachrücken kann.
//	    //TODO Fertig stellen, wenn Paket fertig ist.
//
//	    }
//	}
//
//    //Zuletzt muss so lange durch den sende-Puffer iteriert werden, bis ein Element ohne sent-Flag gefunden wurde.
//    //Sende dieses Paket an routerOut.
//    int bufferIterator = 0;
//    while (true)
//	{
//	//TODO Fertig stellen, wenn Paket fertig ist.
//	break;
//	}
//
//    return 0;
//    }
///*
// * In dieser Klasse lediglich ein Platzhalter.
// * In Unterklassen für den Umgang mit Paketen in routerIn verantwortlich.
// */
//void module::compute()
//    {
//    cout << "Eltern-compute() wurde aufgerufen.";
//    return 0;
//    }


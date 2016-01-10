/*
 * module.cpp
 *
 * Die Klasse "module" stellt eine abstrakte Klasse dar, die die Methoden zur Kommunikation für Gateway, RAM, Cache und
 * Compute Klassen bereitstellt. Die genannten Klassen erben von der Klasse "module" um ebenfalls über die Methoden
 * zu verfügen.
 *
 * Ein Modul kann:
 *
 * 1. Ein Paket empfangen
 * 2. Zu sendende Pakete im Sendebuffer vorhalten
 * 3. Ein Paket aus dem Sendebuffer senden und mit einem sent Flag (gesendet Flag) markieren
 * 4. Auf die Empfangsbestätigung eines gesendeten Paketes warten
 *    Wenn nach 100 Zyklen keine Empfangsbesätigung eigetroffen ist, das Paket erneut senden.
 *    (nach 15 Taktzyklen kann jedes Paket theoretisch zugestellt werden und die Empfangsbestätigung anliegen,
 *    durch die verschidenen Sende- und Empfangsbuffer muss hier allerdings eine größere Zeit gewählt werden!)
 *    Sobald die Empfangsbestätigung eingetroffen ist, wird das betreffende Sendepaket gelöscht.
 * 5. Aufgrund einer ID-Liste bereits empfangener Pakete entscheiden, ob das betreffende Paket bereits empfangen wurde.
 *    In diesem Fall wird das empfagenene Paket verworfen (Falls eine Empfangsbestätigung auf dem Übertragungsweg
 *    verloren gegangen ist).
 *
 */

#include "module.h"
#include "paket.h"

#include <vector>

#include <systemc.h>

module::module(sc_module_name name, int sendBufferSize) :
    sc_module(name), bufferSize(bufferSize)
    {
    //Anbindung zum Router
    sc_in < paket > routerIn;
    sc_out < paket > routerOut;

    //Puffer als Vektor (für einfache Elementeinfügung und FIFO-ähnliches Verhalten)
    std::vector < packet > sendBuffer;
    //Zähler für gesendete Pakete.
    pair<*paket, int> pendingPaketList[bufferSize];

    SC_HAS_PROCESS( module);
    SC_METHOD    (refresh)
    sensitive << clk.pos();

    // TODO Auto-generated constructor stub

    }

module::~module()
    {
    // TODO Auto-generated destructor stub
    }

/*
 * Wird jeden Clock ausgeführt.
 * -Das älteste, nicht gesendete Paket aus dem sendBuffer entfernen und an Router übermitteln.
 * -Das dazugehörige gesendete Paket aus dem sendBuffer entfernen, wenn die Sendebestätigung an routerIn angliegt.
 */
void module::refresh()
    {

    /*Wenn Pakete fertig sind folgendes implementieren:
     *1.: 	Überprüfe, ob eine Sendebestätigung eingetroffen ist.
     *	Wenn ja, entferne das zugehörige Paket aus dem Puffer, sodass andere Pakete nachrücken.
     *2.:	Erhöhe den Antwortzeit-zähler aller gesendeten Pakete im Puffer.
     *	Überprüfe, ob Elemente bereits länger als 100 Zyklen auf Bestätigung warten.
     *	Wenn ja, entferne deren sent-Flag, damit diese erneut gesendet werden.
     *3.:	Sende das älteste Paket ohne sent-Flag.
     */

    //Empfangenes Paket auf Sendebestätigung überprüfen
    bool confirmationReceived; // = Paket-Bedingung, die es als Bestätigung markiert

    if (confirmationReceived)
	{
	//Zugehöriges Paket aus Sende-Puffer entfernen.
	//TODO Fertig stellen, wenn Paket fertig ist.
	}
    else
	{
	//Führe Modul-Spezifischen compute out.
	compute();
	}

    //Nun müssen die Antwortzeit aller gesendeten und noch nicht angekommenen Pakete verarbeitet werden.
    for (int i = 0; i < pendingPaketList.size(); i++)
	{
	pendingPaketList[i].second++;

	if (pendingPaketList[i].second > 99)
	    {
	    //sent-Flag von dem Paket entfernen, damit es im nächste Paket nachrücken kann.
	    //TODO Fertig stellen, wenn Paket fertig ist.

	    }
	}

    //Zuletzt muss so lange durch den sende-Puffer iteriert werden, bis ein Element ohne sent-Flag gefunden wurde.
    //Sende dieses Paket an routerOut.
    int bufferIterator = 0;
    while (true)
	{
	//TODO Fertig stellen, wenn Paket fertig ist.
	break;
	}

    return 0;
    }
/*
 * In dieser Klasse lediglich ein Platzhalter.
 * In Unterklassen für den Umgang mit Paketen in routerIn verantwortlich.
 */
void module::compute()
    {
    cout << "Eltern-compute() wurde aufgerufen.";
    return 0;
    }


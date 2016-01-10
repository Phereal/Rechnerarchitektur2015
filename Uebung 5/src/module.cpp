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
    sc_in<paket> routerIn;
    sc_out<paket> routerOut;

    //Puffer als Vektor (für einfache Elementeinfügung )
    std::vector<packet> sendBuffer = {};
    //Zähler für gesendete Pakete.
    std::map<paket, int> sentPaketResponseTimesList;

    SC_HAS_PROCESS(module);
    SC_METHOD(refresh)
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
void module:: refresh()
    {
    //1 Paket senden
    int bufferIterator = 0;
    while(true)
	{
	//Wenn Pakete fertig sind folgendes implementieren:
	//1.: Wähle (wenn vorhanden) das erste, nicht gesendete Paket aus.
	//2.: Gib dieses Element an routerOut weiter.
	//3.: Entferne das Element und lass hintere Elemente nachrücken
	break;
	}

    //Empfangenes Paket auf Sendebestätigung überprüfen
    bool confirmationReceived; // = Paket-Bedingung, die es als Bestätigung markiert

    if (confirmationReceived)
	{
	//Zugehöriges Paket aus Sende-Puffer entfernen.
	}
    else
	{
	//Führe Modul-Spezifischen compute out.
	compute();
	}

    //Zuletzt müssen die Antwortzeit aller gesendeten und noch nicht angekommenen Pakete verarbeitet werden.
    for (int i = 0; i<sentPaketResponseTimesList.size();i++)
	{

	}
    return 0;
    }
/*
 * In dieser Klasse lediglich ein Platzhalter.
 * In Unterklassen für den Umgang mit Paketen in routerIn verantwortlich.
 */
void module:: compute()
    {
    cout << "Eltern-compute() wurde aufgerufen.";
    return 0;
    }



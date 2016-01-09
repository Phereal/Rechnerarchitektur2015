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

#include <systemc.h>

module::module(sc_module_name name, int sendBufferSize) :
    sc_module(name), bufferSize(bufferSize)
    {
    //Anbindung zum Router
    sc_in<paket> routerIn;
    sc_out<paket> routerOut;

    sendBuffer = new packet[sendBufferSize];

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

    }

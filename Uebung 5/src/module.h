/*
 * module.h
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

#ifndef MODULE_H_
#define MODULE_H_

#include <cstdint>

#include <systemc.h>

#include "paket.h"
#include "PaketBuffer.h"

SC_MODULE(module)
{
    // inputs
    sc_in<bool> clk;

    // interface to router
    sc_in<paket> routerIn;
    sc_out<paket> routerOut;

    // Constructor
    module(sc_module_name name, uint8_t id, uint32_t bufferSize);

    virtual void receive() final;
    virtual void send() final;

    // private Hilfsvariablen
  private:
    uint8_t id;
    uint32_t bufferSize;
    PaketBuffer *sendeBuffer;

    virtual void createAck(paket pkg) final; // erzeugt ack paket und legt es in den sendebuffer
    virtual bool process(paket &pkg) = 0;
};

#endif /* MODULE_H_ */

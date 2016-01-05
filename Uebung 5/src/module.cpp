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
 *
 *  * Wenn Caches umgesetzt werden, erhält der Router als Informationsquelle in seinem Routing-Table nicht die Adresse des RAM-Moduls,
 * sondern die Adresse des nächstgelegenen Caches. Es ist Aufgabe des Caches, die Informationen aus dem RAM zu erhalten und im eigenen
 * Speicher vorzuhalten.
 */

#include "module.h"
#include "paket.h"

#include <systemc.h>

module::module()
{
  // TODO Auto-generated constructor stub

}

module::~module()
{
  // TODO Auto-generated destructor stub
}

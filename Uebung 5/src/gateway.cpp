/*
 * gateway.cpp
 *
 * erbt von module.cpp
 *
 * Das Gateway-Modul steuert die Compute-Module und verteilt die Berechnungen auf sie.
 * Außerdem startet es die Berechnung und beendert das Programm, wenn die Berechnung abgeschlossen ist.
 *
 * Das Gateway Modul sendet beim Programmstart ein Paket mit dem Inhalt OPCODE = [rfi] an das RAM Modul, woraufhin
 * das RAM das externe Bild einliest und lokal speichert. Anschließend [neben den Empfangsbestätigungen]
 * erhält der Gateway eine Benachrichtigung vom RAM-Modul OPCODE = [rff], dass der einleseprozess beendet ist.
 *
 * Um die Berechnungen zu starten, muss der Gateway wissen, welche Berechnungen durchgeführt werden müssen und
 * erhält diese Information ebenfalls vom RAM. Er sendet ein Paket mit dem Inhalt OPCODE = [nxt] am das RAM-Modul.
 *
 * Das RAM untersucht daraufhin im Bildspeicher, welche Pixel bereits bearbeitet wurden und sendet die
 * Koordinaten des nächsten nicht bearbeiteteten Pixels an das Gateway mit dem Inhalt OPCODE = [nxa].
 * sollten in diesem Fall alle Pixel bereits berechnet sein, sende das RAM-Modul ein Paket mit dem
 * Inhalt OPCODE = [end]. Daraufhin werden keine neuen Berechnungsanfragen mehr an das RAM-Modul gestellt.
 *
 * In einem internen Speicher hält das Gateway immer eine definierte Anzahl an zu berechnenden Pixeln vor, damit
 * bei Abschluss einer Berechnung Zeit eingespart werden kann und das Compute-Modul direkt mit dem nächsten
 * Auftrag beginnen kann. Wenn ein Berechnungsauftrag erteilt wird, wird dies in diesem Speicher markiert und
 * der Auftrag wird erst nach Abschluss der Berechnung gelöscht. Der Speicher sollte also mindestens doppelt
 * so grpß sein, wie die Anzahl der Compute-Module.
 *
 * Das Gateway-Modul hat eine Liste der Compute-Module mit Adresse und Zustand der einzelnen Compute-Module.
 * Initial sind alle Compute-Module im Zustand "free". Wenn der Gateway ihnen einen Berechnungsauftrag erteilt,
 * mit dem Inahlt OPCODE = [exe] wechseln sie in den Zustand "busy".
 * Wenn der Gateway das fertig berechnete Paket erhält mit dem Inhalt OPCODE = [fin], wird wieder in den Zustand
 * "free" gewechselt. Somit kann der Gateway dem compute-Modul den nächsten Berechnungsauftrag zuweisen.
 *
 * Wenn die Liste der zu berechnenden Pixel leer ist und ein Flag gesetzt ist, das signalisiert, dass mindestens eine
 * Berechnung durchgeführt wurde (ein berechneter Pixel wurde erhalten) sind alle Pixel berechnet und
 * das Gateway-Modul kann das Rückschreiben des Bildes auf die Festplatte anstoßen.
 *
 * Dazu sendet es ein Paket mit dem Inhalt OPCODE = [wfi] an das RAM-Modul, welches dann beginnt, den Inhalt
 * der zweiten Speicherbank auf die Festplatte zu schreiben. Im weiteren Verlauf, wartet das Gateway-Modul auf
 * eine Bestätigung des RAM-Moduls, dass das schrieben abgeschlossen ist, mit dem Inhalt OPCODE = [wff].
 *
 * Wenn der Schreibvorgang abgeschlossen ist, werden alle internen Speicherelemente neu initialisiert und
 * das Programm wird beendet.
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

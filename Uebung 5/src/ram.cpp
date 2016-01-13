/*
 * ram.cpp
 *
 * erbt von module.cpp
 *
 * Das RAM-Modul stellt den lokalen Speicher des NOC dar, in dem das Originalbild bzw. das Zielbild abgelegt wird.
 * Zusätzlich verfügt die Klasse über Methoden zum einlesen und schreiben von PGM-Dateien.
 *
 * Für das einlesen und auch später das schreiben des Bildes von/in eine PGM-Datei ist die aus der Aufgabenstellung
 * vorgegegene Klasse pgm.cpp vorhanden.
 * Die Methoden read und write der Klasse pgm.cpp verwenden die variablen [xpos,ypos] sowie die Pfadangaben für das Quell- bzw.
 * Zielbild [INFILE,OUTFILE].
 * Die Farbgebung des Pixels basiert auf einem Zahlenwert der in dieser Aufgabe einen Wert zwischen 0 und 255 annehmen kann [COLOR_DEPTH].
 * Die Methode writePixel() stellt sicher, dass von dieser Farbtiefe nicht abgewichen werden kann. Mit dieser Methode schreibt man
 * Die Methoden readPGM und writePGM greifen auf die Methoden der Klasse pgm.h zu und speichern arbeiten jeweils mit den lokalen
 * Vektoren in_image[][] und out_image[][]. In diesen beiden Vektoren des Typs unsigned char werden die einzelnen Pixel gespeichert.
 *
 * Ein weiter Vektor des Typs Bool, speichert, ob der betreffende Pixel sich bereits im Zielbild befindet.
 * Somit kann über diesen Vektor abgefragt werden, welcher Pixel als nächstes berechnet werden muss.
 */

#include "ram.h"

using namespace std;

ram::ram(sc_module_name name, uint8_t id, uint32_t bufferSize) : module(name, id, bufferSize)
{
  if(initialize == false) {
    init();
    initialize = true;
  }
  pakethandler();
  //Die Clock des RAM soll 2 mal langsamer sein, als die anderen Clocks!
  sensitive << clk.pos();

  //SC_HAS_PROCESS(ram);
}
//Überschreiben der process-Methode der Elternklasse Module
//TODO Wird immer nur aufgerufen, wenn ein Paket auch eigeht, somit kann momentan nur bei eingehenden Paketen gesendet werden!
bool ram::process(paket &pkg){
  enable = false;
  cout << "[RAM] Beginne einlesen des Paketes!" << endl;
  //Paket einlesen
  i_id = pkg.id;
  i_opcode = pkg.opcode;
  i_sender = pkg.sender;
  i_receiver = pkg.receiver;
  i_xpos = pkg.xpos;
  i_ypos = pkg.ypos;
  i_color = pkg.color;
  cout << "[RAM] Beende einlesen des Paketes!" << endl;

  //Paket senden
  cout << "[RAM] Beginne senden des Paketes!" << endl;
  pkg.id = o_id;
  pkg.opcode = o_opcode;
  pkg.sender = o_sender;
  pkg.receiver = o_receiver;
  pkg.xpos = o_xpos;
  pkg.ypos = o_ypos;
  pkg.color = o_color;
  cout << "[RAM] Beende senden des Paketes!" << endl;
        return enable;
}

void ram::pakethandler() {
  //Es werden nur OPCodes behandelt, die auch durch das RAM-Modul verarbeitet werden können.
  //Die Paketbehandlung der gesendeten Module in den SendeBuffer wird in der Oberklasse durchgeführt.
  //Hier werden die Pakete direkt verarbeitet und dann über den SendeBuffer der Elternklasse
  //weiterverarbeitet.
  cout << "[RAM] Starte Pakethandler" << endl;
  switch(i_opcode){
    case 0x00: break;//[emp]
    case 0x01: break;//[exe]
    case 0x02: break;//[fin]
    case 0x03: break;//[c_req]
    case 0x04: //[r_req]
      cout << "[RAM] Starte Paketbehandlung eines RAM Requests" << endl;
      o_id = 0;
      o_opcode = 0x07; //[ir_pay]
      o_sender = i_receiver;
      o_receiver = i_sender;
      o_xpos = i_xpos;
      o_ypos = i_ypos;
      o_color = readPixel();
      enable = true;
      cout << "[RAM] Beende Paketbehandlung eines RAM Requests" << endl;
      cout << "[RAM] Sende ir_pay" << endl;
      break;
    case 0x05: break;//[ack]
    case 0x06: break;//[ic_pay]
    case 0x07: break;//[ir_pay]
    case 0x08: //[o_pay]
      cout << "[RAM] Starte Paketbehandlung einer Schreiboperation in den RAM" << endl;
      writePixel();
      cout << "[RAM] Beende Paketbehandlung einer Schreiboperation in den RAM" << endl;
      break;
    case 0x09: //[rfi]
      cout << "[RAM] Starte Paketbehandlung einlesen des Bildes in den RAM" << endl;
      readPGM();
      cout << "[RAM] Beende Paketbehandlung einlesen des Bildes in den RAM" << endl;
      o_id = 0;
      o_opcode = 0x0A; //[rff]
      o_sender = i_receiver;
      o_receiver = i_sender;
      o_xpos = 0;
      o_ypos = 0;
      o_color = 0;
      enable = true;
      cout << "[RAM] Sende rff" << endl;
      break;
    case 0x0A: break;//[rff]
    case 0x0B: //[wfi]
      cout << "[RAM] Starte Paketbehandlung schreiben des Bildes aus dem RAM" << endl;
      writePGM();
      cout << "[RAM] Beende Paketbehandlung schreiben des Bildes aus dem RAM" << endl;
      o_id = 0;
      o_opcode = 0x0C; //[wff]
      o_sender = i_receiver;
      o_receiver = i_sender;
      o_xpos = 0;
      o_ypos = 0;
      o_color = 0;
      enable = true;
      cout << "[RAM] Sende wff" << endl;
      break;
    case 0x0C: break;//[wff]
    case 0x0D: //[nxt]
      cout << "[RAM] Starte Paketbehandlung auf nächsten zu berechnenden Pixel!" << endl;
      if(nxtPixel()); // sendet [nxa] wenn vorhanden
      else {
        o_id = 0;
        o_opcode = 0x0F; //[end]
        o_sender = i_receiver;
        o_receiver = i_sender;
        o_xpos = 0;
        o_ypos = 0;
        o_color = 0;
        enable = true;
        cout << "[RAM] Sende end" << endl;
      }
      cout << "[RAM] Beende Paketbehandlung auf nächsten zu berechnenden Pixel!" << endl;
      break;
    case 0x0E: break;//[nxa]
    case 0x0F: break;//[end]
    case 0x10: //[brd]
      cout << "[RAM] Starte Paketbehandlung auf Übertragung der Bildgrenzen!" << endl;
      o_id = 0;
      o_opcode = 0x11; //[brr]
      o_sender = i_receiver;
      o_receiver = i_sender;
      o_xpos = width; //Nicht die Angabe der Pixelposition sondern die Rastergröße des Bildes wird übertragen!
      o_ypos = height;
      o_color = 0;
      enable = true;
      cout << "[RAM] Beende Paketbehandlung auf Übertragung der Bildgrenzen!" << endl;
      cout << "[RAM] Sende brr" << endl;
      break;
    case 0x11: break; //[brr]
    default: break;
  }
  cout << "[RAM] Beende Pakethandler" << endl;
}

// Wenn alle Pixel berechnet wurden wird anstatt eines [nxa] ein [end] zurückgeschickt!
bool ram::nxtPixel(){
  cout << "[RAM] Starte Untersuchung auf nächsten zu berechnenden Pixel!" << endl;
  for (unsigned int i = 0; i < i_ypos; i++){
    for (unsigned int j = 0; j < i_xpos; j++){
      if(nxt_map[i][j] == false){
        o_id = 0;
        o_opcode = 0x0E; //[nxa]
        o_sender = i_receiver;
        o_receiver = i_sender;
        o_xpos = i;
        o_ypos = j;
        o_color = 0;
        enable = true;
        cout << "[RAM] Beende Untersuchung auf nächsten zu berechnenden Pixel: Pixel verfügbar!" << endl;
        return true;
      }
    }
  }
  cout << "[RAM] Beende Untersuchung auf nächsten zu berechnenden Pixel: Alle Pixel berechnet!" << endl;
  return false; // Benachrichtigung des Gateway-Moduls, dass alle Pixel berechnet wurden.
}
// Initialisierung aller Vektoren mit Initialwerten
void ram::init(){
  cout << "[RAM] Starte Initialisierung des RAM!" << endl;
  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      in_image[i][j] = 0;
      out_image[i][j] = 0;
      nxt_map[i][j] = false;
    }
    cout << "[RAM] Initialisierung des RAM beendet!" << endl;
  }
}
// Der Wert eines Pixels wird zurückgegeben
unsigned char ram::readPixel() {
  cout << "[RAM] Einlesen eines Pixels aus dem RAM gestartet!" << endl;
  return in_image[i_xpos][i_ypos];
  cout << "[RAM] Einlesen eines Pixels aus dem RAM beendet" << endl;
}
// Ein Pixel im Zielbildvektor wird geschrieben mit Fehlerbehandlung für maximale Bildtiefe
void ram::writePixel(){
  cout << "[RAM] Schreiben eines Pixels in den RAM gestartet!" << endl;
  if(i_color < 0){
    i_color = 0;
  }
  if(i_color> depth){
    i_color = depth;
  }
  out_image[i_xpos][i_ypos] = i_color;
  nxt_map[i_xpos][i_ypos] = true;
  cout << "[RAM] Schreiben eines Pixels in den RAM beendet!" << endl;
}
// Das PGM-Bild wird über die Klasse pgm.h eingelesen
void ram::readPGM() {
    read_pgm(infile,width,height,depth,in_image);
    cout << "[RAM] Einlesen der Datei wurde gestartet!" << endl;
/*    for (int i = 0; i < heigth; i++){
      for (int j = 0; j < width; j++){
        cout << to_string(in_image[i][j]) << " ";
      }
      cout << "\n";
    }*/
    cout << "[RAM] Einlesen der Datei wurde beendet!" << endl;
}
// Das PGM-Bild wird über die Klasse pgm.h geschrieben
void ram::writePGM(){
  write_pgm(outfile,width,height,out_image,depth);
  cout << "[RAM] Schreiben der Ausgabedatei wurde gestartet!" << endl;
/*  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      cout << to_string(in_image[i][j]) << " ";
    }
    cout << "\n";
  }*/
  cout << "[RAM] Schreiben der Ausgabedatei wurde beendet!" << endl;
}

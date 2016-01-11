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
 * Die Methoden read und write der Klasse pgm.cpp verwenden die variablen [WIDTH,HEIGHT] sowie die Pfadangaben für das Quell- bzw.
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
  init();

  //SC_HAS_PROCESS(ram);
}
//Überschreiben der process-Methode der Elternklasse Module
bool ram::process(paket &pkg){
  bool processOk = false;

      return processOk;
}

// Wenn alle Pixel berechnet wurden werden die Koordinaten X=-1 und Y=-1 zurückgegeben.
void ram::nxtPixel(int &width, int &heigth){
  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      if(nxt_map[i][j] == false){
        width = i;
        heigth = j;
        break;
      }
      else {
        width = -1;
        heigth = -1;
      }
    }
  }
}
// Initialisierung aller Vektoren mit Initialwerten
void ram::init(){
  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      in_image[i][j] = 0;
      out_image[i][j] = 0;
      nxt_map[i][j] = false;
    }
  }
}
// Der Wert eines Pixels wird zurückgegeben
unsigned char ram::readPixel(int width, int heigth) {
  return in_image[width][heigth];
}
// Ein Pixel im Zielbildvektor wird geschrieben mit Fehlerbehandlung für maximale Bildtiefe
void ram::writePixel(int width, int heigth, unsigned char data){
  if(data < 0){
    data = 0;
  }
  if(data > depth){
    data = depth;
  }
  out_image[width][heigth] = data;
  nxt_map[width][heigth] = true;
}
// Das PGM-Bild wird über die Klasse pgm.h eingelesen
void ram::readPGM() {
    read_pgm(infile,width,height,depth,in_image);
    cout << "read" << endl;
/*    for (int i = 0; i < height; i++){
      for (int j = 0; j < width; j++){
        cout << to_string(in_image[i][j]) << " ";
      }
      cout << "\n";
    }*/
    cout << "read finished" << endl;
}
// Das PGM-Bild wird über die Klasse pgm.h geschrieben
void ram::writePGM(){
  write_pgm(outfile,width,height,out_image,depth);
  cout << "write" << endl;
/*  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      cout << to_string(in_image[i][j]) << " ";
    }
    cout << "\n";
  }*/
  cout << "write finished" << endl;
}

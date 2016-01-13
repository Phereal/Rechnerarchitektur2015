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

ram::ram(sc_module_name name, uint8_t id, uint32_t bufferSize) :
    module(name, id, bufferSize)
{
  if(initialize == false)
  {
    init();
    initialize = true;
  }
  pakethandler();
  sensitive << clk.pos();

  //SC_HAS_PROCESS(ram);
}
//Überschreiben der process-Methode der Elternklasse Module
bool ram::process(paket &pkg)
{
  enable = false;
  //Paket einlesen
  i_id = pkg.id;
  i_opcode = pkg.opcode;
  i_sender = pkg.sender;
  i_receiver = pkg.receiver;
  i_xpos = pkg.xpos;
  i_ypos = pkg.ypos;
  i_color = pkg.color;

  //Paket senden
  pkg.id = o_id;
  pkg.opcode = o_opcode;
  pkg.sender = o_sender;
  pkg.receiver = o_receiver;
  pkg.xpos = o_xpos;
  pkg.ypos = o_ypos;
  pkg.color = o_color;
  return enable;
}

void ram::pakethandler()
{
  //Es werden nur OPCodes behandelt, die auch durch das RAM-Modul verarbeitet werden können.
  switch(i_opcode)
  {
    case 0x00:
      break; //[emp]
    case 0x01:
      break; //[exe]
    case 0x02:
      break; //[fin]
    case 0x03:
      break; //[c_req]
    case 0x04: //[r_req]
      o_id = 0;
      o_opcode = 0x07; //[ir_pay]
      o_sender = i_receiver;
      o_receiver = i_sender;
      o_xpos = i_xpos;
      o_ypos = i_ypos;
      o_color = readPixel(i_xpos, i_ypos);
      enable = true;
    case 0x05:
      break; //[ack]
    case 0x06:
      break; //[ic_pay]
    case 0x07:
      break; //[ir_pay]
    case 0x08: //[o_pay]
      writePixel(i_xpos, i_ypos, i_color);
    case 0x09: //[rfi]
      readPGM();
      o_id = 0;
      o_opcode = 0x0A; //[rff]
      o_sender = i_receiver;
      o_receiver = i_sender;
      o_xpos = 0;
      o_ypos = 0;
      o_color = 0;
      enable = true;
    case 0x0A:
      break; //[rff]
    case 0x0B: //[wfi]
      writePGM();
      o_id = 0;
      o_opcode = 0x0C; //[wff]
      o_sender = i_receiver;
      o_receiver = i_sender;
      o_xpos = 0;
      o_ypos = 0;
      o_color = 0;
      enable = true;
    case 0x0C:
      break; //[wff]
    case 0x0D: //[nxt]
      o_id = 0;
      o_opcode = 0x0E; //[nxa]
      o_sender = i_receiver;
      o_receiver = i_sender;
      nxtPixel(&o_xpos, &o_ypos);
      o_color = 0;
      enable = true;
    case 0x0E:
      break; //[nxa]
    default:
      break;
  }
}

// Wenn alle Pixel berechnet wurden werden die Koordinaten X=-1 und Y=-1 zurückgegeben.
void ram::nxtPixel(uint32_t *xpos, uint32_t *ypos)
{
  for(unsigned int i = 0; i < (*ypos); i++)
  {
    for(unsigned int j = 0; j < (*xpos); j++)
    {
      if(nxt_map[i][j] == false)
      {
        (*xpos) = i;
        (*ypos) = j;
        break;
      }
      else
      {
        // TODO Benachrichtigung des Gateway-Moduls, dass alle Pixel berechnet wurden.
      }
    }
  }
}

// Initialisierung aller Vektoren mit Initialwerten
void ram::init()
{
  for(int i = 0; i < height; i++)
  {
    for(int j = 0; j < width; j++)
    {
      in_image[i][j] = 0;
      out_image[i][j] = 0;
      nxt_map[i][j] = false;
    }
  }
}

// Der Wert eines Pixels wird zurückgegeben
unsigned char ram::readPixel(int xpos, int ypos)
{
  return in_image[xpos][ypos];
}

// Ein Pixel im Zielbildvektor wird geschrieben mit Fehlerbehandlung für maximale Bildtiefe
void ram::writePixel(int xpos, int ypos, unsigned char data)
{
  if(data < 0)
  {
    data = 0;
  }
  if(data > depth)
  {
    data = depth;
  }
  out_image[xpos][ypos] = data;
  nxt_map[xpos][ypos] = true;
}

//void read_pgm(std::string filename, int& xpos, int& ypos, int& color_depth, pgm_image& image);
// Das PGM-Bild wird über die Klasse pgm.h eingelesen
void ram::readPGM()
{
  read_pgm(infile, width, height, depth, in_image);
  cout << "read" << endl;
  /*    for (int i = 0; i < heigth; i++){
   for (int j = 0; j < width; j++){
   cout << to_string(in_image[i][j]) << " ";
   }
   cout << "\n";
   }*/
  cout << "read finished" << endl;
}

// Das PGM-Bild wird über die Klasse pgm.h geschrieben
void ram::writePGM()
{
  write_pgm(outfile, width, height, out_image, depth);
  cout << "write" << endl;
  /*  for (int i = 0; i < height; i++){
   for (int j = 0; j < width; j++){
   cout << to_string(in_image[i][j]) << " ";
   }
   cout << "\n";
   }*/
  cout << "write finished" << endl;
}

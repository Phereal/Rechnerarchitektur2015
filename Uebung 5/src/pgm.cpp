#include "pgm.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <cassert>

using namespace std;

void write_pgm(string filename, int width, int height, const pgm_image &image,
    int color_depth)
{

  ofstream outfile(filename);

  // write header
  outfile << "P2\n" << width << " " << height << "\n" << color_depth << "\n";

  //write image image
  for(int y = 0; y < height; ++y)
  {
    for(int x = 0; x < width; ++x)
    {

      // need to create a string from the char as we willed it with small values and
      // the chars with values < 32 are not reasonably printable (see www.asciitable.com)
      // furthermore it is by no means what we intend
      outfile << to_string(image[y][x]) << " ";
    }
    outfile << "\n";
  }

  outfile.close();
}

void read_pgm(string filename, int &width, int &height, int &depth,
    pgm_image &image)
{
  ifstream infile(filename);

  string line;

  enum class states
  {
    FILETYPE, DIMENSION, COLORDEPTH, BITS
  };

  // reading size
  states state = states::FILETYPE;
  regex dimension("([[:digit:]]+)\\s([[:digit:]]+)");
  regex colordepth("[[:digit:]]+");
  regex bits("([[:digit:]]+\\s?)+");

  if(infile.is_open())
  {
    while(getline(infile, line))
    {

      if(state == states::BITS)
      {
        pgm_line image_line;
        if(regex_match(line, bits))
        {

          /* stupid capture groups do not work as I like them to
           * -> switch to other means of parsing */

          stringstream ss(line);
          while(ss.good())
          {
            string intstring;
            getline(ss, intstring, ' ');
            pgm_entry val = atoi(intstring.c_str());
            image_line.push_back(val);
          }

          assert(image_line.size() == width && "Line length does not match");

          image.push_back(image_line);
        }

        else
        {
          assert(false && "Learn to specify bits!");
        }
      }

      if(state == states::COLORDEPTH)
      {
        smatch m;
        if(regex_match(line, m, colordepth))
        {
          state = states::BITS;
          depth = atoi(m[0].str().c_str());
        }
        else
        {
          assert(false && "Colordepth incorrectly specified!");

        }
      }

      if(state == states::DIMENSION)
      {
        // expecting dimension
        smatch m;
        if(regex_match(line, m, dimension))
        {
          width = atoi(m[1].str().c_str());
          height = atoi(m[2].str().c_str());
          state = states::COLORDEPTH;
        }
        else
        {
          // HALT CATCH FIRE
          assert(false && "Dimension not specified reasonably!");
        }
      }

      if(state == states::FILETYPE)
      {
        if(line == "P2")
        {
          state = states::DIMENSION;
        }
        else
        {
          // HALT CATCH FIRE
          assert(false && "Incompatible file type!");
        }

      }

    }
  }

  assert(image.size() == height && "Amount of rows does not match");

}

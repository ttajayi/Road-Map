

//   YOU DO NOT NEED TO UNDERSTAND WHAT IS HAPPENING IN THIS FILE
//   There is nothing of algorithmic interest. All it does is allow
//   you to create a "canvas" to draw on, set any pixel to any colour,
//   and save the image in the .BMP format. BMP really isn't very
//   interesting at all, and is extremely inefficient.


#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "makebmp.h"

using namespace std;

int colour(int r, int g, int b)
{ if (r < 0 || r > 255)
  { cerr << "red value " << r << " out of range 0...255 in colour\n";
    exit(1); }
  if (g < 0 || g > 255)
  { cerr << "green value " << g << " out of range 0...255 in colour\n";
    exit(1); }
  if (b < 0 || b > 255)
  { cerr << "blue value " << b << " out of range 0...255 in colour\n";
    exit(1); }
  return (r << 16) | (g << 8) | b; }

void decode_colour(int c, int & r, int & g, int & b)
{ r = (c >> 16) & 255;
  g = (c >> 8) & 255;
  b = c & 255; }

image::image(int w, int h)
{ width = w;
  height = h;
  pixels = new int[width * height];
  memset(pixels, 255, width * height * sizeof(int)); }

image::~image()
{ delete [] pixels; }

int & image::at(int x, int y)
{ if (x < 0 || x >= width)
  { cerr << "x = " << x << " when accessing image with " << width << " columns\n";
    exit(1); }
  if (y < 0 || y >= height)
  { cerr << "y = " << y << " when accessing image with " << height << " rows\n";
    exit(1); }
  return pixels[(height - y - 1) * width + x]; }

void image::write(const string & filename)
{ ofstream f(filename, ios::out | ios::trunc | ios::binary);
  if (f.fail())
  { cerr << "Can't create file \"" << filename << "\"\n";
    exit(1); }

  int rowbytes = width * 3;
  int phase = rowbytes % 4;
  int extras = 4 - phase;
  if (phase != 0)
    rowbytes += extras;

  BMPHeader prehdr;
  prehdr.letterB = 'B';
  prehdr.letterM = 'M';
  prehdr.filesize = 54 + height * rowbytes;
  prehdr.reserved = 0;
  prehdr.dataoffset = 54;
  f.write((char *) & prehdr.letterB, 14);

  BMPInfoHeader bif;
  bif. forty = 40;
  bif.width = width;
  bif.height = height;
  bif.planes = 1;
  bif.bitsperpixel = 24;
  bif.compression = 0;
  bif.imagesize = 0;
  bif.xpixelspermetre = 3936;
  bif.ypixelspermetre = 3936;
  bif.numcolours = 0;
  bif.numimportantcolours = 0;
  f.write((char *) & bif, 40);

  unsigned char * row = new unsigned char[rowbytes + 8];
  int index = 0;
  for (int r = 0; r < height; r += 1)
  { int pos = 0;
    for (int c = 0; c < width; c += 1)
    { int pixel = pixels[index];
      * (int *) (row + pos) = pixel;
      index += 1;
      pos += 3; }
    * (int *) (row + pos) = 0;
    f.write((char *) row, rowbytes); }
  
  f.close(); }

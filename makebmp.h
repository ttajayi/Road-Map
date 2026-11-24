#ifndef ____makebmp_included____
#define ____makebmp_included____

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

int colour(int r, int g, int b);
void decode_colour(int c, int & r, int & g, int & b);

struct BMPHeader
{ short int rubbish;
  char letterB;
  char letterM;
  int filesize;
  int reserved;
  int dataoffset; };

struct BMPInfoHeader
{ int forty;
  int width;
  int height;
  short int planes;
  short int bitsperpixel;
  int compression;
  int imagesize;
  int xpixelspermetre;
  int ypixelspermetre;
  int numcolours;
  int numimportantcolours; };

struct image
{ int * pixels;
  int width, height;

  image(int w, int h);
  ~image();
  int & at(int x, int y);
  void write(const string & filename); };

#endif


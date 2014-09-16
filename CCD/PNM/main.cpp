/* 
 * File:   main.cpp
 * Author: dvs
 *
 * Created on September 10, 2014, 8:30 AM
 * 
 * test the PNM library
 */

// ----------------------------------------------
// includes
// ----------------------------------------------
#include <iostream>
#include <cstdlib>
using namespace std;

#include <math.h>

#include "pnm.h"

// ----------------------------------------------
// macros
// ----------------------------------------------
#define random rand

/*
 * 
 */
int main(int argc, char** argv) {

  // set up a random number generator
  srand((unsigned int)time(0));

  bool ok = true;

  // create a PNM
  cout << "instantiating a PNM ..." << endl;
  try {
    PNM pnm;
    cout << "  success" << endl;
  }
  catch(PNM_Exception e) {
    cout << "  failed: " << e.what() << endl;
    ok = false;
  }

  PNM pnm;

  // read a file
  cout << "reading a file ..." << endl;
  try {
    pnm.read(string("babe.ppm"));
    cout << "  success" << endl;
  }
  catch(PNM_IOException e) {
    cout << "unable to read file: PNM_IOException: " << e.what() << endl;
    ok = false;
  }
  catch(PNM_Exception e) {
    cout << "unable to read file: PNM_Exception: " << e.what() << endl;
    ok = false;
  }
  catch(...) {
    cout << "unable to read file" << endl;
    ok = false;
  }

  // scale an image
  cout << "testing interpolation ..." << endl;
  try {
    int cols = pnm.cols();
    int rows = pnm.rows();
    int value;

    // create a scaled image
    double scale = 2.0;

    // calculate output image size
    int xdim = (int)rint((double)cols * scale);
    int ydim = (int)rint((double)rows * scale);

    // create output image
    PNM pnmL(xdim, ydim, PNM::typePGM);

    // set interpolation method
    pnm.interpolationMethod(PNM::LINEAR);

    // do the scaling
    double i,j;
    for(int x = 0; x < xdim; x++) {
      for(int y = 0; y < ydim; y++) {
        try {
          i = (double)x/scale;
          j = (double)y/scale;
          pnmL.gray(x,y,pnm.gray(i,j));
        }
        catch(PNM_ArrayException e) {}
      }
    }

    // write the results
    pnmL.write(string("scaledL.pgm"));

  }
  catch(PNM_ArrayException e) {
    cout << "unable to do interpolation: PNM_ArrayException: " << e.what() << endl;
    ok = false;
  }
  catch(PNM_Exception e) {
    cout << "unable to do interpolation: PNM_Exception: " << e.what() << endl;
    ok = false;
  }
  catch(...) {
    cout << "unable to do interpolation" << endl;
    ok = false;
  }
  
  // draw some lines
  cout << "drawing some lines ..." << endl;
  try {
    int cols = pnm.cols();
    int rows = pnm.rows();

    // clear the image to a light white, with a hint of magenta
    pnm = PNM_Color(200, 170, 190);

    pnm.drawLine(0,     0,      cols-1,rows-1, PNM_Color(255, 255, 255));
    pnm.drawLine(0,     rows-1, cols-1,0,      PNM_Color(255, 0,   0));
    pnm.drawLine(cols/2,0,      cols/2,rows-1, PNM_Color(0,   255, 0));
    pnm.drawLine(0,     rows/2, cols-1,rows/2, PNM_Color(0,   0,   255));
    pnm.write(string("lines.ppm"));
  }
  catch(PNM_IOException e) {
    cout << "unable to draw a line: PNM_IOException: " << e.what() << endl;
    ok = false;
  }
  catch(PNM_Exception e) {
    cout << "unable to draw a line: PNM_Exception: " << e.what() << endl;
    ok = false;
  }
  catch(...) {
    cout << "unable to draw a line" << endl;
    ok = false;
  }

  // draw some polygons
  cout << "drawing some polygons ..." << endl;
  try {
    int cols = pnm.cols();
    int rows = pnm.rows();
    list<PNM_Point> poly;
    PNM_Point p;
    PNM_Color c;
    int vertices;

    // clear the image to a neutral gray
    pnm = 128;

    for(int j = 0; j < 5; j++) {
      c.red(random() & 0xff);
      c.green(random() & 0xff);
      c.blue(random() & 0xff);
      vertices = random() & 0x07;
      for(int i = 0; i < vertices; i++) {
        p.x = (random() & 0xff) + (cols/4);
        p.y = (random() & 0xff) + (rows/4);
        poly.push_back(p);
      }
      poly.push_back(p);
      pnm.drawPolygon(poly, PNM_Color(0,0,0), PNM_Color(p.x,p.y,p.x));
      poly.clear();
    }
    pnm.write(string("polys.ppm"));
  }
  catch(...) {
    cout << "unable to draw a polygon" << endl;
    ok = false;
  }

  // test exceptions
  cout << "testing exceptions ..." << endl;
  try {
    pnm.gray(-1, -1);
    cout << "  failed" << endl;
    ok = false;
  }
  catch(PNM_ArrayException e) {
    cout << "  PNM_ArrayException: " << e.what() << endl;
    ok = true;
  }
  catch(PNM_Exception e) {
    cout << "  PNM_Exception: " << e.what() << endl;
    ok = false;
  }
  catch(...) {
    cout << "  access failed" << endl;
    ok = false;
  }

  // convert to PGM
  cout << "converting file ..." << endl;
  try {
    pnm.convert(PNM::typePGM);
    cout << "  success" << endl;
  }
  catch(PNM_Exception e) {
    cout << "  unable to convert file: PNM_Exception: " << e.what() << endl;
    ok = false;
  }
  catch(...) {
    cout << "  unable to convert file" << endl;
    ok = false;
  }
	
  // write the file
  cout << "writing a file ..." << endl;
  try {
    pnm.write(string("babe.pgm"));
    cout << "  success" << endl;
  }
  catch(PNM_IOException e) {
    cout << "  unable to write file: PNM_IOException: " << e.what() << endl;
    ok = false;
  }
  catch(PNM_Exception e) {
    cout << "  unable to write file: PNM_Exception: " << e.what() << endl;
    ok = false;
  }
  catch(...) {
    cout << "  unable to write file" << endl;
    ok = false;
  }

  if(ok) {
    cout << "PNM appears to be working" << endl;
  }
  else {
    cout << "Errors encountered during test" << endl;
  }

  return 0;

}


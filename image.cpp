#include <iostream>
#include <fstream>
#include <strstream>
#include <list>
#include <string>
using namespace std;

#include "image.h"

// constructor
Image::Image() {
}

// script constructor
Image::Image(const string &filename) {

  // initialize data members

  // load image
  try {
    PNM *tmp = new PNM(filename);
    im = *tmp;
  }
  catch (PNM_IOException) {
    cerr << "Image::(const string &" << filename << "): open failed" << endl;
  }

}

// destructor
Image::~Image() {
}

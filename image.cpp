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
  } catch (PNM_IOException) {
    cerr << "Image::(const string &" << filename << "): open failed" << endl;
  }

}

// destructor

Image::~Image() {
}

// -------------------------------------
// morphology
//
//   open
//   close
// -------------------------------------

void Image::open(int depth=1) {
  for(int i = 0; i < depth; i++) {
    erode();
  }
  for(int i = 0; i < depth; i++) {
    dilate();
  }
}

void Image::close(int depth=1) {
  for(int i = 0; i < depth; i++) {
    dilate();
  }
  for(int i = 0; i < depth; i++) {
    erode();
  }
}

#include <iostream>
#include <fstream>
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

void Image::writeSexpr(ostream &out)
{
  // This should be the nodes of the image Graph.
  out << "(graph" << endl;
  out << " (nodes";
  for (map<unsigned int, Shape>::iterator iter = dictionary.begin(); iter != dictionary.end(); iter++) {
    out << " (";
    out << iter->first;
    out << " ";
    iter->second.writeSexpr(out);
    out << ")";
  }
  out << ")" << endl;
  graph.writeSexpr(out);
  out << ")" << endl;
}

// -------------------------------------
// reconstruct
//
// for each shape
// draw the boundary on the output image
// fill with the shape color
//
// -------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <ctime>
using namespace std;

#include "pnm.h"
#include "image.h"


// foward declarations
//void drawShape(PNM *img, Shape &s, PNM_Color &c);
//void fillShape(PNM *img, Shape &s, PNM_Color &c);


// -------------------------------------
// main
// -------------------------------------
bool Image::reconstruct(bool debug, string outputImageName) {

  bool ok = true;
  string flag;

  ifstream in;
  ofstream out;
  ofstream imageOut;

  // open output image file
  if(!outputImageName.empty()) {
    if(debug) cerr << "  opening " << outputImageName << endl;
    imageOut.open(outputImageName.c_str(), ios::out);
    if(!imageOut) {
      cerr << "  unable to open '" << outputImageName << "' for writing" << endl;
      ok = false;
    }
    imageOut.close();
  }
  else {
    cerr << "  no output image specified, aborting" << endl;
    ok = false;
  }

  int count = 0;
  int maxX = 0;
  int maxY = 0;
  PNM *outputImage = NULL;
  map<unsigned int, Shape>::iterator iter;

  // figure out how big to make output image
  if(ok) {
    if(debug) cerr << "  calculating output image size" << endl;
    list<Point>::iterator biter;
    for(iter = dictionary.begin(); iter != dictionary.end(); iter++) {
      for(biter = iter->second.points.begin(); biter != iter->second.points.end(); biter++) {
        maxX = (maxX < biter->x) ? biter->x : maxX;
        maxY = (maxY < biter->y) ? biter->y : maxY;
      }
    }
  }

  // create output image
  if(ok) {
    if(debug) cerr << "  creating output PNM("
      << maxX + 1 << ", "
      << maxY + 1 << ", PNM:typePGM)" << endl;
    outputImage = new PNM(maxX + 1, maxY + 1, PNM::typePGM);
  }

  // --------------------------------------------------------
  // draw and fill all the connected components in the image
  // --------------------------------------------------------
  PNM_Color c;

  // for each shape
  if(ok) {
    if(debug) cerr << "  drawing and filling shapes" << endl;
    for(iter = dictionary.begin(); iter != dictionary.end(); iter++) {
      if(iter->second.label > 0) {
        c.red(iter->second.color);
        c.green(iter->second.color);
        c.blue(iter->second.color);
        drawShape(outputImage, iter->second, c);
        fillShape(outputImage, iter->second, c);
        count++;
      }
    }
  }

  // write the results
  if(ok) {
    outputImage->write(outputImageName);
  }

  if(debug) cerr << "  drew and filled " << count << " shapes" << endl;

  return ok;
}

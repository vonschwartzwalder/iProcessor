// -------------------------------------
// overlay
//
// for each shape
//   draw the boundary on the output image
// -------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <ctime>
using namespace std;

#include "image.h"

// ---------------------------------------------------------------------
// overlay
// ---------------------------------------------------------------------

void Image::overlay() {

  int count = 0;

  // --------------------------------------------------------
  // draw all the connected components on the image
  // --------------------------------------------------------
  im.convert(PNM::typePPM);

  map<unsigned int, Shape>::iterator iter;
  PNM_Color c[6];
  c[0] = PNM_Color(255, 0, 0);
  c[1] = PNM_Color(0, 255, 0);
  c[2] = PNM_Color(0, 0, 255);
  c[3] = PNM_Color(255, 255, 0);
  c[4] = PNM_Color(0, 255, 255);
  c[5] = PNM_Color(255, 0, 255);
  int color = 0;

  // for each shape
  for (iter = dictionary.begin(); iter != dictionary.end(); iter++) {
    if (iter->second.label > 0) {
      drawShape(&im, iter->second, c[color]);
      color = (color + 1) % 6;
      count++;
    }
  }

}

// -------------------------------------
// draw a shape on an image
// -------------------------------------
#include "image.h"

void Image::drawShape(PNM *img, Shape &s, PNM_Color &c) {

  // draw lines between each point
  Point curr;
  Point prev;
  list<Point>::iterator iter;
  iter = s.points.begin();
  prev = *iter;
  for(; iter != s.points.end(); iter++) {
    curr = *iter;

    // draw the line
    try {
      img->drawLine(prev.x, prev.y, curr.x, curr.y, c);
    }
    catch(PNM_ArrayException e) {}
    prev = curr;
  }

} // drawShape


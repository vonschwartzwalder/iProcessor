// ---------------------------------------------------------------------
// Shape declaration
// ---------------------------------------------------------------------

#ifndef SHAPE_H
#define SHAPE_H

// includes
#include <iostream>
#include <list>

#include "point.h"


class Shape {
public:
  
  // constructors
  Shape();

  // destructor
  ~Shape();

  // I/O
  friend ostream &operator<<(ostream &out, Shape &s);

  // methods
  bool contains(Shape &s);

  // operators
  bool operator<(Shape &s);
  bool operator==(Shape &s);
  bool operator>(Shape &s);
  Shape &operator+(Shape &s);
  
  // data 
  int          label;          // label
  Point        location;       // location (top left)
  int          color;          // color of pixels
  int          pixel_count;    // count of pixels
  int          area;           // enclosed area in pixels
  list<Point>  points;         // boundary points
  
  double moments[8];
  Point  centroid;
  double perimeter;
  Point  upper_left;
  Point  lower_right;
  
};

#endif

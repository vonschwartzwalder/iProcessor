// ---------------------------------------------------------------------
// Shape declaration
// ---------------------------------------------------------------------

#ifndef SHAPE_H
#define SHAPE_H

// includes
#include <iostream>
#include <list>
using namespace std;

#include "point.h"


class Shape {
public:
  
  // constructors
  Shape();

  // destructor
  ~Shape();

  static const int AREA;
  static const int LABEL;
  static const int COLOR;
  static const int LOCATION;
  static const int X;
  static const int Y;
  static const int FLOW;
  
  // I/O
  friend ostream &operator<<(ostream &out, Shape &s);
  ostream &write(ostream &out);
  friend istream &operator>>(istream &in, Shape &s);
  istream &read(istream &in);
  void read();
  
  // methods
  bool contains(Shape &s);

  // variable access
  int compare(int c);
  int compare();

  // operators
  bool operator<(Shape &s);
  bool operator==(Shape &s);
  bool operator>(Shape &s);
  
  // data
  int _compare;            // what field to use for comparison
  
  unsigned int label;      // label
  Point        location;   // location (top left)
  int          color;      // color of pixels
  int          area;       // enclosed area in pixels
  Point  centroid;
  Point  upper_left;       // bounding box
  Point  lower_right;      //  "
  list<Point>  points;     // boundary points
  
  double moments[8];
  double flow;
  
};

istream &operator>>(istream &in, Shape &s);

#endif

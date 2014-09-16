// ---------------------------------------------------------------------
// Point declaration
// ---------------------------------------------------------------------

#ifndef POINT_H
#define POINT_H

// includes
#include <iostream>
#include <cmath>
using namespace std;


class Point {
public:

  // constructors
  Point();
  Point(const Point &p);
  Point(const int x, const int y);

  // destructor
  ~Point();

  // I/O
  friend ostream &operator<<(ostream &out, Point &p);

  // methods

  // operators
  bool operator<(const Point &p);
  bool operator==(const Point &p);
  bool operator>(const Point &p);
  Point &operator+(const Point &p);
  Point &operator-(const Point &p);
  Point &operator*(const Point &p);
  Point &operator/(const Point &p);
  Point &operator+(int v);
  Point &operator-(int v);
  Point &operator*(int v);
  Point &operator/(int v);

  // data
  int x;
  int y;
  int d;
  
};

#endif

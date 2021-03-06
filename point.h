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
  Point(const int x, const int y, const int d);

  // destructor
  ~Point();

  // I/O
  friend ostream &operator<<(ostream &out, Point &p);
  ostream &write(ostream &out);
  friend istream &operator<<(istream &in, Point &p);
  istream &read(istream &in);
  void read();
  void writeSexpr(ostream &out);

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

istream &operator>>(istream &in, Point &p);

#endif

// ---------------------------------------------------------------------
// Point implementation
// ---------------------------------------------------------------------

#include <math.h>

#include "point.h"

// -------------------------------------
// constructors
// -------------------------------------
Point::Point() {
  x = 0;
  y = 0;
  d = 0;
}

Point::Point(const Point &p) {
  x = p.x;
  y = p.y;
  d = p.d;
}

Point::Point(const int x, const int y) {
  this->x = x;
  this->y = y;
  this->d = d;
}

// -------------------------------------
// destructor
// -------------------------------------
Point::~Point() {
}

// -------------------------------------
// I/O
// -------------------------------------
ostream &operator<<(ostream &out, Point &p) {
  out << p.x << "," << p.y;
  return out;
}
ostream &Point::write(ostream &out) {
  out << x << "," << y;
  return out;
}

istream &Point::read(istream &in) {
  in >> *this;
  return in;
}

istream &operator>>(istream &in, Point &p) {
  int cp;
  int value;

  // get x
  in >> value;
  p.x = value;

  // skip white space
  cp = in.peek();
  while (cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
    in.get();
    cp = in.peek();
  }

  // skip comma
  cp = in.get();
  if (cp != ',') {
    cerr << "no comma found, found " << cp << " instead" << endl;
    cerr << "pretending " << cp << " is a comma and continuing" << endl;
  }

  // skip white space
  cp = in.peek();
  while (cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
    in.get();
    cp = in.peek();
  }

  // get y
  in >> value;
  p.y = value;

  return in;
}

// -------------------------------------
// operators
// -------------------------------------

// less than: compare distance from the origin
bool Point::operator<(const Point &p) {
  return (d < p.d) ? true : false;
}

// equality
bool Point::operator==(const Point &p) {
  return (x == p.x && y == p.y) ? true : false;
}

// greater than: compare distance from the origin
bool Point::operator>(const Point &p) {
  return (d > p.d) ? true : false;
}

Point &Point::operator+(const Point &p) {
  Point *pp = new Point();
  pp->x = x + p.x;
  pp->y = y + p.y;
  pp->d = sqrt((double)(pp->x*pp->x + pp->y*pp->y));
  return *pp;
}

Point &Point::operator-(const Point &p) {
  Point *pp = new Point();
  pp->x = x - p.x;
  pp->y = y - p.y;
  pp->d = sqrt((double)(pp->x*pp->x + pp->y*pp->y));
  return *pp;
}

Point &Point::operator*(const Point &p) {
  Point *pp = new Point();
  pp->x = x * p.x;
  pp->y = y * p.y;
  pp->d = sqrt((double)(pp->x*pp->x + pp->y*pp->y));
  return *pp;
}

Point &Point::operator/(const Point &p) {
  Point *pp = new Point();
  pp->x = x / p.x;
  pp->y = y / p.y;
  pp->d = sqrt((double)(pp->x*pp->x + pp->y*pp->y));
  return *pp;
}

Point &Point::operator+(int v) {
  Point *pp = new Point();
  pp->x = x + v;
  pp->y = y + v;
  pp->d = sqrt((double)(pp->x*pp->x + pp->y*pp->y));
  return *pp;
}

Point &Point::operator-(int v) {
  Point *pp = new Point();
  pp->x = x - v;
  pp->y = y - v;
  pp->d = sqrt((double)(pp->x*pp->x + pp->y*pp->y));
  return *pp;
}

Point &Point::operator*(int v) {
  Point *pp = new Point();
  pp->x = x * v;
  pp->y = y * v;
  pp->d = sqrt((double)(pp->x*pp->x + pp->y*pp->y));
  return *pp;
}

Point &Point::operator/(int v) {
  Point *pp = new Point();
  pp->x = x / v;
  pp->y = y / v;
  pp->d = sqrt((double)(pp->x*pp->x + pp->y*pp->y));
  return *pp;
}

// ---------------------------------------------------------------------
// Point implementation
// ---------------------------------------------------------------------

#include "point.h"

// -------------------------------------
// constructors
// -------------------------------------
Point::Point() {
  x = -1;
  y = -1;
  d = -1;
}

Point::Point(const Point &p) {
  x = p.x;
  y = p.y;
  d = p.d;
}

Point::Point(const int x, const int y) {
  this->x = x;
  this->y = y;
  d = -1;
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
  //out << p.x << "," << p.y << "," << p.d;
  out << p.x << "," << p.y;
  return out;
}

// -------------------------------------
// methods
// -------------------------------------

// -------------------------------------
// operators
// -------------------------------------

// less than: compare distance from the origin
bool Point::operator<(const Point &p) {
  double d1, d2;
  d1 = sqrt((double)(x*x + y*y));
  d2 = sqrt((double)(p.x*p.x + p.y*p.y));
  return (d1 < d2) ? true : false;
}

// equality
bool Point::operator==(const Point &p) {
  return (x == p.x && y == p.y) ? true : false;
}

// greater than: compare distance from the origin
bool Point::operator>(const Point &p) {
  double d1, d2;
  d1 = sqrt((double)(x*x + y*y));
  d2 = sqrt((double)(p.x*p.x + p.y*p.y));
  return (d1 > d2) ? true : false;
}

Point &Point::operator+(const Point &p) {
  Point *pp = new Point();
  pp->x = x + p.x;
  pp->y = y + p.y;
  return *pp;
}

Point &Point::operator-(const Point &p) {
  Point *pp = new Point();
  pp->x = x - p.x;
  pp->y = y - p.y;
  return *pp;
}

Point &Point::operator*(const Point &p) {
  Point *pp = new Point();
  pp->x = x * p.x;
  pp->y = y * p.y;
  return *pp;
}

Point &Point::operator/(const Point &p) {
  Point *pp = new Point();
  pp->x = x / p.x;
  pp->y = y / p.y;
  return *pp;
}

Point &Point::operator+(int v) {
  Point *pp = new Point();
  pp->x = x + v;
  pp->y = y + v;
  return *pp;
}

Point &Point::operator-(int v) {
  Point *pp = new Point();
  pp->x = x - v;
  pp->y = y - v;
  return *pp;
}

Point &Point::operator*(int v) {
  Point *pp = new Point();
  pp->x = x * v;
  pp->y = y * v;
  return *pp;
}

Point &Point::operator/(int v) {
  Point *pp = new Point();
  pp->x = x / v;
  pp->y = y / v;
  return *pp;
}


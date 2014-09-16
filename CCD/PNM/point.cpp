// ---------------------------------------------------------------------
// PNM_Point implementation
// ---------------------------------------------------------------------

#include <math.h>

#include "pnm.h"

// -------------------------------------
// constructors
// -------------------------------------
PNM_Point::PNM_Point() {
  x = 0;
  y = 0;
}

PNM_Point::PNM_Point(const PNM_Point &p) {
  x = p.x;
  y = p.y;
}

PNM_Point::PNM_Point(const int x, const int y) {
  this->x = x;
  this->y = y;
}

// -------------------------------------
// destructor
// -------------------------------------
PNM_Point::~PNM_Point() {
}

// -------------------------------------
// I/O
// -------------------------------------
ostream &operator<<(ostream &out, PNM_Point &p) {
  out << p.x << "," << p.y;
  return out;
}
ostream &PNM_Point::write(ostream &out) {
  out << x << "," << y;
  return out;
}

istream &PNM_Point::read(istream &in) {
  in >> *this;
  return in;
}

istream &operator>>(istream &in, PNM_Point &p) {
  int cp;
  int value;

  // get x
  in >> value;
  p.x = value;

  // skip white space
  cp = in.peek();
  while(cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
    in.get();
    cp = in.peek();
  }

  // skip comma
  cp = in.get();
  if(cp != ',') {
    cerr << "no comma found, found " << cp << " instead" << endl;
    cerr << "pretending " << cp << " is a comma and continuing" << endl;
  }

  // skip white space
  cp = in.peek();
  while(cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
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
bool PNM_Point::operator<(PNM_Point &p) {
  double d1, d2;
  d1 = sqrt((double)(x*x + y*y));
  d2 = sqrt((double)(p.x*p.x + p.y*p.y));
  return (d1 < d2) ? true : false;
}

// equality
bool PNM_Point::operator==(PNM_Point &p) {
  return (x == p.x && y == p.y) ? true : false;
}

// greater than: compare distance from the origin
bool PNM_Point::operator>(PNM_Point &p) {
  double d1, d2;
  d1 = sqrt((double)(x*x + y*y));
  d2 = sqrt((double)(p.x*p.x + p.y*p.y));
  return (d1 > d2) ? true : false;
}

PNM_Point &PNM_Point::operator+(PNM_Point &p) {
  PNM_Point *pp = new PNM_Point();
  pp->x = x + p.x;
  pp->y = y + p.y;
  return *pp;
}

PNM_Point &PNM_Point::operator-(PNM_Point &p) {
  PNM_Point *pp = new PNM_Point();
  pp->x = x - p.x;
  pp->y = y - p.y;
  return *pp;
}

PNM_Point &PNM_Point::operator*(PNM_Point &p) {
  PNM_Point *pp = new PNM_Point();
  pp->x = x * p.x;
  pp->y = y * p.y;
  return *pp;
}

PNM_Point &PNM_Point::operator/(PNM_Point &p) {
  PNM_Point *pp = new PNM_Point();
  pp->x = x / p.x;
  pp->y = y / p.y;
  return *pp;
}

PNM_Point &PNM_Point::operator+(int v) {
  PNM_Point *pp = new PNM_Point();
  pp->x = x + v;
  pp->y = y + v;
  return *pp;
}

PNM_Point &PNM_Point::operator-(int v) {
  PNM_Point *pp = new PNM_Point();
  pp->x = x - v;
  pp->y = y - v;
  return *pp;
}

PNM_Point &PNM_Point::operator*(int v) {
  PNM_Point *pp = new PNM_Point();
  pp->x = x * v;
  pp->y = y * v;
  return *pp;
}

PNM_Point &PNM_Point::operator/(int v) {
  PNM_Point *pp = new PNM_Point();
  pp->x = x / v;
  pp->y = y / v;
  return *pp;
}


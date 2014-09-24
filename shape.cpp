// ---------------------------------------------------------------------
// Shape implementation
// ---------------------------------------------------------------------

#include "shape.h"

// constructors
Shape::Shape() {
  label = -1;
  color = -1;
  pixel_count = 0;
  area = 0;
  for (int i = 0; i < 8; i++) {
    moments[i] = 0.0;
  }
  perimeter = 0.0;
}

// destructor
Shape::~Shape() {
}

// I/O
ostream &operator<<(ostream &out, Shape &s) {

  list<Point>::iterator iter;

  // shape
  out << "shape( "
    << "label=" << s.label << " "
    << "location=" << s.location << " "
    << "color=" << s.color << " "
    << "pixel_count=" << s.pixel_count << " "
    << "area=" << s.area << " "

    // descriptors
    << "descriptors( "
    << "perimeter=" << s.perimeter << " "
    << "centroid=" << s.centroid << " "
    << "upper_left=" << s.upper_left << " "
    << "lower_right=" << s.lower_right << " "
    << ") "

    // moments
    << "moments( "
    << "m00=" << s.moments[0] << " "
    << "m10=" << s.moments[1] << " "
    << "m01=" << s.moments[2] << " "
    << "u11=" << s.moments[3] << " "
    << "u20=" << s.moments[4] << " "
    << "u02=" << s.moments[5] << " "
    << "X=" << s.moments[6] << " "
    << "Y=" << s.moments[7] << " "
    << ") "

    << "boundary( ";
  for (iter = s.points.begin(); iter != s.points.end(); iter++) {
    out << *iter << " ";
  }
  out << ")";

  return out;
}


// -------------------------------------
// contains
// -------------------------------------
bool Shape::contains(Shape &s) {

  // check bounding box
  if (!((s.location.x >= upper_left.x && s.location.x <= lower_right.x) &&
    (s.location.y >= upper_left.y && s.location.y <= lower_right.y))) {
    return false;
  }

  // check edges
  if (s.location.x == 0 || s.location.y == 0) {
    return false;
  }

  bool outside1, outside2, outside3, outside4;
  outside1 = outside2 = outside3 = outside4 = true;

  // check boundary points
  list<Point>::iterator iter;
  Point last = *(points.begin());
  for (iter = points.begin(); iter != points.end(); iter++) {

    // check in the x direction
    if ((*iter).y == s.location.y && (*iter).x < s.location.x) {
      if ((*iter).y != last.y) {
        outside1 = !outside1;
      }
    }
    if ((*iter).y == s.location.y && (*iter).x > s.location.x) {
      if ((*iter).y != last.y) {
        outside2 = !outside2;
      }
    }

    // check in the y direction
    if ((*iter).x == s.location.x && (*iter).y < s.location.y) {
      if ((*iter).x != last.x) {
        outside3 = !outside3;
      }
    }
    if ((*iter).x == s.location.x && (*iter).y > s.location.y) {
      if ((*iter).x != last.x) {
        outside4 = !outside4;
      }
    }

    last = *iter;

  }

  // is this point on the outside?
  if (outside1 && outside2) {
    return false;
  }
  if (outside3 && outside4) {
    return false;
  }

  // not outside, so must be inside
  return true;
}


// -------------------------------------
// operator==
//
// this is where the matching code will be
// for now it just compares enclosed area
// -------------------------------------
bool Shape::operator==(Shape &s) {
  return (area == s.area);
}

// -------------------------------------
// operator<
//
// this is used to sort lists of shapes
// -------------------------------------
bool Shape::operator<(Shape &s) {
  return (area > s.area);
}

// -------------------------------------
// operator>
//
// this is used to sort lists of shapes
// -------------------------------------
bool Shape::operator>(Shape &s) {
  return (area < s.area);
}

// -------------------------------------
// coalesce two shapes
//   add the smaller into the larger
//     use boundary of larger area
//   add pixel counts
// -------------------------------------
Shape &Shape::operator+(Shape &s) {
  Shape *sp;

  // use copy constructor with largest shape
  if (area > s.area) {
    sp = new Shape(*this);
  }
  else {
    sp = new Shape(s);
  }

  // add pixel_counts
  if (pixel_count != -1 && s.pixel_count != -1) {
    sp->pixel_count = pixel_count + s.pixel_count;
  }
  else if (pixel_count != -1 && s.pixel_count == -1){
    sp->pixel_count = pixel_count;
  }
  else if (pixel_count == -1 && s.pixel_count != -1){
    sp->pixel_count = s.pixel_count;
  }
  else {
    sp->pixel_count = -1;
  }

  return *sp;
}


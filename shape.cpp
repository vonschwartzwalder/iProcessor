// ---------------------------------------------------------------------
// Shape implementation
// ---------------------------------------------------------------------

#include "shape.h"

const int Shape::AREA     = 0;
const int Shape::LABEL    = 1;
const int Shape::COLOR    = 2;
const int Shape::LOCATION = 3;
const int Shape::X        = 4;
const int Shape::Y        = 5;
const int Shape::FLOW     = 6;

// constructors
Shape::Shape() {
  _compare = AREA;
  label = 0;
  color = 0; 
  area = 0;
  for(int i = 0; i < 8; i++) {
    moments[i] = 0.0;
  }
  flow = 0.0;
}

// destructor
Shape::~Shape() {
}

// I/O
ostream &operator<<(ostream &out, Shape &s) {

  list<Point>::iterator iter;
  bool first_time = true;

  // header
  out << "shape {" << endl;
    
  // basic
  out << "  label=" << s.label << endl;
  out << "  location=" << s.location << endl;
  out << "  color=" << s.color << endl;
  out << "  area=" << s.area << endl;
  out << "  centroid=" << s.centroid << endl;
  out << "  upper_left=" << s.upper_left << endl;
  out << "  lower_right=" << s.lower_right << endl;

  // moments
  out << "  m00=" << s.moments[0] << endl;
  out << "  m10=" << s.moments[1] << endl;
  out << "  m01=" << s.moments[2] << endl;
  out << "  u11=" << s.moments[3] << endl;
  out << "  u20=" << s.moments[4] << endl;
  out << "  u02=" << s.moments[5] << endl;
  out << "  X="   << s.moments[6] << endl;
  out << "  Y="   << s.moments[7] << endl;
  out << "  flow=" << s.flow << endl;

  // boundary
  out << "  boundary=(";
  for(iter = s.points.begin(); iter != s.points.end(); iter++) {
    if(first_time) {
      out << *iter;
      first_time = false;
    }
    else {
      out << " " << *iter;
    }
  }
  out << ")";

  out << endl << "}" << endl;

  return out;
}

ostream &Shape::write(ostream &out) {
  out << *this;
  return out;
}

istream &Shape::read(istream &in) {
  in >> *this;
  return in;
}

void Shape::read() {
  cin >> *this;
}

istream &operator>>(istream &in, Shape &s) {
  int cp;
  string id;
  Point p;

  // look for a shape
  while(id != "shape" && !in.eof()) {

    // find start of shape id
    cp = in.peek();
    while(cp != 's' && !in.eof()) {
      in.get();
      cp = in.peek();
    }

    // get shape id
    cp = in.peek();
    while(isalpha(cp) && !in.eof()) {
      id += in.get();
      cp = in.peek();
    }

    if(id != "shape") id = "";
  }
  if(in.eof()) return in;

  // skip white space
  cp = in.peek();
  while(cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
    in.get();
    if(in.eof()) return in;
    cp = in.peek();
  }

  // get starting brace
  cp = in.peek();
  if(cp != '{') {
    cerr << "no starting brace" << endl;
    return in;
  }
  else in.get();
  if(in.eof()) return in;

  // skip white space
  cp = in.peek();
  while(cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
    in.get();
    if(in.eof()) return in;
    cp = in.peek();
  }

  // read keys and values until closing brace
  string key;
  while(cp != '}') {

    // skip white space
    cp = in.peek();
    while(cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
      in.get();
      if(in.eof()) return in;
      cp = in.peek();
    }

    // get a keyword
    key = "";
    while(!(cp == '=' || cp == ',' || cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r')) {
      cp = in.get();
      if(in.eof()) return in;
      key += cp;
      cp = in.peek();
    }

    // skip white space and equal signs
    cp = in.peek();
    while(cp == '=' || cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
      in.get();
      if(in.eof()) return in;
      cp = in.peek();
    }

    // get a value
    if     (key == "label")       in >> s.label;
    else if(key == "location")    in >> s.location;
    else if(key == "color")       in >> s.color;
    else if(key == "area")        in >> s.area;
    else if(key == "flow")        in >> s.flow;
    else if(key == "centroid")    in >> s.centroid;
    else if(key == "upper_left")  in >> s.upper_left;
    else if(key == "lower_right") in >> s.lower_right;
    else if(key == "m00")         in >> s.moments[0];
    else if(key == "m10")         in >> s.moments[1];
    else if(key == "m01")         in >> s.moments[2];
    else if(key == "u11")         in >> s.moments[3];
    else if(key == "u20")         in >> s.moments[4];
    else if(key == "u02")         in >> s.moments[5];
    else if(key == "X")           in >> s.moments[6];
    else if(key == "Y")           in >> s.moments[7];
    else if(key == "boundary") {

      // find next open paren
      cp = in.peek();
      while(cp != '(') {
        in.get();
        if(in.eof()) return in;
        cp = in.peek();
      }
      in.get();

      // skip white space
      cp = in.peek();
      while(cp == ',' || cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
        in.get();
        if(in.eof()) return in;
        cp = in.peek();
      }

      // read points until closing paren
      cp = in.peek();
      while(cp != ')') {

        // read a point
        in >> p;
        if(in.eof()) return in;
        s.points.push_back(p);

        // skip white space
        cp = in.peek();
        while(cp == ',' || cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
          in.get();
          if(in.eof()) return in;
          cp = in.peek();
        }

      }
      in.get();
      if(in.eof()) return in;
    }
    if(in.eof()) return in;
    
    // skip white space and commas
    cp = in.peek();
    while(cp == ',' || cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
      in.get();
      if(in.eof()) return in;
      cp = in.peek();
    }

  }

  return in;
}

void Shape::writeSexpr(ostream &out)
{
  list<Point>::iterator iter;
  bool first_time = true;

  out << "#(<shape>";
  out << " " << label;
  location.writeSexpr(out);
  out << " " << color;
  out << " " << area;
  centroid.writeSexpr(out);
  upper_left.writeSexpr(out);
  lower_right.writeSexpr(out);

  out << "#(<moments>";
  for (int i = 0; i < 8; ++i)
    out << " " << moments[i];
  out << ")";

  out << " " << flow;

  // boundary
  /*out << " (";
  for (iter = points.begin(); iter != points.end(); iter++) {
    if (first_time)
      first_time = false;
    else
      out << " ";
    (*iter).writeSexpr(out);
  }
  out << ")";*/
  out << ")";
}

// -------------------------------------
// get and set compare
// -------------------------------------
int Shape::compare(int c) {
  int old = _compare;
  _compare = c;
  return old;
}
int Shape::compare() {
  return _compare;
}

// -------------------------------------
// contains
// -------------------------------------
bool Shape::contains(Shape &s) {

  // check bounding box
  if(!((s.location.x >= upper_left.x && s.location.x <= lower_right.x) &&
       (s.location.y >= upper_left.y && s.location.y <= lower_right.y))) {
    return false;
  }

  // check edges
  if(s.location.x == 0 || s.location.y == 0) {
    return false;
  }

  bool outside1, outside2, outside3, outside4;
  outside1 = outside2 = outside3 = outside4 = true;

  // check boundary points
  list<Point>::iterator iter;
  Point last = *(points.begin());
  for(iter = points.begin(); iter != points.end(); iter++) {

    // check in the x direction
    if((*iter).y == s.location.y && (*iter).x < s.location.x) {
      if((*iter).y != last.y) {
        outside1 = !outside1;
      }
    }
    if((*iter).y == s.location.y && (*iter).x > s.location.x) {
      if((*iter).y != last.y) {
        outside2 = !outside2;
      }
    }

    // check in the y direction
    if((*iter).x == s.location.x && (*iter).y < s.location.y) {
      if((*iter).x != last.x) {
        outside3 = !outside3;
      }
    }
    if((*iter).x == s.location.x && (*iter).y > s.location.y) {
      if((*iter).x != last.x) {
        outside4 = !outside4;
      }
    }

    last = *iter;

  }

  // is this point on the outside?
  if(outside1 && outside2) {
    return false;
  }
  if(outside3 && outside4) {
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
  bool rslt = false;
  switch(_compare) {
    case AREA:
      rslt = (area == s.area);
      break;
    case LABEL:
      rslt = (label == s.label);
      break;
    case COLOR:
      rslt = (color == s.color);
      break;
    case LOCATION:
      rslt = (location == s.location);
      break;
    case X:
      rslt = (moments[6] == s.moments[6]);
      break;
    case Y:
      rslt = (moments[7] == s.moments[7]);
      break;
    case FLOW:
      rslt = (flow == s.flow);
      break;
  }
  return rslt;
}

// -------------------------------------
// operator<
//
// this is used to sort lists of shapes
// -------------------------------------
bool Shape::operator<(Shape &s) {
  bool rslt = false;
  switch(_compare) {
    case AREA:
      rslt = (area > s.area);
      break;
    case LABEL:
      rslt = (label > s.label);
      break;
    case COLOR:
      rslt = (color > s.color);
      break;
    case LOCATION:
      rslt = (location > s.location);
      break;
    case X:
      rslt = (moments[6] > s.moments[6]);
      break;
    case Y:
      rslt = (moments[7] > s.moments[7]);
      break;
    case FLOW:
      rslt = (flow > s.flow);
      break;
  }
  return rslt;
}

// -------------------------------------
// operator>
//
// this is used to sort lists of shapes
// -------------------------------------
bool Shape::operator>(Shape &s) {
  bool rslt = false;
  switch(_compare) {
    case AREA:
      rslt = (area < s.area);
      break;
    case LABEL:
      rslt = (label < s.label);
      break;
    case COLOR:
      rslt = (color < s.color);
      break;
    case LOCATION:
      rslt = (location < s.location);
      break;
    case X:
      rslt = (moments[6] < s.moments[6]);
      break;
    case Y:
      rslt = (moments[7] < s.moments[7]);
      break;
    case FLOW:
      rslt = (flow < s.flow);
      break;
  }
  return rslt;
}


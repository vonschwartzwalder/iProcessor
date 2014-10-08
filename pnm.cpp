//---------------------------------------------------------------------
//
//---------------------------------------------------------------------

// ------------------------
// PNM class implementation
// ------------------------

// includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pnm.h"

// file scope function
double CatMull(double, double, double, double, double);

// initialize constants
const int PNM::typeNON = 0;
const int PNM::typePBM = 1;
const int PNM::typePGM = 2;
const int PNM::typePPM = 3;

const int PNM::planeR = 0;
const int PNM::planeG = 1;
const int PNM::planeB = 2;
const int PNM::planeH = 0;
const int PNM::planeS = 1;
const int PNM::planeV = 2;

const int PNM::NEIGHBOR = 0;
const int PNM::LINEAR = 1;
const int PNM::CUBIC = 2;

PNM::PNM() {
  _raw = true;
  _rows = 0;
  _cols = 0;
  _max_value = 255;
  _type = typeNON;
  _threshold = 127;
  _interpolationMethod = NEIGHBOR;
  bit_image = NULL;
  gray_image = NULL;
  color_image = NULL;
}

PNM::PNM(const PNM &p) {
  _raw = p._raw;
  _rows = p._rows;
  _cols = p._cols;
  _max_value = p._max_value;
  _type = p._type;
  _threshold = p._threshold;
  _interpolationMethod = p._interpolationMethod;
  bit_image = NULL;
  gray_image = NULL;
  color_image = NULL;
  newImage(_type);
  switch (_type) {
  case typePBM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        bit_image[x][y] = p.bit_image[x][y];
      }
    }
    break;
  case typePGM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        gray_image[x][y] = p.gray_image[x][y];
      }
    }
    break;
  case typePPM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        color_image[x][y] = p.color_image[x][y];
      }
    }
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
    break;
  }
}

PNM::PNM(char *filename) {
  _raw = true;
  _rows = 0;
  _cols = 0;
  _max_value = 255;
  _threshold = 127;
  _type = typeNON;
  _interpolationMethod = NEIGHBOR;
  bit_image = NULL;
  gray_image = NULL;
  color_image = NULL;
  read(filename);
}

PNM::PNM(string filename) {
  _raw = true;
  _rows = 0;
  _cols = 0;
  _max_value = 255;
  _threshold = 127;
  _type = typeNON;
  _interpolationMethod = NEIGHBOR;
  bit_image = NULL;
  gray_image = NULL;
  color_image = NULL;
  read(filename);
}

PNM::PNM(int ncols, int nrows, int type) {
  _raw = true;
  _rows = nrows;
  _cols = ncols;
  _max_value = 255;
  _threshold = 127;
  _interpolationMethod = NEIGHBOR;
  bit_image = NULL;
  gray_image = NULL;
  color_image = NULL;
  this->_type = type;
  newImage(_type);
}

PNM::~PNM() {
  deleteImage(typeNON);
}

void PNM::newImage(int type) {
  switch (type) {
  case typePBM:
    bit_image = new bool *[_cols];
    for (int i = 0; i < _cols; i++) {
      bit_image[i] = new bool[_rows];
    }
    break;
  case typePGM:
    gray_image = new int *[_cols];
    for (int i = 0; i < _cols; i++) {
      gray_image[i] = new int[_rows];
    }
    break;
  case typePPM:
    color_image = new PNM_Color *[_cols];
    for (int i = 0; i < _cols; i++) {
      color_image[i] = new PNM_Color[_rows];
    }
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
    break;
  }
}

void PNM::deleteImage(int type) {
  switch (type) {
  case typePBM:
    if (bit_image != NULL) {
      for (int i = 0; i < _cols; i++) {
        delete bit_image[i];
      }
      delete bit_image;
      bit_image = NULL;
    }
    break;
  case typePGM:
    if (gray_image != NULL) {
      for (int i = 0; i < _cols; i++) {
        delete gray_image[i];
      }
      delete gray_image;
      gray_image = NULL;
    }
    break;
  case typePPM:
    if (color_image != NULL) {
      for (int i = 0; i < _cols; i++) {
        delete color_image[i];
      }
      delete color_image;
      color_image = NULL;
    }
    break;
  case typeNON:
    if (bit_image != NULL) {
      for (int i = 0; i < _cols; i++) {
        delete bit_image[i];
      }
      delete bit_image;
      bit_image = NULL;
    }
    if (gray_image != NULL) {
      for (int i = 0; i < _cols; i++) {
        delete gray_image[i];
      }
      delete gray_image;
      gray_image = NULL;
    }
    if (color_image != NULL) {
      for (int i = 0; i < _cols; i++) {
        delete color_image[i];
      }
      delete color_image;
      color_image = NULL;
    }
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
    break;
  }
}

bool PNM::valid(const int col, const int row) {
  if (col < 0) return false;
  if (row < 0) return false;
  if (col >= _cols) return false;
  if (row >= _rows) return false;
  return true;
}

bool PNM::valid(const double col, const double row) {
  if (col < 0) return false;
  if (row < 0) return false;
  if (col >= (double)_cols) return false;
  if (row >= (double)_rows) return false;
  return true;
}

PNM &PNM::operator=(PNM p) {
  _raw = p._raw;
  _rows = p._rows;
  _cols = p._cols;
  _max_value = p._max_value;
  _type = p._type;
  _threshold = p._threshold;
  newImage(_type);
  switch (_type) {
  case typePBM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        bit_image[x][y] = p.bit_image[x][y];
      }
    }
    break;
  case typePGM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        gray_image[x][y] = p.gray_image[x][y];
      }
    }
    break;
  case typePPM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        color_image[x][y] = p.color_image[x][y];
      }
    }
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
    break;
  }
  return *this;
}

PNM &PNM::operator=(bool c) {
  switch (_type) {
  case typePBM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        bit_image[x][y] = c;
      }
    }
    break;
  case typePGM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        gray_image[x][y] = (c ? 255 : 0);
      }
    }
    break;
  case typePPM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        color_image[x][y] = (c ? PNM_Color(255, 255, 255) : PNM_Color(0, 0, 0));
      }
    }
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
    break;
  }
  return *this;
}

PNM &PNM::operator=(int c) {
  switch (_type) {
  case typePBM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        bit_image[x][y] = (c > 0 ? true : false);
      }
    }
    break;
  case typePGM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        gray_image[x][y] = c;
      }
    }
    break;
  case typePPM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        color_image[x][y] = PNM_Color(c, c, c);
      }
    }
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
    break;
  }
  return *this;
}

PNM &PNM::operator=(PNM_Color c) {
  switch (_type) {
  case typePBM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        bit_image[x][y] = c.bit();
      }
    }
    break;
  case typePGM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        gray_image[x][y] = c.gray();
      }
    }
    break;
  case typePPM:
    for (int y = 0; y < _rows; y++) {
      for (int x = 0; x < _cols; x++) {
        color_image[x][y] = c;
      }
    }
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
    break;
  }
  return *this;
}

bool PNM::bit(int col, int row) {
  if (!valid(col, row)) {
    throw PNM_ArrayException(col, row);
  }
  switch (_type) {
  case typePBM:
    _bit = bit_image[col][row];
    break;
  case typePGM:
    _bit = false;
    if (gray_image[col][row] > _threshold) {
      _bit = true;
    }
    break;
  case typePPM:
    _bit = false;
    _color = color_image[col][row];
    _gray = (int)(_color.red() * 0.299 + _color.green() * 0.587 + _color.blue() * 0.114);
    if (_gray > _threshold) {
      _bit = true;
    }
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
  }
  return _bit;
}

int PNM::gray(int col, int row) {
  if (!valid(col, row)) {
    throw PNM_ArrayException(col, row);
  }
  switch (_type) {
  case typePBM:
    _gray = (int)bit_image[col][row];
    break;
  case typePGM:
    _gray = gray_image[col][row];
    break;
  case typePPM:
    _color = color_image[col][row];
    _gray = (int)(_color.red() * 0.299 + _color.green() * 0.587 + _color.blue() * 0.114);
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
  }
  return _gray;
}

PNM_Color PNM::color(int col, int row) {
  if (!valid(col, row)) {
    throw PNM_ArrayException(col, row);
  }
  switch (_type) {
  case typePBM:
    _color.red((int)bit_image[col][row]);
    _color.green(_color.red());
    _color.blue(_color.red());
    break;
  case typePGM:
    _color.red((int)gray_image[col][row]);
    _color.green(_color.red());
    _color.blue(_color.red());
    break;
  case typePPM:
    _color = color_image[col][row];
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
  }
  return _color;
}

PNM_Color PNM::hsv(int col, int row) {
  if (!valid(col, row)) {
    throw PNM_ArrayException(col, row);
  }
  double var_R, var_G, var_B;
  double var_Min, var_Max, del_Max;
  double H, S, V;
  pair<double, double> result;
  switch (_type) {
  case typePBM:
    _color.red((int)bit_image[col][row]);
    _color.green(_color.red());
    _color.blue(_color.red());
    break;
  case typePGM:
    _color.red((int)gray_image[col][row]);
    _color.green(_color.red());
    _color.blue(_color.red());
    break;
  case typePPM:
    _color = color_image[col][row];

      // RGB to HSV
      var_R = (_color.red() / 255.0);   //RGB from 0 to 255
      var_G = (_color.green() / 255.0);
      var_B = (_color.blue() / 255.0);

      var_Min = MIN(var_R, var_G, var_B);
      var_Max = MAX(var_R, var_G, var_B);
      del_Max = var_Max - var_Min;  //Delta RGB value

      V = var_Max;

      if (del_Max == 0.0) { // This is a gray, no chroma...
        H = 0.0;            // HSV results from 0 to 1
        S = 0.0;
      } else {            // Chromatic data...
        S = del_Max / var_Max;

        double del_R = (((var_Max - var_R) / 6.0) + (del_Max / 2.0)) / del_Max;
        double del_G = (((var_Max - var_G) / 6.0) + (del_Max / 2.0)) / del_Max;
        double del_B = (((var_Max - var_B) / 6.0) + (del_Max / 2.0)) / del_Max;

        if      (var_R == var_Max) H = del_B - del_G;
        else if (var_G == var_Max) H = (1.0 / 3.0) + del_R - del_B;
        else if (var_B == var_Max) H = (2.0 / 3.0) + del_G - del_R;

        if (H < 0.0) H += 1.0;
        if (H > 1.0) H -= 1.0;
      }
      _color.red((int)(H * 255.0));
      _color.green((int)(S * 255.0));
      _color.blue((int)(V * 255.0));
      break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
  }
  return _color;
}

bool PNM::bit(int col, int row, bool val) {
  if (!valid(col, row)) {
    throw PNM_ArrayException(col, row);
  }
  bool old = bit(col, row);
  switch (_type) {
  case typePBM:
    bit_image[col][row] = val;
    break;
  case typePGM:
    gray_image[col][row] = (val) ? 255 : 0;
    break;
  case typePPM:
    _color.red((val) ? 255 : 0);
    _color.green(_color.red());
    _color.blue(_color.red());
    color_image[col][row] = _color;
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
  }
  return old;
}

int PNM::gray(int col, int row, int val) {
  if (!valid(col, row)) {
    throw PNM_ArrayException(col, row);
  }
  int old = gray(col, row);
  switch (_type) {
  case typePBM:
    bit_image[col][row] = (val > _threshold) ? 255 : 0;
    break;
  case typePGM:
    gray_image[col][row] = val;
    break;
  case typePPM:
    _color.red(val);
    _color.green(_color.red());
    _color.blue(_color.red());
    color_image[col][row] = _color;
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
  }
  return old;
}

PNM_Color PNM::color(int col, int row, PNM_Color val) {
  if (!valid(col, row)) {
    throw PNM_ArrayException(col, row);
  }
  PNM_Color old = color(col, row);
  switch (_type) {
  case typePBM:
    _gray = (int)(val.red() * 0.299 + val.green() * 0.587 + val.blue() * 0.114);
    bit_image[col][row] = (_gray > _threshold) ? 255 : 0;
    break;
  case typePGM:
    _gray = (int)(val.red() * 0.299 + val.green() * 0.587 + val.blue() * 0.114);
    gray_image[col][row] = _gray;
    break;
  case typePPM:
    color_image[col][row] = val;
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
  }
  return old;
}

void PNM::drawLine(Point p0, Point p1, PNM_Color clr) {
  drawLine(p0.x, p0.y, p1.x, p1.y, clr);
}

void PNM::drawLine(int scol, int srow, int ecol, int erow, PNM_Color clr) {
  if (!valid(scol, srow)) {
    throw PNM_ArrayException(scol, srow);
  }
  if (!valid(ecol, erow)) {
    throw PNM_ArrayException(ecol, erow);
  }

  // initial points
  int x0 = scol;
  int x1 = ecol;
  int y0 = srow;
  int y1 = erow;

  // initial values
  int dx = x1 - x0;
  int dy = y1 - y0;
  int x = x0;
  int y = y0;

  // write first pixel
  color(x, y, clr);

  // dx and dy positive, dy <= dx
  if (dx >= 0 && dy >= 0 && dy <= dx) {
    int d = 2 * (dy - dx);
    int incrE = 2 * dy;
    int incrNE = 2 * (dy - dx);
    while (x < x1) {
      if (d <= 0) {
        d += incrE;
        x++;
      }
      else {
        d += incrNE;
        x++;
        y++;
      }
      color(x, y, clr);
    }
  }

  // dx and dy positive, dy > dx
  else if (dx >= 0 && dy >= 0 && dy > dx) {
    int d = 2 * (dx - dy);
    int incrE = 2 * dx;
    int incrNE = 2 * (dx - dy);
    while (y < y1) {
      if (d <= 0) {
        d += incrE;
        y++;
      }
      else {
        d += incrNE;
        x++;
        y++;
      }
      color(x, y, clr);
    }
  }

  // dx and dy negative, dy >= dx
  else if (dx < 0 && dy < 0 && dy >= dx) {
    int d = 2 * (dy - dx);
    int incrE = 2 * dy;
    int incrNE = 2 * (dy - dx);
    while (x > x1) {
      if (d <= 0) {
        d -= incrE;
        x--;
      }
      else {
        d -= incrNE;
        x--;
        y--;
      }
      color(x, y, clr);
    }
  }

  // dx and dy negative, dy < dx
  else if (dx < 0 && dy < 0 && dy < dx) {
    int d = 2 * (dx - dy);
    int incrE = 2 * dx;
    int incrNE = 2 * (dx - dy);
    while (y > y1) {
      if (d <= 0) {
        d -= incrE;
        y--;
      }
      else {
        d -= incrNE;
        x--;
        y--;
      }
      color(x, y, clr);
    }
  }

  // dx positive and dy negative, -dy <= dx
  else if (dx >= 0 && dy < 0 && -dy <= dx) {
    int d = 2 * (-dy - dx);
    int incrE = 2 * -dy;
    int incrNE = 2 * (-dy - dx);
    while (x < x1) {
      if (d <= 0) {
        d += incrE;
        x++;
      }
      else {
        d += incrNE;
        x++;
        y--;
      }
      color(x, y, clr);
    }
  }

  // dx positive and dy negative, -dy > dx
  else if (dx >= 0 && dy < 0 && -dy > dx) {
    int d = 2 * (dx - -dy);
    int incrE = 2 * dx;
    int incrNE = 2 * (dx - -dy);
    while (y > y1) {
      if (d <= 0) {
        d += incrE;
        y--;
      }
      else {
        d += incrNE;
        x++;
        y--;
      }
      color(x, y, clr);
    }
  }

  // dx negative and dy positive, dy <= -dx
  else if (dx < 0 && dy >= 0 && dy <= -dx) {
    int d = 2 * (dy - -dx);
    int incrE = 2 * dy;
    int incrNE = 2 * (dy - -dx);
    while (x > x1) {
      if (d <= 0) {
        d += incrE;
        x--;
      }
      else {
        d += incrNE;
        x--;
        y++;
      }
      color(x, y, clr);
    }
  }

  // dx negative and dy positive, dy > -dx
  else if (dx < 0 && dy >= 0 && dy > -dx) {
    int d = 2 * (-dx - dy);
    int incrE = 2 * -dx;
    int incrNE = 2 * (-dx - dy);
    while (y < y1) {
      if (d <= 0) {
        d += incrE;
        y++;
      }
      else {
        d += incrNE;
        x--;
        y++;
      }
      color(x, y, clr);
    }
  }

  else {
    throw(PNM_Exception(string("impossible line")));
  }

}

// anti-aliased line?
// TODO: fix this to give a smooth line
void PNM::drawLine(double scol, double srow, double ecol, double erow, PNM_Color clr) {
  if (!valid(scol, srow)) {
    throw PNM_ArrayException(scol, srow);
  }
  if (!valid(ecol, erow)) {
    throw PNM_ArrayException(ecol, erow);
  }

  // initial points
  double x0 = scol;
  double x1 = ecol;
  double y0 = srow;
  double y1 = erow;

  // initial values
  double dx = x1 - x0;
  double dy = y1 - y0;
  double x = x0;
  double y = y0;

  // write first pixel
  try {
    color(x, y, clr);
  }
  catch (PNM_ArrayException e) {}

  // dx and dy positive, dy <= dx
  if (dx >= 0 && dy >= 0 && dy <= dx) {
    double d = 2 * (dy - dx);
    double incrE = 2 * dy;
    double incrNE = 2 * (dy - dx);
    while (x < x1) {
      if (d <= 0) {
        d += incrE;
        x++;
      }
      else {
        d += incrNE;
        x++;
        y++;
      }
      try {
        color(x, y, clr);
      }
      catch (PNM_ArrayException e) {}
    }
  }

  // dx and dy positive, dy > dx
  else if (dx >= 0 && dy >= 0 && dy > dx) {
    double d = 2 * (dx - dy);
    double incrE = 2 * dx;
    double incrNE = 2 * (dx - dy);
    while (y < y1) {
      if (d <= 0) {
        d += incrE;
        y++;
      }
      else {
        d += incrNE;
        x++;
        y++;
      }
      try {
        color(x, y, clr);
      }
      catch (PNM_ArrayException e) {}
    }
  }

  // dx and dy negative, dy >= dx
  else if (dx < 0 && dy < 0 && dy >= dx) {
    double d = 2 * (dy - dx);
    double incrE = 2 * dy;
    double incrNE = 2 * (dy - dx);
    while (x > x1) {
      if (d <= 0) {
        d -= incrE;
        x--;
      }
      else {
        d -= incrNE;
        x--;
        y--;
      }
      try {
        color(x, y, clr);
      }
      catch (PNM_ArrayException e) {}
    }
  }

  // dx and dy negative, dy < dx
  else if (dx < 0 && dy < 0 && dy < dx) {
    double d = 2 * (dx - dy);
    double incrE = 2 * dx;
    double incrNE = 2 * (dx - dy);
    while (y > y1) {
      if (d <= 0) {
        d -= incrE;
        y--;
      }
      else {
        d -= incrNE;
        x--;
        y--;
      }
      try {
        color(x, y, clr);
      }
      catch (PNM_ArrayException e) {}
    }
  }

  // dx positive and dy negative, -dy <= dx
  else if (dx >= 0 && dy < 0 && -dy <= dx) {
    double d = 2 * (-dy - dx);
    double incrE = 2 * -dy;
    double incrNE = 2 * (-dy - dx);
    while (x < x1) {
      if (d <= 0) {
        d += incrE;
        x++;
      }
      else {
        d += incrNE;
        x++;
        y--;
      }
      try {
        color(x, y, clr);
      }
      catch (PNM_ArrayException e) {}
    }
  }

  // dx positive and dy negative, -dy > dx
  else if (dx >= 0 && dy < 0 && -dy > dx) {
    double d = 2 * (dx - -dy);
    double incrE = 2 * dx;
    double incrNE = 2 * (dx - -dy);
    while (y > y1) {
      if (d <= 0) {
        d += incrE;
        y--;
      }
      else {
        d += incrNE;
        x++;
        y--;
      }
      try {
        color(x, y, clr);
      }
      catch (PNM_ArrayException e) {}
    }
  }

  // dx negative and dy positive, dy <= -dx
  else if (dx < 0 && dy >= 0 && dy <= -dx) {
    double d = 2 * (dy - -dx);
    double incrE = 2 * dy;
    double incrNE = 2 * (dy - -dx);
    while (x > x1) {
      if (d <= 0) {
        d += incrE;
        x--;
      }
      else {
        d += incrNE;
        x--;
        y++;
      }
      try {
        color(x, y, clr);
      }
      catch (PNM_ArrayException e) {}
    }
  }

  // dx negative and dy positive, dy > -dx
  else if (dx < 0 && dy >= 0 && dy > -dx) {
    double d = 2 * (-dx - dy);
    double incrE = 2 * -dx;
    double incrNE = 2 * (-dx - dy);
    while (y < y1) {
      if (d <= 0) {
        d += incrE;
        y++;
      }
      else {
        d += incrNE;
        x--;
        y++;
      }
      try {
        color(x, y, clr);
      }
      catch (PNM_ArrayException e) {}
    }
  }

  else {
    throw(PNM_Exception(string("impossible line")));
  }

}

// clear the image
void PNM::clear(int value) {
  switch (_type) {
  case typePBM:
    for (int x = 0; x < _cols; x++) {
      for (int y = 0; y < _rows; y++) {
        bit_image[x][y] = value > 127 ? true : false;
      }
    }
    break;
  case typePGM:
    for (int x = 0; x < _cols; x++) {
      for (int y = 0; y < _rows; y++) {
        gray_image[x][y] = value;
      }
    }
    break;
  case typePPM:
    for (int x = 0; x < _cols; x++) {
      for (int y = 0; y < _rows; y++) {
        color_image[x][y] = PNM_Color(value, value, value);
      }
    }
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
  }
}

void PNM::convert(int type) {
  switch (type) {
  case typePBM:
    switch (_type) {
    case typePBM:
      break;
    case typePGM:
      newImage(typePBM);
      for (int x = 0; x < _cols; x++) {
        for (int y = 0; y < _rows; y++) {
          bit_image[x][y] = bit(x, y);
        }
      }
      deleteImage(typePGM);
      _type = typePBM;
      break;
    case typePPM:
      newImage(typePBM);
      for (int x = 0; x < _cols; x++) {
        for (int y = 0; y < _rows; y++) {
          bit_image[x][y] = bit(x, y);
        }
      }
      deleteImage(typePPM);
      _type = typePBM;
      break;
    }
    break;
  case typePGM:
    switch (_type) {
    case typePBM:
      newImage(typePGM);
      for (int x = 0; x < _cols; x++) {
        for (int y = 0; y < _rows; y++) {
          gray_image[x][y] = gray(x, y);
        }
      }
      deleteImage(typePBM);
      _type = typePGM;
      break;
    case typePGM:
      break;
    case typePPM:
      newImage(typePGM);
      for (int x = 0; x < _cols; x++) {
        for (int y = 0; y < _rows; y++) {
          gray_image[x][y] = gray(x, y);
        }
      }
      deleteImage(typePPM);
      _type = typePGM;
      break;
    }
    break;
  case typePPM:
    switch (_type) {
    case typePBM:
      newImage(typePPM);
      for (int x = 0; x < _cols; x++) {
        for (int y = 0; y < _rows; y++) {
          color_image[x][y] = color(x, y);
        }
      }
      deleteImage(typePBM);
      _type = typePPM;
      break;
    case typePGM:
      newImage(typePPM);
      for (int x = 0; x < _cols; x++) {
        for (int y = 0; y < _rows; y++) {
          color_image[x][y] = color(x, y);
        }
      }
      deleteImage(typePGM);
      _type = typePPM;
      break;
    case typePPM:
      break;
    }
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
  }
}

void PNM::quantize(int plane, int levels) {
  int k = 255 / levels;
  switch (_type) {
    case typePBM:
      newImage(typePGM);
      for (int x = 0; x < _cols; x++) {
        for (int y = 0; y < _rows; y++) {
          gray_image[x][y] = k * (gray(x, y) / k);
        }
      }
      deleteImage(typePBM);
      _type = typePGM;
      break;
    case typePGM:
      for (int x = 0; x < _cols; x++) {
        for (int y = 0; y < _rows; y++) {
          gray_image[x][y] = k * (gray(x, y) / k);
        }
      }
      break;
    case typePPM:
      newImage(typePGM);
      for (int x = 0; x < _cols; x++) {
        for (int y = 0; y < _rows; y++) {
          switch(plane) {
            case planeR:
          gray_image[x][y] = k * (color(x, y).red() / k);
              break;
            case planeG:
          gray_image[x][y] = k * (color(x, y).green() / k);
              break;
            case planeB:
          gray_image[x][y] = k * (color(x, y).blue() / k);
              break;
          }
        }
      }
      deleteImage(typePPM);
      _type = typePGM;
      break;
  }
}

void PNM::plane(int plane) {
  switch (_type) {
    case typePBM:
      newImage(typePGM);
      for (int x = 0; x < _cols; x++) {
        for (int y = 0; y < _rows; y++) {
          gray_image[x][y] = gray(x, y);
        }
      }
      deleteImage(typePBM);
      _type = typePGM;
      break;
    case typePGM:
      break;
    case typePPM:
      newImage(typePGM);
      for (int x = 0; x < _cols; x++) {
        for (int y = 0; y < _rows; y++) {
          switch(plane) {
            case planeR:
          gray_image[x][y] = color(x, y).red();
              break;
            case planeG:
          gray_image[x][y] = color(x, y).green();
              break;
            case planeB:
          gray_image[x][y] = color(x, y).blue();
              break;
          }
        }
      }
      deleteImage(typePPM);
      _type = typePGM;
      break;
  }
}

void PNM::toHSV() {
  if (_type != typePPM) {
    return;
  }
  for (int x = 0; x < _cols; x++) {
    for (int y = 0; y < _rows; y++) {
      color_image[x][y] = hsv(x, y);
    }
  }
}

void PNM::read(char *filename) {
  ifstream infile;
  if (strcmp("stdin", filename)) {
    infile.open(filename, ios::in | ios::binary);
    if (!infile) {
      ostringstream out;
      out << "unable to open file " << filename;
      throw PNM_IOException(out.str());
    }
    read(infile);
  }
  else {
    read(cin);
  }
}

void PNM::read(string filename) {
  ifstream infile;
  if (filename != "stdin") {
    infile.open(filename.c_str(), ios::in | ios::binary);
    if (!infile) {
      ostringstream out;
      out << "unable to open file " << filename;
      throw PNM_IOException(out.str());
    }
    read(infile);
  }
  else {
    read(cin);
  }
}

void PNM::read(istream &instream) {

  int   row, col;
  int   value;
  char  c1, c2;
  unsigned char cp;

  // clean up original image
  _cols = 0;
  _rows = 0;
  deleteImage(_type);
  _type = typeNON;

  // check magic number
  c1 = instream.get();
  c2 = instream.get();
  if (c1 == 'P') {
    if (c2 == '1') {
      _type = typePBM;
      _raw = false;
    }
    else if (c2 == '2') {
      _type = typePGM;
      _raw = false;
    }
    else if (c2 == '3') {
      _type = typePPM;
      _raw = false;
    }
    else if (c2 == '4') {
      _type = typePBM;
      _raw = true;
    }
    else if (c2 == '5') {
      _type = typePGM;
      _raw = true;
    }
    else if (c2 == '6') {
      _type = typePPM;
      _raw = true;
    }
    else {
      ostringstream out;
      out << "bad magic number (" << c1 << c2 << ")";
      throw PNM_IOException(out.str());
    }
  }
  else {
    ostringstream out;
    out << "bad magic number (" << c1 << ")";
    throw PNM_IOException(out.str());
  }

  // skip white space
  cp = instream.peek();
  while (cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
    instream.get();
    cp = instream.peek();
  }

  // skip any comments
  while (instream.peek() == '#') {
    while (instream.get() != '\n');
  }

  // get rows and columns
  instream >> _cols;
  instream >> _rows;

  // skip white space
  cp = instream.peek();
  while (cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
    instream.get();
    cp = instream.peek();
  }

  // skip any comments
  while (instream.peek() == '#') {
    while (instream.get() != '\n');
  }

  // skip white space
  cp = instream.peek();
  while (cp == ' ' || cp == '\t' || cp == '\n' || cp == '\r') {
    instream.get();
    cp = instream.peek();
  }

  // get max_value
  if (_type != typePBM) {
    instream >> _max_value;
  }

  // skip white space (one character)
  instream.get();

  // allocate new image array
  newImage(_type);

  // read the array
  for (row = 0; row < _rows; row++) {
    for (col = 0; col < _cols; col++) {
      switch (_type) {
      case typePBM:
        if (_raw) {
          cp = instream.get();
          if (col < _cols) bit_image[col++][row] = (cp & 0x80) ? 255 : 0;
          if (col < _cols) bit_image[col++][row] = (cp & 0x40) ? 255 : 0;
          if (col < _cols) bit_image[col++][row] = (cp & 0x20) ? 255 : 0;
          if (col < _cols) bit_image[col++][row] = (cp & 0x10) ? 255 : 0;
          if (col < _cols) bit_image[col++][row] = (cp & 0x08) ? 255 : 0;
          if (col < _cols) bit_image[col++][row] = (cp & 0x04) ? 255 : 0;
          if (col < _cols) bit_image[col++][row] = (cp & 0x02) ? 255 : 0;
          if (col < _cols) bit_image[col][row] = (cp & 0x01) ? 255 : 0;
        }
        else {
          instream >> value;
          value = (value > 0) ? 255 : 0;
          bit_image[col][row] = value;
        }
        break;
      case typePGM:
        if (_raw) {
          cp = instream.get();
          gray_image[col][row] = (int)cp;
        }
        else {
          instream >> value;
          gray_image[col][row] = (int)value;
        }
        break;
      case typePPM:
        if (_raw) {
          cp = instream.get();
          _color.red((int)cp);
          cp = instream.get();
          _color.green((int)cp);
          cp = instream.get();
          _color.blue((int)cp);
          color_image[col][row] = _color;
        }
        else {
          instream >> value;
          _color.red((int)value);
          instream >> value;
          _color.green((int)value);
          instream >> value;
          _color.blue((int)value);
          color_image[col][row] = _color;
        }
        break;
      default:
        ostringstream out;
        out << "unknown type: " << _type;
        throw PNM_Exception(out.str());
        break;
      }
    }
  }

} // read

void PNM::write(char *filename) {
  ofstream outfile;
  if (strcmp("stdout", filename)) {
    outfile.open(filename, ios::out | ios::binary);
    if (!outfile) exit(-1);
    if (!outfile) {
      ostringstream out;
      out << "unable to open file " << filename;
      throw PNM_IOException(out.str());
    }
    write(outfile);
  }
  else {
    write(cout);
  }
}

void PNM::write(string filename) {
  ofstream outfile;
  if (filename != "stdout") {
    outfile.open(filename.c_str(), ios::out | ios::binary);
    if (!outfile) {
      ostringstream out;
      out << "unable to open file " << filename;
      throw PNM_IOException(out.str());
    }
    write(outfile);
  }
  else {
    write(cout);
  }
}

void PNM::write(ostream &outstream) {

  int row, col;
  int value;
  int pixels = 0;
  unsigned char cp;

  // write magic
  switch (_type) {
  case typePBM:
    outstream << (_raw ? "P4" : "P1") << "\n";
    break;
  case typePGM:
    outstream << (_raw ? "P5" : "P2") << "\n";
    break;
  case typePPM:
    outstream << (_raw ? "P6" : "P3") << "\n";
    break;
  default:
    ostringstream out;
    out << "unknown type: " << _type;
    throw PNM_Exception(out.str());
    break;
  }

  // advertise
  outstream << "# PNM class written by Duane Schwartzwald" << "\n";

  // write size and max_value
  outstream << _cols << " " << _rows;
  if (_type != typePBM) {
    outstream << "\n" << _max_value;
  }
  outstream.put(0x0A);  // single whitespace character

  // write array
  for (row = 0; row < _rows; row++) {
    for (col = 0; col < _cols; col++) {
      switch (_type) {
      case typePBM:
        if (_raw) {
          cp = 0;
          cp |= (col < _cols) ? (bit_image[col++][row]) ? 0x80 : 0 : 0;
          cp |= (col < _cols) ? (bit_image[col++][row]) ? 0x40 : 0 : 0;
          cp |= (col < _cols) ? (bit_image[col++][row]) ? 0x20 : 0 : 0;
          cp |= (col < _cols) ? (bit_image[col++][row]) ? 0x10 : 0 : 0;
          cp |= (col < _cols) ? (bit_image[col++][row]) ? 0x08 : 0 : 0;
          cp |= (col < _cols) ? (bit_image[col++][row]) ? 0x04 : 0 : 0;
          cp |= (col < _cols) ? (bit_image[col++][row]) ? 0x02 : 0 : 0;
          cp |= (col < _cols) ? (bit_image[col][row]) ? 0x01 : 0 : 0;
          outstream.put(cp);
        }
        else {
          pixels++;
          value = (bit_image[col][row]) ? 1 : 0;
          outstream << value << " ";
          if (!(pixels % 30)) {
            outstream << "\n";
          }
        }
        break;
      case typePGM:
        if (_raw) {
          cp = (unsigned char)gray_image[col][row];
          outstream.put(cp);
        }
        else {
          pixels++;
          value = gray_image[col][row];
          outstream << value << " ";
          if (!(pixels % 15)) {
            outstream << "\n";
          }
        }
        break;
      case typePPM:
        if (_raw) {
          cp = (unsigned char)color_image[col][row].red();
          outstream.put(cp);
          cp = (unsigned char)color_image[col][row].green();
          outstream.put(cp);
          cp = (unsigned char)color_image[col][row].blue();
          outstream.put(cp);
        }
        else {
          pixels++;
          value = color_image[col][row].red();
          outstream << value << " ";
          value = color_image[col][row].green();
          outstream << value << " ";
          value = color_image[col][row].blue();
          outstream << value << " ";
          if (!(pixels % 5)) {
            outstream << "\n";
          }
        }
        break;
      default:
        ostringstream out;
        out << "unknown type: " << _type;
        throw PNM_Exception(out.str());
        break;
      }
    }
  }

} // write


// interpolated values
bool PNM::bit(double x, double y) {
  if (!valid(x, y)) {
    throw PNM_ArrayException(x, y);
  }
  int intx, inty;
  switch (_interpolationMethod) {
  case LINEAR:
  case CUBIC:
  case NEIGHBOR:
    intx = (int)x;
    inty = (int)y;
    return bit(intx, inty);
  }
  return false;
}


int PNM::gray(double x, double y) {
  if (!valid(x, y)) {
    throw PNM_ArrayException(x, y);
  }
  int intx, inty;
  double fracx, fracy;
  switch (_interpolationMethod) {
  case NEIGHBOR:
    intx = (int)x;
    inty = (int)y;
    return gray(intx, inty);
  case LINEAR:
    intx = (int)x;
    inty = (int)y;
    fracx = x - intx;
    fracy = y - inty;

    // handle edges and corners
    if (intx == _cols - 1 || inty == _rows - 1) {

      // bottom right corner
      if (intx == _cols - 1 && inty == _rows - 1) {
        return gray(intx, inty);
      }

      // right edge
      if (intx == _cols - 1) {
        return (int)((double)gray(intx, inty) * (1 - fracy) +
          (double)gray(intx, inty + 1) * fracy);
      }

      // bottom edge
      if (inty == _rows - 1) {
        return (int)((double)gray(intx, inty) * (1 - fracx) +
          (double)gray(intx + 1, inty) * fracx);
      }
    }

    // everywhere else
    return (int)((double)gray(intx, inty) * (1 - fracx) * (1 - fracy) +
      (double)gray(intx + 1, inty) * fracx     * (1 - fracy) +
      (double)gray(intx, inty + 1) * (1 - fracx) * fracy +
      (double)gray(intx + 1, inty + 1) * fracx     * fracy);
  case CUBIC:
    intx = (int)x;
    inty = (int)y;
    fracx = x - intx;
    fracy = y - inty;

    // if near the edge, use LINEAR instead
    if (intx < 1 || inty < 1 || intx > _cols - 3 || inty > _rows - 3) {

      // handle edges and corners
      if (intx == _cols - 1 || inty == _rows - 1) {

        // bottom right corner
        if (intx == _cols - 1 && inty == _rows - 1) {
          return gray(intx, inty);
        }

        // right edge
        if (intx == _cols - 1) {
          return (int)((double)gray(intx, inty) * (1 - fracy) +
            (double)gray(intx, inty + 1) * fracy);
        }

        // bottom edge
        if (inty == _rows - 1) {
          return (int)((double)gray(intx, inty) * (1 - fracx) +
            (double)gray(intx + 1, inty) * fracx);
        }
      }

      // everywhere else
      return (int)((double)gray(intx, inty) * (1 - fracx) * (1 - fracy) +
        (double)gray(intx + 1, inty) * fracx     * (1 - fracy) +
        (double)gray(intx, inty + 1) * (1 - fracx) * fracy +
        (double)gray(intx + 1, inty + 1) * fracx     * fracy);
    }

    double y0, y1, y2, y3;
    double x0, x1, x2, x3;

    double t = fracx;

    y0 = gray(intx - 1, inty - 1);
    y1 = gray(intx, inty - 1);
    y2 = gray(intx + 1, inty - 1);
    y3 = gray(intx + 2, inty - 1);

    x0 = CatMull(t, y0, y1, y2, y3);

    y0 = gray(intx - 1, inty);
    y1 = gray(intx, inty);
    y2 = gray(intx + 1, inty);
    y3 = gray(intx + 2, inty);

    x1 = CatMull(t, y0, y1, y2, y3);

    y0 = gray(intx - 1, inty + 1);
    y1 = gray(intx, inty + 1);
    y2 = gray(intx + 1, inty + 1);
    y3 = gray(intx + 2, inty + 1);

    x2 = CatMull(t, y0, y1, y2, y3);

    y0 = gray(intx - 1, inty + 2);
    y1 = gray(intx, inty + 2);
    y2 = gray(intx + 1, inty + 2);
    y3 = gray(intx + 2, inty + 2);

    x3 = CatMull(t, y0, y1, y2, y3);

    t = fracy;

    double result = CatMull(t, x0, x1, x2, x3);
    result = (result < 0) ? 0 : (result > 255) ? 255 : result;
    return (int)result;
  }
  return 0;
}


PNM_Color PNM::color(double x, double y) {
  if (!valid(x, y)) {
    throw PNM_ArrayException(x, y);
  }
  int r, g, b;
  int intx, inty;
  double fracx, fracy;
  switch (_interpolationMethod) {
  case NEIGHBOR:
    intx = (int)x;
    inty = (int)y;
    return color(intx, inty);
  case LINEAR:
    intx = (int)x;
    inty = (int)y;
    fracx = x - intx;
    fracy = y - inty;

    // handle edges and corners
    if (intx == _cols - 1 || inty == _rows - 1) {

      // bottom right corner
      if (intx == _cols - 1 && inty == _rows - 1) {
        return color(intx, inty);
      }

      // right edge
      if (intx == _cols - 1) {
        r = (int)((double)(color(intx, inty).red()) * (1 - fracy) +
          (double)(color(intx, inty + 1).red()) * fracy);
        g = (int)((double)(color(intx, inty).green()) * (1 - fracy) +
          (double)(color(intx, inty + 1).green()) * fracy);
        b = (int)((double)(color(intx, inty).blue()) * (1 - fracy) +
          (double)(color(intx, inty + 1).blue()) * fracy);
        return PNM_Color(r, g, b);
      }

      // bottom edge
      if (inty == _rows - 1) {
        r = (int)((double)(color(intx, inty).red()) * (1 - fracx) +
          (double)(color(intx + 1, inty).red()) * fracx);
        g = (int)((double)(color(intx, inty).green()) * (1 - fracx) +
          (double)(color(intx + 1, inty).green()) * fracx);
        b = (int)((double)(color(intx, inty).blue()) * (1 - fracx) +
          (double)(color(intx + 1, inty).blue()) * fracx);
        return PNM_Color(r, g, b);
      }
    }

    // everywhere else
    r = (int)((double)(color(intx, inty).red()) * (1 - fracx) * (1 - fracy) +
      (double)(color(intx + 1, inty).red()) * fracx     * (1 - fracy) +
      (double)(color(intx, inty + 1).red()) * (1 - fracx) * fracy +
      (double)(color(intx + 1, inty + 1).red()) * fracx     * fracy);
    g = (int)((double)(color(intx, inty).green()) * (1 - fracx) * (1 - fracy) +
      (double)(color(intx + 1, inty).green()) * fracx     * (1 - fracy) +
      (double)(color(intx, inty + 1).green()) * (1 - fracx) * fracy +
      (double)(color(intx + 1, inty + 1).green()) * fracx     * fracy);
    b = (int)((double)(color(intx, inty).blue()) * (1 - fracx) * (1 - fracy) +
      (double)(color(intx + 1, inty).blue()) * fracx     * (1 - fracy) +
      (double)(color(intx, inty + 1).blue()) * (1 - fracx) * fracy +
      (double)(color(intx + 1, inty + 1).blue()) * fracx     * fracy);
    return PNM_Color(r, g, b);
  case CUBIC:
    intx = (int)x;
    inty = (int)y;
    fracx = x - intx;
    fracy = y - inty;

    // if near the edge, use LINEAR instead
    if (intx < 1 || inty < 1 || intx > _cols - 3 || inty > _rows - 3) {

      // handle edges and corners
      if (intx == _cols - 1 || inty == _rows - 1) {

        // bottom right corner
        if (intx == _cols - 1 && inty == _rows - 1) {
          return color(intx, inty);
        }

        // right edge
        if (intx == _cols - 1) {
          r = (int)((double)(color(intx, inty).red()) * (1 - fracy) +
            (double)(color(intx, inty + 1).red()) * fracy);
          g = (int)((double)(color(intx, inty).green()) * (1 - fracy) +
            (double)(color(intx, inty + 1).green()) * fracy);
          b = (int)((double)(color(intx, inty).blue()) * (1 - fracy) +
            (double)(color(intx, inty + 1).blue()) * fracy);
          return PNM_Color(r, g, b);
        }

        // bottom edge
        if (inty == _rows - 1) {
          r = (int)((double)(color(intx, inty).red()) * (1 - fracx) +
            (double)(color(intx + 1, inty).red()) * fracx);
          g = (int)((double)(color(intx, inty).green()) * (1 - fracx) +
            (double)(color(intx + 1, inty).green()) * fracx);
          b = (int)((double)(color(intx, inty).blue()) * (1 - fracx) +
            (double)(color(intx + 1, inty).blue()) * fracx);
          return PNM_Color(r, g, b);
        }
      }

      // everywhere else
      r = (int)((double)(color(intx, inty).red()) * (1 - fracx) * (1 - fracy) +
        (double)(color(intx + 1, inty).red()) * fracx     * (1 - fracy) +
        (double)(color(intx, inty + 1).red()) * (1 - fracx) * fracy +
        (double)(color(intx + 1, inty + 1).red()) * fracx     * fracy);
      g = (int)((double)(color(intx, inty).green()) * (1 - fracx) * (1 - fracy) +
        (double)(color(intx + 1, inty).green()) * fracx     * (1 - fracy) +
        (double)(color(intx, inty + 1).green()) * (1 - fracx) * fracy +
        (double)(color(intx + 1, inty + 1).green()) * fracx     * fracy);
      b = (int)((double)(color(intx, inty).blue()) * (1 - fracx) * (1 - fracy) +
        (double)(color(intx + 1, inty).blue()) * fracx     * (1 - fracy) +
        (double)(color(intx, inty + 1).blue()) * (1 - fracx) * fracy +
        (double)(color(intx + 1, inty + 1).blue()) * fracx     * fracy);
      return PNM_Color(r, g, b);
    }

    double y0, y1, y2, y3;
    double x0, x1, x2, x3;

    double t = fracx;
    double result;

    y0 = color(intx - 1, inty - 1).red();
    y1 = color(intx, inty - 1).red();
    y2 = color(intx + 1, inty - 1).red();
    y3 = color(intx + 2, inty - 1).red();

    x0 = CatMull(t, y0, y1, y2, y3);

    y0 = color(intx - 1, inty).red();
    y1 = color(intx, inty).red();
    y2 = color(intx + 1, inty).red();
    y3 = color(intx + 2, inty).red();

    x1 = CatMull(t, y0, y1, y2, y3);

    y0 = color(intx - 1, inty + 1).red();
    y1 = color(intx, inty + 1).red();
    y2 = color(intx + 1, inty + 1).red();
    y3 = color(intx + 2, inty + 1).red();

    x2 = CatMull(t, y0, y1, y2, y3);

    y0 = color(intx - 1, inty + 2).red();
    y1 = color(intx, inty + 2).red();
    y2 = color(intx + 1, inty + 2).red();
    y3 = color(intx + 2, inty + 2).red();

    x3 = CatMull(t, y0, y1, y2, y3);

    t = fracy;

    result = CatMull(t, x0, x1, x2, x2);
    r = (int)((result < 0) ? 0 : (result > 255) ? 255 : result);

    y0 = color(intx - 1, inty - 1).green();
    y1 = color(intx, inty - 1).green();
    y2 = color(intx + 1, inty - 1).green();
    y3 = color(intx + 2, inty - 1).green();

    x0 = CatMull(t, y0, y1, y2, y3);

    y0 = color(intx - 1, inty).green();
    y1 = color(intx, inty).green();
    y2 = color(intx + 1, inty).green();
    y3 = color(intx + 2, inty).green();

    x1 = CatMull(t, y0, y1, y2, y3);

    y0 = color(intx - 1, inty + 1).green();
    y1 = color(intx, inty + 1).green();
    y2 = color(intx + 1, inty + 1).green();
    y3 = color(intx + 2, inty + 1).green();

    x2 = CatMull(t, y0, y1, y2, y3);

    y0 = color(intx - 1, inty + 2).green();
    y1 = color(intx, inty + 2).green();
    y2 = color(intx + 1, inty + 2).green();
    y3 = color(intx + 2, inty + 2).green();

    x3 = CatMull(t, y0, y1, y2, y3);

    t = fracy;

    result = CatMull(t, x0, x1, x2, x3);
    g = (int)((result < 0) ? 0 : (result > 255) ? 255 : result);

    y0 = color(intx - 1, inty - 1).blue();
    y1 = color(intx, inty - 1).blue();
    y2 = color(intx + 1, inty - 1).blue();
    y3 = color(intx + 2, inty - 1).blue();

    x0 = CatMull(t, y0, y1, y2, y3);

    y0 = color(intx - 1, inty).blue();
    y1 = color(intx, inty).blue();
    y2 = color(intx + 1, inty).blue();
    y3 = color(intx + 2, inty).blue();

    x1 = CatMull(t, y0, y1, y2, y3);

    y0 = color(intx - 1, inty + 1).blue();
    y1 = color(intx, inty + 1).blue();
    y2 = color(intx + 1, inty + 1).blue();
    y3 = color(intx + 2, inty + 1).blue();

    x2 = CatMull(t, y0, y1, y2, y3);

    y0 = color(intx - 1, inty + 2).blue();
    y1 = color(intx, inty + 2).blue();
    y2 = color(intx + 1, inty + 2).blue();
    y3 = color(intx + 2, inty + 2).blue();

    x3 = CatMull(t, y0, y1, y2, y3);

    t = fracy;

    result = CatMull(t, x0, x1, x2, x3);
    b = (int)((result < 0) ? 0 : (result > 255) ? 255 : result);

    return PNM_Color(r, g, b);
  }
  return PNM_Color(0, 0, 0);
}

bool PNM::bit(double x, double y, bool value) {
  if (!valid(x, y)) {
    throw PNM_ArrayException(x, y);
  }
  int old = bit(x, y);
  int intx, inty;
  switch (_interpolationMethod) {
  case CUBIC:
  case LINEAR:
  case NEIGHBOR:
    intx = (int)x;
    inty = (int)y;
    bit(inty, intx, value);
    break;
  } // switch
  return old;
}


int PNM::gray(double x, double y, int value) {
  if (!valid(x, y)) {
    throw PNM_ArrayException(x, y);
  }
  int old = gray(x, y);
  int intx, inty;
  switch (_interpolationMethod) {
  case NEIGHBOR:
    intx = (int)x;
    inty = (int)y;
    gray(inty, intx, value);
    break;
  case CUBIC:
  case LINEAR:
    intx = (int)x;
    inty = (int)y;
    double fracx = x - intx;
    double fracy = y - inty;
    try {
      gray(inty, intx, (int)((double)value * (1.0 - fracx) * (1.0 - fracy)));
      gray(inty, intx + 1, (int)((double)value * fracx * (1.0 - fracy)));
      gray(inty + 1, intx, (int)((double)value * (1.0 - fracx) * fracy));
      gray(inty + 1, intx + 1, (int)((double)value * fracx * fracy));
    }
    catch (PNM_ArrayException e) {
      gray(inty, intx, (int)((double)value * (1 - fracx) * (1 - fracy)));
      try {
        gray(inty, intx + 1, (int)((double)value * fracx * (1 - fracy)));
      }
      catch (PNM_ArrayException ee) {
      }
      try {
        gray(inty + 1, intx, (int)((double)value * (1 - fracx) * fracy));
      }
      catch (PNM_ArrayException ee) {
      }
    }
    break;
  } // switch
  return old;
}


PNM_Color PNM::color(double x, double y, PNM_Color value) {
  if (!valid(x, y)) {
    throw PNM_ArrayException(x, y);
  }
  PNM_Color old = color(x, y);
  int intx, inty;
  switch (_interpolationMethod) {
  case NEIGHBOR:
    intx = (int)x;
    inty = (int)y;
    color(inty, intx, value);
    break;
  case CUBIC:
  case LINEAR:
    intx = (int)x;
    inty = (int)y;
    double fracx = x - intx;
    double fracy = y - inty;
    try {
      color(inty, intx, value * ((1.0 - fracx) * (1.0 - fracy)));
      color(inty, intx + 1, value * (fracx * (1.0 - fracy)));
      color(inty + 1, intx, value * ((1.0 - fracx) * fracy));
      color(inty + 1, intx + 1, value * (fracx * fracy));
    }
    catch (PNM_ArrayException e) {
      color(inty, intx, value * ((1 - fracx) * (1 - fracy)));
      try {
        color(inty, intx + 1, value * (fracx * (1 - fracy)));
      }
      catch (PNM_ArrayException ee) {
      }
      try {
        color(inty + 1, intx, value * ((1 - fracx) * fracy));
      }
      catch (PNM_ArrayException ee) {
      }
    }
    break;
  } // switch
  return old;
}


// -------------------------------------
// cubic interpolation using the CatMull-Rom method
// -------------------------------------
double CatMull(double t, double y0, double y1, double y2, double y3) {
  return
    -0.5 * (
    t*t*t * (y0 - 3 * y1 + 3 * y2 - y3) -
    t*t  * (2 * y0 - 5 * y1 + 4 * y2 - y3) +
    t     * (y0 - y2) -
    (2 * y1)
    );

} // CatMull

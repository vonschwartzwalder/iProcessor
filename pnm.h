//---------------------------------------------------------------------
//
//---------------------------------------------------------------------

#ifndef PNM_H
#define PNM_H

#define MIN(a, b, c) (a < b ? a : b) < c ? (a < b ? a : b) : c
#define MAX(a, b, c) (a > b ? a : b) > c ? (a > b ? a : b) : c

// ------------------------
// PNM class declaration
// ------------------------

// includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
using namespace std;

#include "point.h"


// -------------------------------------
// PNM_Color
// -------------------------------------
// Rec. 709 Chromaticities (D65 white point)
//
//    R      G      B      white
// x  0.640  0.300  0.150  0.3127
// y  0.330  0.600  0.060  0.3290
// z  0.030  0.100  0.790  0.3582
//
// Rec. 709 Conversions
//
// RGB to XYZ
// [X]   [ 0.412453  0.357580  0.180423]   [R]
// [Y] = [ 0.212671  0.715160  0.072169] * [G]
// [Z]   [ 0.019334  0.119193  0.950227]   [B]
//
// XYZ to RGB
// [R]   [ 3.240479 -1.537150 -0.498535]   [X]
// [G] = [-0.969256  1.875992  0.041556] * [Y]
// [B]   [ 0.055648 -0.204043  1.057311]   [Z]
//
// HSV conversions from www.easyrgb.com
//
// RGB to HSV
//    var_R = ( R / 255 )                     //RGB from 0 to 255
//    var_G = ( G / 255 )
//    var_B = ( B / 255 )
//
//    var_Min = min( var_R, var_G, var_B )    //Min. value of RGB
//    var_Max = max( var_R, var_G, var_B )    //Max. value of RGB
//    del_Max = var_Max - var_Min             //Delta RGB value
//
//    V = var_Max
//
//    if ( del_Max == 0 )                     //This is a gray, no chroma...
//    {
//       H = 0                                //HSV results from 0 to 1
//       S = 0
//    }
//    else                                    //Chromatic data...
//    {
//       S = del_Max / var_Max
//
//       del_R = ( ( ( var_Max - var_R ) / 6 ) + ( del_Max / 2 ) ) / del_Max
//       del_G = ( ( ( var_Max - var_G ) / 6 ) + ( del_Max / 2 ) ) / del_Max
//       del_B = ( ( ( var_Max - var_B ) / 6 ) + ( del_Max / 2 ) ) / del_Max
//
//       if      ( var_R == var_Max ) H = del_B - del_G
//       else if ( var_G == var_Max ) H = ( 1 / 3 ) + del_R - del_B
//       else if ( var_B == var_Max ) H = ( 2 / 3 ) + del_G - del_R
//
//       if ( H < 0 ) H += 1
//       if ( H > 1 ) H -= 1
//    }
//
// HSV to RGB
//    if ( S == 0 )                       //HSV from 0 to 1
//    {
//       R = V * 255
//       G = V * 255
//       B = V * 255
//    }
//    else
//    {
//       var_h = H * 6
//       if ( var_h == 6 ) var_h = 0      //H must be < 1
//       var_i = int( var_h )             //Or ... var_i = floor( var_h )
//       var_1 = V * ( 1 - S )
//       var_2 = V * ( 1 - S * ( var_h - var_i ) )
//       var_3 = V * ( 1 - S * ( 1 - ( var_h - var_i ) ) )
//
//       if      ( var_i == 0 ) { var_r = V     ; var_g = var_3 ; var_b = var_1 }
//       else if ( var_i == 1 ) { var_r = var_2 ; var_g = V     ; var_b = var_1 }
//       else if ( var_i == 2 ) { var_r = var_1 ; var_g = V     ; var_b = var_3 }
//       else if ( var_i == 3 ) { var_r = var_1 ; var_g = var_2 ; var_b = V     }
//       else if ( var_i == 4 ) { var_r = var_3 ; var_g = var_1 ; var_b = V     }
//       else                   { var_r = V     ; var_g = var_1 ; var_b = var_2 }
//
//       R = var_r * 255                  //RGB results from 0 to 255
//       G = var_g * 255
//       B = var_b * 255
//    }
// -------------------------------------

class PNM_Color {
public:
  PNM_Color() { r = 0; g = 0; b = 0; }
  PNM_Color(int nr, int ng, int nb) { r = nr; g = ng; b = nb; }
  int red() { return r; }
  int green() { return g; }
  int blue() { return b; }
  int hue() { return r; }
  int sat() { return g; }
  int val() { return b; }
  int gray() { return (int)(r * 0.299 + g * 0.587 + b * 0.114); }
  bool bit() { return !((r == 0) && (g == 0) && (b == 0)); }
  void red(int nr) { r = nr; }
  void green(int ng) { g = ng; }
  void blue(int nb) { b = nb; }
  void hue(int nr) { r = nr; }
  void sat(int ng) { g = ng; }
  void val(int nb) { b = nb; }
  void gray(int v) { r = v; g = v; b = v; }
  void bit(bool v) { r = (v ? 255 : 0); g = (v ? 255 : 0); b = (v ? 255 : 0); }
  void set(int nr, int ng, int nb) { r = nr; g = ng; b = nb; }
  bool operator==(const PNM_Color &c) { return (r == c.r && g == c.g && b == c.b); }
  bool operator!=(const PNM_Color &c) { return !(r == c.r && g == c.g && b == c.b); }
  bool operator<(const PNM_Color &c) { return (gray() < (int)(c.r*0.299 + c.g*0.587 + c.b*0.114)); }
  bool operator>(const PNM_Color &c) { return (gray() > (int)(c.r*0.299 + c.g*0.587 + c.b*0.114)); }
  PNM_Color &operator+(double i) {
    PNM_Color *c = new PNM_Color();
    c->r = (int)(r + i);
    c->g = (int)(g + i);
    c->b = (int)(b + i);
    return *c;
  }
  PNM_Color &operator-(double i) {
    PNM_Color *c = new PNM_Color();
    c->r = (int)(r - i);
    c->g = (int)(g - i);
    c->b = (int)(b - i);
    return *c;
  }
  PNM_Color &operator*(double i) {
    PNM_Color *c = new PNM_Color();
    c->r = (int)(r * i);
    c->g = (int)(g * i);
    c->b = (int)(b * i);
    return *c;
  }
  PNM_Color &operator/(double i) {
    PNM_Color *c = new PNM_Color();
    c->r = (int)(r / i);
    c->g = (int)(g / i);
    c->b = (int)(b / i);
    return *c;
  }
private:
  int r;
  int g;
  int b;
};

// -------------------------------------
// PNM exceptions
// -------------------------------------
class PNM_Exception {
public:
  PNM_Exception() { error = ""; }
  PNM_Exception(string s) { error = s; }
  PNM_Exception(char *s) { error = s; }
  string what() {
    return error;
  }
private:
  string error;
};

class PNM_IOException : PNM_Exception {
public:
  PNM_IOException() { error = "I/O error"; }
  PNM_IOException(string s) { error = s; }
  PNM_IOException(char *s) { error = s; }
  string what() {
    return error;
  }
private:
  string error;
};

class PNM_ArrayException : PNM_Exception {
public:
  PNM_ArrayException() { error = "Array subscript out of bounds"; }
  PNM_ArrayException(string s) { error = s; }
  PNM_ArrayException(char *s) { error = s; }
  PNM_ArrayException(int x, int y) {
    ostringstream sout;
    sout << "Array subscript (" << x << "," << y << ") out of bounds";
    error = sout.str();
  }
  PNM_ArrayException(double x, double y) {
    ostringstream sout;
    sout << "Array subscript (" << x << "," << y << ") out of bounds";
    error = sout.str();
  }
  string what() {
    return error;
  }
private:
  string error;
};


// -------------------------------------
// PNM class
// -------------------------------------
class PNM {

public:

  // image types
  static const int typeNON;
  static const int typePBM;
  static const int typePGM;
  static const int typePPM;
  
  // image planes
  static const int planeR;
  static const int planeG;
  static const int planeB;
  static const int planeH;
  static const int planeS;
  static const int planeV;

  // interpolation methods
  static const int NEIGHBOR;
  static const int LINEAR;
  static const int CUBIC;

  // constructors
  PNM();
  PNM(const PNM &p);
  PNM(char *filename);
  PNM(string filename);
  PNM(int ncols, int nrows, int type);

  // destructor
  ~PNM();

  // I/O
  void read(char *filename);
  void read(string filename);
  void read(istream &instream);
  void write(char *filename);
  void write(string filename);
  void write(ostream &outstream);

  // query
  bool raw() { return _raw; }
  bool pbm() { return (_type == typePBM) ? true : false; }
  bool pgm() { return (_type == typePGM) ? true : false; }
  bool ppm() { return (_type == typePPM) ? true : false; }
  int type() { return _type; }
  int threshold() { return _threshold; }
  int max_value() { return _max_value; }
  int rows() { return _rows; }
  int cols() { return _cols; }
  int interpolationMethod() { return _interpolationMethod; }

  // set attributes
  bool raw(bool val) {
    bool old = _raw;
    _raw = val;
    return old;
  }

  int threshold(int val) {
    int old = _threshold;
    _threshold = val;
    return old;
  }

  int max_value(int val) {
    int old = _max_value;
    _max_value = val;
    return old;
  }

  int interpolationMethod(int val) {
    int old = _interpolationMethod;
    _interpolationMethod = val;
    return old;
  }

  // get a pixel value
  bool      bit(int col, int row);
  bool      bit(double col, double row);
  int       gray(int col, int row);
  int       gray(double col, double row);
  PNM_Color color(int col, int row);
  PNM_Color color(double col, double row);
  PNM_Color hsv(int col, int row);

  // set a pixel value
  bool      bit(int col, int row, bool val);
  bool      bit(double col, double row, bool val);
  int       gray(int col, int row, int val);
  int       gray(double col, double row, int val);
  PNM_Color color(int col, int row, PNM_Color val);
  PNM_Color color(double col, double row, PNM_Color val);

  // graphic operations
  void drawLine(Point p0, Point p1, PNM_Color color);
  void drawLine(int scol, int srow, int ecol, int erow, PNM_Color color);
  void drawLine(double scol, double srow, double ecol, double erow, PNM_Color color);
  void drawPolygon(list<Point> polygon, PNM_Color color);
  void drawPolygon(list<Point> polygon, PNM_Color lc, PNM_Color fc);

  // operators
  PNM &operator=(PNM p);
  PNM &operator=(PNM_Color c);
  PNM &operator=(int v);
  PNM &operator=(bool v);

  // other functions
  void newImage(int type);
  void deleteImage(int type);
  bool valid(const int col, const int row);
  bool valid(const double col, const double row);
  void convert(int type);
  void toHSV();
  void plane(int plane);
  void quantize(int plane, int levels);
  void clear(int value);

private:
  bool      _raw;
  int       _rows;
  int       _cols;
  int       _max_value;
  int       _type;
  int       _interpolationMethod;
  bool      **bit_image;
  int       **gray_image;
  PNM_Color **color_image;
  int       _bit;
  int       _gray;
  PNM_Color _color;
  int       _threshold;

};

#endif


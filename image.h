#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <string>
#include <list>
using namespace std;

#include "pnm.h"
#include "graph.h"
#include "shape.h"

#define MATH_AND 1
#define MATH_OR  2
#define MATH_XOR 3
#define MATH_ADD 4
#define MATH_SUB 5
#define MATH_MUL 6
#define MATH_DIV 7
#define MATH_MOD 8
#define MATH_DIF 9
#define MATH_MAX 10
#define MATH_MIN 11

#define FILTER_MEDIAN    1
#define FILTER_SMOOTH    2
#define FILTER_SOBEL     3
#define FILTER_LAPLACIAN 4

#define DERIVATIVE_IX   1
#define DERIVATIVE_IY   2
#define DERIVATIVE_IXY  3
#define DERIVATIVE_IXX  4
#define DERIVATIVE_IYY  5
#define DERIVATIVE_K    6
#define DERIVATIVE_GM   7
#define DERIVATIVE_GD   8

#define FFT_FORWARD     1
#define FFT_REVERSE     2

#define DCT_FORWARD     1
#define DCT_REVERSE     2

#define WAVELET_FORWARD 1
#define WAVELET_REVERSE 2

#define CCD_FORWARD     1
#define CCD_REVERSE     2

#define ZOOM_IN   1
#define ZOOM_OUT  2


class Image {

public:

  // constructors
  Image();
  Image(const string &filename);
  //  Image(const Image &i);

  // destructor
  ~Image();

  // I/O
  PNM *loadImage(string name);
  bool saveImage(string name);
  void writeSexpr(ostream &out);

  // morphology
  bool threshold(int t);
  int  threshold_is();
  int  threshold_ki();
  int  threshold_otsu();
  int *calc_histogram(int plane);
  void dilate();
  void erode();
  void invert();
  void open(int depth);
  void close(int depth);

  // filter
  void filter(const int op);
  void intensity();
  void convolve(double *k, int size);

  // math
  void math(const PNM &img, const int op);

  // derivative
  void derivative(const int op);
  bool uvdec(double lambda, double delta, int iterations);
  void flow(int iterations, double step);

  // transform
  //void fft();
  //void dct();
  //void wavelet();
  
  // internal CCD functions
  // TODO: better Image integration
  void findShapes();                      // find all shapes in image
  void remove_shape(int shape);           // remove a shape from list, dict and adjacency
  int  fill4(Shape &s);                   // fill a region to create a shape
  bool inBounds(int x, int y);            // legal coordinates?
  int  contour(Shape &s);                 // chain around boundary of a shape
  int  inner_contour4(Shape &s);          // chain around inside boundary of a shape
  void next_inner_point4(Point &c, int color); // next point in a 4 contour
  int  outer_contour4(Shape &s);          // chain around inside boundary of a shape
  void next_outer_point4(Point &c, int color); // next point in a 4 contour
  void outwardFlows();                    // calculate outward flow on all shapes
  void outwardFlow(Shape &s);             // calculate outward flow on one shape
  void smoothBoundaries(int width, double sigma); // smooth the boundary on all shapes
  void smoothBoundary(Shape &s, double kernel[], int width); // smooth the boundary on one shape
  void clearMask();                       // create a mask sized to image
  void coalesce(bool debug, int coalesceLevels); // merge shapes
  void drawShape(PNM *img, Shape &s, PNM_Color &c); // draw a shape on an image
  void fillShape(PNM *image, Shape &shape, PNM_Color &fill);
  void prune(bool debug, int pruneSize, bool leafOnly); // remove small shapes
  bool readDB(bool debug, string inputDBName);
  bool writeDB(bool debug, string outputDBName);
  bool reconstruct(bool debug, string outputImageName);
  void overlay();
  
  // I/O
  void read(istream &in);             // read image from a stream
  void write(ostream &out);           // save image to a stream
  void dumpDictionary(ostream &out);  // write all the vertices in the dictionary
  
  // data
  PNM image;                           // working image
  PNM mask;                            // mask
  Graph graph;                         // hierarchical information
  map<unsigned int, Shape> dictionary; // dictionary of shapes

  // miscellaneous
  bool   dumpROI();
  bool   edge8(int x, int y, int color);
  bool   edge4(int x, int y, int color);
  int    range(int x, int y, int size);
  
  // TODO: move these to PNM or delete them
  int    calc_min_gray();
  int    calc_max_gray();
  double calc_mean_gray();
  int    calc_median_gray();
  int    count_gray(int color);
  int    rgbToGray(int r, int g, int b);

protected:

public:

  // internal image
  PNM im;
  struct {
    int left;
    int right;
    int top;
    int bottom;
  } roi;

};

#endif // IMAGE_H

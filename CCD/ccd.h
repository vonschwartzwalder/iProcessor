// ---------------------------------------------------------------------
//
//  CCD declaration
//
//  Find all the shapes in an image and create a tree
//  the reflects the structure.
//
// ---------------------------------------------------------------------

#ifndef CCD_H
#define CCD_H

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <set>
using namespace std;


// templates 
//#include "array2d.hpp"
//#include "array3d.hpp"

#include "PNM/pnm.h"
#include "point.h"
#include "shape.h"
#include "graph.h"



// --------------------------------------------------------
// classes
// --------------------------------------------------------
class CCD {

public:
  // constructors, destructor
  CCD();
  CCD(const string &name);
  ~CCD();

  // variable access
  
  // shapes
  int  find_shapes();                      // find all shapes in image
  void draw_shapes();                      // draw shape polygons onto image
  void draw_shape(Shape &s, PNM_Color &c); // draw a shape onto cimage
  void reconstruct();                      // create a new image from the shapes

  // internal shape utilities
  void remove_shape(int shape);           // remove a shape from list, dict and adjacency
  int fill4(Shape &s);                    // fill a region to create a shape
  void fillShape(Shape shape, int color); // fill a shape for reconstruction
  bool in_bounds(int x, int y);           // legal coordinates?
  int contour(Shape &s);                  // chain around boundary of a shape
  int inner_contour4(Shape &s);           // chain around inside boundary of a shape
  void next_inner_point4(Point &c, int color);   // next point in a 4 contour
  int outer_contour4(Shape &s);           // chain around inside boundary of a shape
  void next_outer_point4(Point &c, int color);   // next point in a 4 contour

  void quantize(int levels);          // quantize the image

  // internal conversion
  void image2gimage();    // convert image to internal gray array
  void gimage2image();    // convert internal gray array to image 
  void image2cimage();    // convert image to internal color array
  void cimage2image();    // convert internal color array to image
  void clear_mask();      // create a mask sized to image
  
  // I/O
  void read(const string &name);   // read image from a file
  void write(const string &name);  // save image to a file
  void dump_mask(string name);     // write mask image
  
  // data
  PNM image;                      // working image
  PNM original_image;             // saved image
  unsigned char **gimage;         // local gray image array
  unsigned char ***cimage;        // local color image array (3 planes)
  int **mask;                     // local mask array
  Graph graph;                    // hierarchical information
};

#endif

// --------------------------------------------------------
//
//  CCD implementation
//
//  Find all the shapes (objects, or connected components) in an image and create a tree
//  the reflects the structure.
//
// --------------------------------------------------------

#include <iostream>
#include <stack>
#include <cmath>
using namespace std;

#include "ccd.h"


// default constructor
CCD::CCD() {
}

// string constructor
CCD::CCD(const string &name) {
  read(name);
  original_image = image;
}

// destructor
CCD::~CCD() {
}


// --------------------------------------------------------
// quantize the original image and put it in the working image
// --------------------------------------------------------
void CCD::quantize(int levels) {
  cerr << "quantize no longer available" << endl;
}

// --------------------------------------------------------
// find all the connected components in the image
// --------------------------------------------------------
int CCD::find_shapes() {

  // local variables
  int   pixel;
  Shape cur_shape;
  int   count = 1;
  int   cols = image.cols();
  int   rows = image.rows();
  set<int> a;

  // create generic arrays
  image2gimage();
  clear_mask();

  // add a dummy shape
  cur_shape.label = 0;
  cur_shape.pixel_count = 0;
  cur_shape.area = 0;
  cur_shape.color = 255;
  graph.add_vertex(cur_shape);
  
  // --------------------------------------------------------
  // process each pixel in image
  // --------------------------------------------------------
  for(int y = 0; y < rows; y++) {
    for(int x = 0; x < cols; x++) {

      // if not visited
      if(!mask[x + y * cols]) {

        // get this pixel
        pixel = gimage[x][y];

        // create a new shape
        cur_shape.location.x = x;
        cur_shape.location.y = y;
        cur_shape.label = count;
        cur_shape.color = pixel;

        // fill shape
        cur_shape.pixel_count = fill4(cur_shape);
        cur_shape.area = cur_shape.pixel_count;

        // add shape to dict
        graph.add_vertex(cur_shape);

        count++;
      }
    }
  }

  return(count-1);
  
} // find_shapes


// --------------------------------------------------------
// draw all the connected components in the image
// --------------------------------------------------------
void CCD::draw_shapes() {

  // copy image to generic array
  image2cimage();

  Graph::d_iterator iter;
  PNM_Color c(255, 255, 120);

  // for each shape
  for(iter = graph.d_begin(); iter != graph.d_end(); iter++) {
    if(iter->second.label > 0) {
      draw_shape(iter->second, c);
    }
  }
  
  // copy generic array to image
  cimage2image();

} // draw_shapes


// --------------------------------------------------------
// draws a shape on the color image array
// --------------------------------------------------------
void CCD::draw_shape(Shape &s, PNM_Color &c) {

  unsigned char r = c.red();
  unsigned char g = c.green();
  unsigned char b = c.blue();

  // generate contour
  s.perimeter = contour(s);
  
  // for each point
  list<Point>::iterator iter;
  for(iter = s.points.begin();
      iter != s.points.end();
      iter++) {
    cimage[iter->x][iter->y][0] = r;
    cimage[iter->x][iter->y][1] = g;
    cimage[iter->x][iter->y][2] = b;
  }
  
} // draw_shape


// --------------------------------------------------------
// dump_mask
//
//   writes the mask as an image
// --------------------------------------------------------
void CCD::dump_mask(string name) {
  PNM timage = image;
  int cols = image.cols();
  int rows = image.rows();

  for(int x = 0; x < cols; x++) {
    for(int y = 0; y < rows; y++) {
      timage.color(x, y, PNM_Color(mask[x][y], mask[x][y], mask[x][y]));
    }
  }
  timage.write(name);
  
} // dump_mask


// -----------------------------------------------
// fill a region
//
// use a 4 connected model
//
// calculates region moments
// 
// algorithm
//   get start pixel color
//   set mask for this pixel
//   for each neighbor
//     if this is a blob pixel (start color and mask not set)
//       if next to another blob
//         add edge
//       push location on stack
//   while stack not empty
//     pop location
//     set mask for this pixel
//       for each neighbor
//         if this is a blob pixel (start color and mask not set)
//           if next to another blob
//             add edge
//           push location on stack
//
// -----------------------------------------------
int CCD::fill4(Shape &s) {

  // local variables
  int x = s.location.x;
  int y = s.location.y;
  int pix;
  int lbl = s.label;
  Point loc;
  stack<Point> stk;
  int count = 0;

  // moment variables
  int    x_min = 9999999;
  int    y_min = 9999999;
  int    x_max = 0;
  int    y_max = 0;
  double m00 = 0;
  double m10 = 0;
  double m01 = 0;
  double m11 = 0;
  double m02 = 0;
  double m20 = 0;
  double u11, u02, u20, X, Y;

  // first pixel
  if(in_bounds(x, y)) {
    
    // get start color
    pix = gimage[x][y];

    // set mask start pixel
    mask[x][y] = lbl;
    count++;

    // moments
    m00++;
    m10 += x;
    m01 += y;
    m11 += x * y;
    m20 += x * x;
    m02 += y * y;
    
    // min and max
    x_max = (x > x_max) ? x : x_max;
    y_max = (y > y_max) ? y : y_max;
    x_min = (x < x_min) ? x : x_min;
    y_min = (y < y_min) ? y : y_min;

  }
  else {
    return count;
  }

  // check neighbors
  if(in_bounds(x-1, y)) {
    if(gimage[x-1][y] == pix && mask[x-1][y] != lbl) {
      stk.push(Point(x-1, y));
    }
    else if(mask[x-1][y] != 0) {
      graph.add_edge(lbl, mask[x-1][y]);
    }
  }
  else {
    graph.add_edge(lbl, 0);
  }
  if(in_bounds(x, y+1)) {
    if(gimage[x][y+1] == pix && mask[x][y+1] != lbl) {
      stk.push(Point(x, y+1));
    }
    else if(mask[x][y+1] != 0) {
      graph.add_edge(lbl, mask[x][y+1]);
    }
  }
  else {
    graph.add_edge(lbl, 0);
  }
  if(in_bounds(x+1, y)) {
    if(gimage[x+1][y] == pix && mask[x+1][y] != lbl) {
      stk.push(Point(x+1, y));
    }
    else if(mask[x+1][y] != 0) {
      graph.add_edge(lbl, mask[x+1][y]);
    }
  }
  else {
    graph.add_edge(lbl, 0);
  }
  if(in_bounds(x, y-1)) {
    if(gimage[x][y-1] == pix && mask[x][y-1] != lbl) {
      stk.push(Point(x, y-1));
    }
    else if(mask[x][y-1] != 0) {
      graph.add_edge(lbl, mask[x][y-1]);
    }
  }
  else {
    graph.add_edge(lbl, 0);
  }
  
  // while stk not empty
  while(! stk.empty()) {

    // fill pixel on stk
    loc = stk.top(); stk.pop();
    if(in_bounds(loc.x, loc.y)) {
      if(gimage[loc.x][loc.y] == pix && mask[loc.x][loc.y] != lbl) {
        mask[loc.x][loc.y] = lbl;
        count++;

        // moments
        m00++;
        m10 += loc.x;
        m01 += loc.y;
        m11 += loc.x * loc.y;
        m20 += loc.x * loc.x;
        m02 += loc.y * loc.y;
        
        // min and max
        x_max = (loc.x > x_max) ? loc.x : x_max;
        y_max = (loc.y > y_max) ? loc.y : y_max;
        x_min = (loc.x < x_min) ? loc.x : x_min;
        y_min = (loc.y < y_min) ? loc.y : y_min;

      }
    }

    // check neighbors
    if(in_bounds(loc.x-1, loc.y)) {
      if(gimage[loc.x-1][loc.y] == pix && mask[loc.x-1][loc.y] != lbl) {
        stk.push(Point(loc.x-1, loc.y));
      }
      else if(mask[loc.x-1][loc.y] != 0) {
        graph.add_edge(lbl, mask[loc.x-1][loc.y]);
      }
    }
    else {
      graph.add_edge(lbl, 0);
    }
    if(in_bounds(loc.x, loc.y+1)) {
      if(gimage[loc.x][loc.y+1] == pix && mask[loc.x][loc.y+1] != lbl) {
        stk.push(Point(loc.x, loc.y+1));
      }
      else if(mask[loc.x][loc.y+1] != 0) {
        graph.add_edge(lbl, mask[loc.x][loc.y+1]);
      }
    }
    else {
      graph.add_edge(lbl, 0);
    }
    if(in_bounds(loc.x+1, loc.y)) {
      if(gimage[loc.x+1][loc.y] == pix && mask[loc.x+1][loc.y] != lbl) {
        stk.push(Point(loc.x+1, loc.y));
      }
      else if(mask[loc.x+1][loc.y] != 0) {
        graph.add_edge(lbl, mask[loc.x+1][loc.y]);
      }
    }
    else {
      graph.add_edge(lbl, 0);
    }
    if(in_bounds(loc.x, loc.y-1)) {
      if(gimage[loc.x][loc.y-1] == pix && mask[loc.x][loc.y-1] != lbl) {
        stk.push(Point(loc.x, loc.y-1));
      }
      else if(mask[loc.x][loc.y-1] != 0) {
        graph.add_edge(lbl, mask[loc.x][loc.y-1]);
      }
    }
    else {
      graph.add_edge(lbl, 0);
    }
    
  }

  // central moments calculation
  u11 = m11 - (m10 * m01)/m00;
  u20 = m20 - (m10 * m10)/m00;
  u02 = m02 - (m01 * m01)/m00;

  // moment invariants
  X = u20 + u02;  // spread
  Y = sqrt((u20 - u02) * (u20 - u02) + (4.0 * (u11 * u11))); // slenderness

  // moments array
  s.moments[0] = m00;
  s.moments[1] = m10;
  s.moments[2] = m01;
  s.moments[3] = u11;
  s.moments[4] = u20;
  s.moments[5] = u02;
  s.moments[6] = X;
  s.moments[7] = Y;

  // centroid
  s.centroid.x = (int)(m10/m00);
  s.centroid.y = (int)(m01/m00);

  // bounding box
  s.upper_left.x = x_min;
  s.upper_left.y = y_min;
  s.lower_right.x = x_max;
  s.lower_right.y = y_max;

  return count;
}

// -----------------------------------------------
// fill a shape
//
// use a 4 connected model
//
// algorithm
//   fill first pixel with color
//   for each neighbor
//     if this is a shape pixel that hasn't been filled
//       push location on stack
//   while stack not empty
//     pop location
//     fill this pixel
//     for each neighbor
//       if this is a shape pixel that hasn't been filled
//         push location on stack
// -----------------------------------------------
void CCD::fillShape(Shape shape, int color) {

  // local variables
  Point loc;
  stack<Point> stack;
  int x, y;
  PNM_Color pixel;
  PNM_Color fill(color, color, color);

  // get starting location
  x = shape.location.x;
  y = shape.location.y;

  // fill start pixel
  if(in_bounds(x, y)) {
    pixel = image.color(x, y);
    if(mask[x][y] == shape.label && pixel != fill) {
      image.color(x, y, fill);
    }
  }
  else {
    return;
  }

  // look at neighbors
  if(in_bounds(x-1, y)) {
    pixel = image.color(x-1, y);
    if(mask[x-1][y] == shape.label && pixel != fill) {
      stack.push(Point(x-1, y));
    }
  }
  if(in_bounds(x, y+1)) {
    pixel = image.color(x, y+1);
    if(mask[x][y+1] == shape.label && pixel != fill) {
      stack.push(Point(x, y+1));
    }
  }
  if(in_bounds(x+1, y)) {
    pixel = image.color(x+1, y);
    if(mask[x+1][y] == shape.label && pixel != fill) {
      stack.push(Point(x+1, y));
    }
  }
  if(in_bounds(x, y-1)) {
    pixel = image.color(x, y-1);
    if(mask[x][y-1] == shape.label && pixel != fill) {
      stack.push(Point(x, y-1));
    }
  }

  // while stack not empty
  while(! stack.empty()) {

    loc = stack.top(); stack.pop();
    x = loc.x;
    y = loc.y;

    // fill pixel on stack
    image.color(x, y, fill);

    // look at neighbors
    if(in_bounds(x-1, y)) {
      pixel = image.color(x-1, y);
      if(mask[x-1][y] == shape.label && pixel != fill) {
        stack.push(Point(x-1, y));
      }
    }
    if(in_bounds(x, y+1)) {
      pixel = image.color(x, y+1);
      if(mask[x][y+1] == shape.label && pixel != fill) {
        stack.push(Point(x, y+1));
      }
    }
    if(in_bounds(x+1, y)) {
      pixel = image.color(x+1, y);
      if(mask[x+1][y] == shape.label && pixel != fill) {
        stack.push(Point(x+1, y));
      }
    }
    if(in_bounds(x, y-1)) {
      pixel = image.color(x, y-1);
      if(mask[x][y-1] == shape.label && pixel != fill) {
        stack.push(Point(x, y-1));
      }
    }

  }

}

// -----------------------------------------------
// contour
//   find the boundary of a shape
//   and build an adjacency list of object
// -----------------------------------------------
int CCD::contour(Shape &s) {
  return(inner_contour4(s));
}

// -----------------------------------------------
// inner_contour4
//   find interior boundary of a shape
//   and build an adjacency list of object
//
// algorithm
//   repeat
//     get next center
//     if adjacent pixels not seen before
//       add to adjacency list
//   until current position is the start pixel
// -----------------------------------------------
int CCD::inner_contour4(Shape &s) {

  Point start;
  Point cpoint;
  int cnt = 0;
  int color = s.color;
  int label;
  int x, y;
  bool done = false;

  start = s.location;
  cpoint = s.location;
  cpoint.d = 1;  // start looking right

  // start a new boundary
  s.points.clear();

  // add this point to the boundary
  s.points.push_back(cpoint);
  cnt++;

  do {

    // find next pixel
    next_inner_point4(cpoint, color);

    // save contour point in boundary
    s.points.push_back(cpoint);

    // check region for adjacent shapes
    x = cpoint.x;
    y = cpoint.y;
    if(in_bounds(x , y-1)) {
      label = mask[x][y-1];
      if(label != 0 && label != s.label) {
        graph.add_edge(label, s.label);
      }
    }
    else {
      graph.add_edge(0, s.label);
    }
    if(in_bounds(x+1, y)) {
      label = mask[x+1][y];
      if(label != 0 && label != s.label) {
        graph.add_edge(label, s.label);
      }
    }
    else {
      graph.add_edge(0, s.label);
    }
    if(in_bounds(x ,y+1)) {
      label = mask[x][y+1];
      if(label != 0 && label != s.label) {
        graph.add_edge(label, s.label);
      }
    }
    else {
      graph.add_edge(0, s.label);
    }
    if(in_bounds(x-1, y)) {
      label = mask[x-1][y];
      if(label != 0 && label != s.label) {
        graph.add_edge(label, s.label);
      }
    }
    else {
      graph.add_edge(0, s.label);
    }

    cnt++;

    // see if this is really the end
    if(cpoint == start) {
      done = true;
      if(cpoint.d == 3) {
        if(in_bounds(start.x, start.y + 1) &&
           gimage[start.x][start.y+1] == color) {
          cpoint.d = 3;
          next_inner_point4(cpoint, color);
          if(!(cpoint == start)) {
            done = false;
            s.points.push_back(cpoint);
          }
        }
      }
    }
    
  } while(!done);

  return cnt;
}


// -----------------------------------------------
// next_inner_point4
//   find next contour point
//
// use a 4 connected model to match region grower (fill4)
//
// directions:
//
//        0
//        |
//   3 -- o -- 1
//        |
//        2
//
// algorithm
// -----------------------------------------------
void CCD::next_inner_point4(Point &c, int color) {

  int dir;
  int nhood[4];   // neighborhood of pixels

  int x = c.x;
  int y = c.y;

  // initialize nhood
  for(int i = 0; i < 4; i++) {
    nhood[i] = -1;
  }

  // get neighbors
  if(in_bounds(x , y-1)) {      // 0
    nhood[0] = gimage[x][y-1];
  }
  if(in_bounds(x+1, y)) {       // 1
    nhood[1] = gimage[x+1][y];
  }
  if(in_bounds(x ,y+1)) {       // 2
    nhood[2] = gimage[x][y+1];
  }
  if(in_bounds(x-1, y)) {       // 3
    nhood[3] = gimage[x-1][y];
  }
  
  // set start direction
  dir = ((c.d - 1) < 0) ? 3 : c.d - 1;

  // look for new direction
  bool found = false;
  if(nhood[dir] == color) {
    found = true;
  }
  else {
    dir = (dir+1) % 4;
    if(nhood[dir] == color) {
      found = true;
    }
    else {
      dir = (dir+1) % 4;
      if(nhood[dir] == color) {
        found = true;
      }
      else {
        dir = (dir+1) % 4;
        if(nhood[dir] == color) {
          found = true;
        }
      }
    }
  }

  // isolated pixel?
  if(!found) {
    return;
  }

  // set new direction
  c.d = dir;

  // set new location
  switch(dir) {
    case 0:
      c.y = y - 1;
      break;
    case 1:
      c.x = x + 1;
      break;
    case 2:
      c.y = y + 1;
      break;
    case 3:
      c.x = x - 1;
      break;
  }

}


// -----------------------------------------------
// outer_contour4
//   find exterior boundary of a shape
//   and build an adjacency list of object
//
// algorithm
//   repeat
//     get next center
//     if adjacent pixels not seen before
//       add to adjacency list
//   until current position is the start pixel
// -----------------------------------------------
int CCD::outer_contour4(Shape &s) {

  Point start;
  Point cpoint;
  int cnt = 0;
  int color = s.color;
  int label;

  // start one pixel to the left
  cpoint = s.location;
  cpoint.x = cpoint.x - 1;
  cpoint.d = 1;
  start = cpoint;

  // start a new boundary
  s.points.clear();

  // add this point to the boundary
  s.points.push_back(cpoint);
  cnt++;

  do {

    // find next pixel
    next_outer_point4(cpoint, color);

    // save contour point in boundary
    s.points.push_back(cpoint);

    // add point to adjacency list
    label = mask[cpoint.x][cpoint.y];
    if(label != 0 && label != s.label) {
      graph.add_edge(label, s.label);
    }

    cnt++;

  } while(!(cpoint == start));

  return cnt;
}


// -----------------------------------------------
// next_outer_point4
//   find next contour point
//
// use a 4 connected model to match region grower (fill4)
//
// directions:
//
//        0
//        |
//   3 -- o -- 1
//        |
//        2
//
// algorithm
// -----------------------------------------------
void CCD::next_outer_point4(Point &c, int color) {

  int dir;
  int nhood[4];   // neighborhood of pixels

  int x = c.x;
  int y = c.y;

  // initialize nhood
  for(int i = 0; i < 4; i++) {
    nhood[i] = -1;
  }

  // get neighbors
  if(in_bounds(x , y-1)) {      // 0
    nhood[0] = gimage[x][y-1];
  }
  if(in_bounds(x+1, y)) {       // 1
    nhood[1] = gimage[x+1][y];
  }
  if(in_bounds(x ,y+1)) {       // 2
    nhood[2] = gimage[x][y+1];
  }
  if(in_bounds(x-1, y)) {       // 3
    nhood[3] = gimage[x-1][y];
  }

  // set start direction
  dir = ((c.d - 1) < 0) ? 3 : c.d - 1;

  // look for new direction
  bool found = false;
  if(nhood[dir] != color) {
    found = true;
  }
  else {
    dir = (dir+1) % 4;
    if(nhood[dir] != color) {
      found = true;
    }
    else {
      dir = (dir+1) % 4;
      if(nhood[dir] != color) {
        found = true;
      }
      else {
        dir = (dir+1) % 4;
        if(nhood[dir] != color) {
          found = true;
        }
      }
    }
  }

  // isolated pixel?
  if(!found) {
    return;
  }

  // set new direction
  c.d = dir;

  // set new location
  switch(dir) {
    case 0:
      c.y = y - 1;
      break;
    case 1:
      c.x = x + 1;
      break;
    case 2:
      c.y = y + 1;
      break;
    case 3:
      c.x = x - 1;
      break;
  }

}


// --------------------------------------------------------
// recreate the image from shapes
// --------------------------------------------------------
void CCD::reconstruct() {

  // copy image to generic array
  image2gimage();

  Graph::d_iterator iter;
  PNM_Color c(255, 255, 120);

  // for each shape
  for(iter = graph.d_begin(); iter != graph.d_end(); iter++) {
    if(iter->second.label != 0) {
      c = PNM_Color(iter->second.color, iter->second.color, iter->second.color);
      //fillShape(iter->second, c);
    }
  }

  // copy generic array to image
  gimage2image();
  
}

// --------------------------------------------------------
// read an image
// --------------------------------------------------------
void CCD::read(const string &name) {
  image.read(name);
}

// --------------------------------------------------------
// write the image
// --------------------------------------------------------
void CCD::write(const string &name) {
  image.write(name);
}

// --------------------------------------------------------
// in_bounds
// --------------------------------------------------------
bool CCD::in_bounds(int x, int y) {
  static int cols = image.cols();
  static int rows = image.rows();
  if((x >= 0 && x < cols) && (y >= 0 && y < rows)) {
    return true;
  }
  return false;
} // in_bounds()


// --------------------------------------------------------
// image2gimage
// --------------------------------------------------------
void CCD::image2gimage() {

  int cols = image.cols();
  int rows = image.rows();

  // allocate new array
  gimage = new unsigned char *[cols];
  for (int i = 0; i < cols; i++) {
    gimage[i] = new unsigned char[rows];
  }

  // copy data
  for(int x = 0; x < cols; x++) {
    for(int y = 0; y < rows; y++) {
      gimage[x][y] = image.gray(x, y);
    }
  }
  
}

// --------------------------------------------------------
// image2cimage
// --------------------------------------------------------
void CCD::image2cimage() {

  int cols = image.cols();
  int rows = image.rows();
  int planes = 3;

  cimage = new unsigned char **[cols];
  for (int i = 0; i < cols; i++) {
    cimage[i] = new unsigned char *[rows];
    for (int j = 0; j < rows; j++) {
      cimage[i][j] = new unsigned char[planes];
    }
  }
  
  // copy data
  PNM_Color color;
  for(int x = 0; x < cols; x++) {
    for(int y = 0; y < rows; y++) {
      color = image.color(x, y);
      cimage[x][y][0] = color.red();
      cimage[x][y][1] = color.green();
      cimage[x][y][2] = color.blue();
    }
  }

}

// --------------------------------------------------------
// clear_mask
// --------------------------------------------------------
void CCD::clear_mask() {

  int cols = image.cols();
  int rows = image.rows();

  // allocate new array
  mask = new int *[cols];
  for (int i = 0; i < cols; i++) {
    mask[i] = new int[rows];
  }
  
  // clear data
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      mask[i][j] = 0;
    }
  }
}

// --------------------------------------------------------
// gimage2image
// --------------------------------------------------------
void CCD::gimage2image() {
  int cols = image.cols();
  int rows = image.rows();

  for(int x = 0; x < cols; x++) {
    for(int y = 0; y < rows; y++) {
      image.color(x, y, PNM_Color(gimage[x][y], gimage[x][y], gimage[x][y]));
    }
  }
}

// --------------------------------------------------------
// cimage2image
// --------------------------------------------------------
void CCD::cimage2image() {
  int cols = image.cols();
  int rows = image.rows();

  image.convert(PNM::typePPM);

  for(int x = 0; x < cols; x++) {
    for(int y = 0; y < rows; y++) {
      image.color(x, y, PNM_Color(cimage[x][y][0], cimage[x][y][1], cimage[x][y][2]));
    }
  }
}


// --------------------------------------------------------
//
//  CCD implementation
//
//  Find all the shapes (objects, or connected components)
//  in an image and create a graph that reflects the structure.
//
// --------------------------------------------------------

#include <iostream>
#include <string>
#include <stack>
#include <list>
#include <map>
#include <set>
using namespace std;

#include "image.h"
#include "gaussian.h"

// --------------------------------------------------------
// find all the connected components in the image
// --------------------------------------------------------
int Image::findShapes() {

  // local variables
  image = PNM(im); // make a copy of the original
  int   pixel;
  Shape cur_shape;
  int   count = 1;
  int   cols = image.cols();
  int   rows = image.rows();

  // create and initialize mask
  clearMask();

  // add an outer shape
  cur_shape.label = 0;
  cur_shape.area = 0;
  cur_shape.color = 255;
  graph.addNode(0);
  dictionary[0] = cur_shape;

  // --------------------------------------------------------
  // process each pixel in im
  // --------------------------------------------------------
  for(int y = 0; y < rows; y++) {
    for(int x = 0; x < cols; x++) {

      // if not visited
      if(mask.gray(x, y) == 0) {
        
        // get this pixel
        pixel = image.gray(x, y);

        // set up a new Shape object
        cur_shape.location.x = x;
        cur_shape.location.y = y;
        cur_shape.label = count;
        cur_shape.color = pixel;

        // fill shape
        cur_shape.area = fill4(cur_shape);

        // get contour
        contour(cur_shape);
        
        // add shape to graph
        graph.addNode(cur_shape.label);

        // add shape to dictionary
        dictionary[cur_shape.label] = cur_shape;

        count++;
      }
    }
  }

  return(count-1);
  
} // findShapes

// --------------------------------------------------------
// dump all the nodes in the dictionary
// --------------------------------------------------------
void Image::dumpDictionary(ostream &out) {
  map<unsigned int, Shape>::iterator iter;
  for(iter = dictionary.begin(); iter != dictionary.end(); iter++) {
    out << iter->second;
  }
} // dumpDictionary

// --------------------------------------------------------
// calculate outward flow for all shapes
// --------------------------------------------------------
void Image::outwardFlows() {

  map<unsigned int, Shape>::iterator iter;

  // for each shape
  for(iter = dictionary.begin(); iter != dictionary.end(); iter++) {
    if(iter->second.label > 0) {
      outwardFlow(iter->second);
    }
  }

}

// --------------------------------------------------------
// calculate outward flow as a figure of merit
// --------------------------------------------------------
void Image::outwardFlow(Shape &shp) {

  int x, y, i, j;
  int grays[9];
  int count;
  double n, s, e, w;
  double Ix, Iy;
  double gm = 0.0;

  // for each point
  list<Point>::iterator iter;
  for(iter = shp.points.begin();
      iter != shp.points.end();
      iter++) {

    x = iter->x;
    y = iter->y;
    
    // 3x3 kernel
    grays[0] = image.gray(x, y);
    for(i = 1; i < 9; i++) {
      grays[i] = grays[0];
    }
    count = 0;
    for(j = y-1; j <= y+1; j++) {
      for(i = x-1; i <= x+1; i++) {
        try {
          grays[count] = image.gray(i, j);
        }
        catch(PNM_ArrayException e) {}
        count++;
      }
    }

    // neighborhood
    n  = (double)grays[1];
    e  = (double)grays[5];
    s  = (double)grays[7];
    w  = (double)grays[3];

    // derivatives using central differences
    Ix = (-w + e)/2;
    Iy = (-n + s)/2;
    
    // gradient magnitude
    gm += sqrt((Ix*Ix)+(Iy*Iy));

  }

  // normalize
  gm /= shp.points.size();

  // save
  shp.flow = gm;

}

// --------------------------------------------------------
// smooth the boundary of all shapes
// using convolution with a Gaussian kernel
// --------------------------------------------------------
void Image::smoothBoundaries(int width, double sigma) {

  map<unsigned int, Shape>::iterator iter;

  // initialize variables
  double * kernel = new double[width];
  Gaussian g;

  // get a kernel
  kernel = g.getKernel(width, sigma);

  // for each shape
  for(iter = dictionary.begin(); iter != dictionary.end(); iter++) {
    if(iter->second.label > 0) {

      // smooth the boundary
      smoothBoundary(iter->second, kernel, width);

      // remove duplicate points
    }
  }

}

// --------------------------------------------------------
// smooth the boundary of a shape
// using convolution with a Gaussian kernel
// --------------------------------------------------------
void Image::smoothBoundary(Shape &s, double kernel[], int width) {

  int i;
  double sx, sy;
  int x = -1, y = -1;
  int cnt;
  list<Point> npoints;
  Point *npoint;
  
  // for each point
  list<Point>::iterator fiter;
  list<Point>::iterator citer;
  list<Point>::iterator biter;
  for(citer = s.points.begin();
      citer != s.points.end();
      citer++) {
    fiter = citer;
    biter = citer;

    // back up biter to start of kernel
    for(i = 0; i < width/2; i++) {
      if(biter == s.points.begin()) {
        biter = s.points.end();
      }
      biter--;
    }

    // move fiter to end of kernel
    for(i = 0; i <= width/2; i++) {
      fiter++;
      if(fiter == s.points.end()) {
        fiter = s.points.begin();
      }
    }

    // move over kernel
    sx = 0;
    sy = 0;
    cnt = 0;
    while(biter != fiter) {
      sx += biter->x * kernel[cnt];
      sy += biter->y * kernel[cnt];
      cnt++;
      biter++;
      if(biter == s.points.end()) {
        biter = s.points.begin();
      }
    }

    // add point to list
    x = (int)rint(sx);
    y = (int)rint(sy);
    npoint = new Point(x, y, citer->d);
    npoints.push_back(*npoint);
      
  }

  // put new list in shape
  s.points.swap(npoints);
  
}

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
int Image::fill4(Shape &s) {

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
  if(inBounds(x, y)) {
    
    // get start color
    pix = image.gray(x, y);

    // set mask start pixel
    mask.gray(x, y, lbl);
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
  if(inBounds(x-1, y)) {
    if(image.gray(x-1, y) == pix && mask.gray(x-1, y) != lbl) {
      stk.push(Point(x-1, y));
    }
    else if(mask.gray(x-1, y) != 0) {
      graph.addEdge(lbl, mask.gray(x-1, y));
    }
  }
  else {
    graph.addEdge(lbl, 0);
  }
  if(inBounds(x, y+1)) {
    if(image.gray(x, y+1) == pix && mask.gray(x, y+1) != lbl) {
      stk.push(Point(x, y+1));
    }
    else if(mask.gray(x, y+1) != 0) {
      graph.addEdge(lbl, mask.gray(x, y+1));
    }
  }
  else {
    graph.addEdge(lbl, 0);
  }
  if(inBounds(x+1, y)) {
    if(image.gray(x+1, y) == pix && mask.gray(x+1, y) != lbl) {
      stk.push(Point(x+1, y));
    }
    else if(mask.gray(x+1, y) != 0) {
      graph.addEdge(lbl, mask.gray(x+1, y));
    }
  }
  else {
    graph.addEdge(lbl, 0);
  }
  if(inBounds(x, y-1)) {
    if(image.gray(x, y-1) == pix && mask.gray(x, y-1) != lbl) {
      stk.push(Point(x, y-1));
    }
    else if(mask.gray(x, y-1) != 0) {
      graph.addEdge(lbl, mask.gray(x, y-1));
    }
  }
  else {
    graph.addEdge(lbl, 0);
  }
  
  // while stk not empty
  while(! stk.empty()) {

    // fill pixel on stk
    loc = stk.top(); stk.pop();
    if(inBounds(loc.x, loc.y)) {
      if(image.gray(loc.x, loc.y) == pix && mask.gray(loc.x, loc.y) != lbl) {
        mask.gray(loc.x, loc.y, lbl);
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
    if(inBounds(loc.x-1, loc.y)) {
      if(image.gray(loc.x-1, loc.y) == pix && mask.gray(loc.x-1, loc.y) != lbl) {
        stk.push(Point(loc.x-1, loc.y));
      }
      else if(mask.gray(loc.x-1, loc.y) != 0) {
        graph.addEdge(lbl, mask.gray(loc.x-1, loc.y));
      }
    }
    else {
      graph.addEdge(lbl, 0);
    }
    if(inBounds(loc.x, loc.y+1)) {
      if(image.gray(loc.x, loc.y+1) == pix && mask.gray(loc.x, loc.y+1) != lbl) {
        stk.push(Point(loc.x, loc.y+1));
      }
      else if(mask.gray(loc.x, loc.y+1) != 0) {
        graph.addEdge(lbl, mask.gray(loc.x, loc.y+1));
      }
    }
    else {
      graph.addEdge(lbl, 0);
    }
    if(inBounds(loc.x+1, loc.y)) {
      if(image.gray(loc.x+1, loc.y) == pix && mask.gray(loc.x+1, loc.y) != lbl) {
        stk.push(Point(loc.x+1, loc.y));
      }
      else if(mask.gray(loc.x+1, loc.y) != 0) {
        graph.addEdge(lbl, mask.gray(loc.x+1, loc.y));
      }
    }
    else {
      graph.addEdge(lbl, 0);
    }
    if(inBounds(loc.x, loc.y-1)) {
      if(image.gray(loc.x, loc.y-1) == pix && mask.gray(loc.x, loc.y-1) != lbl) {
        stk.push(Point(loc.x, loc.y-1));
      }
      else if(mask.gray(loc.x, loc.y-1) != 0) {
        graph.addEdge(lbl, mask.gray(loc.x, loc.y-1));
      }
    }
    else {
      graph.addEdge(lbl, 0);
    }
    
  }

  // central moments calculation
  u11 = m11 - ((m10 * m01)/m00);
  u20 = m20 - ((m10 * m10)/m00);
  u02 = m02 - ((m01 * m01)/m00);

  // normalize central moments
  u11 = u11/(m00 * m00);
  u20 = u20/(m00 * m00);
  u02 = u02/(m00 * m00);

  // moment invariants
  X = u20 + u02;  // spread
  Y = sqrt(((u20 - u02) * (u20 - u02)) + (4.0 * (u11 * u11))); // slenderness

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
// contour
//   find the boundary of a shape
//   and build an adjacency list of object
// -----------------------------------------------
int Image::contour(Shape &s) {
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
int Image::inner_contour4(Shape &s) {

  Point start;
  Point cpoint;
  int cnt = 0;
  int color = s.color;
  unsigned int label;
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
    if(inBounds(x , y-1)) {
      label = mask.gray(x, y-1);
      if(label != 0 && label != s.label) {
        graph.addEdge(label, s.label);
      }
    }
    else {
      graph.addEdge(0, s.label);
    }
    if(inBounds(x+1, y)) {
      label = mask.gray(x+1, y);
      if(label != 0 && label != s.label) {
        graph.addEdge(label, s.label);
      }
    }
    else {
      graph.addEdge(0, s.label);
    }
    if(inBounds(x ,y+1)) {
      label = mask.gray(x, y+1);
      if(label != 0 && label != s.label) {
        graph.addEdge(label, s.label);
      }
    }
    else {
      graph.addEdge(0, s.label);
    }
    if(inBounds(x-1, y)) {
      label = mask.gray(x-1, y);
      if(label != 0 && label != s.label) {
        graph.addEdge(label, s.label);
      }
    }
    else {
      graph.addEdge(0, s.label);
    }

    cnt++;

    // see if this is really the end
    if(cpoint == start) {
      done = true;
      if(cpoint.d == 3) {
        if(inBounds(start.x, start.y + 1) &&
           image.gray(start.x, start.y+1) == color) {
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
void Image::next_inner_point4(Point &c, int color) {

  int dir;
  int nhood[4];   // neighborhood of pixels

  int x = c.x;
  int y = c.y;

  // initialize nhood
  for(int i = 0; i < 4; i++) {
    nhood[i] = -1;
  }

  // get neighbors
  if(inBounds(x , y-1)) {      // 0
    nhood[0] = image.gray(x, y-1);
  }
  if(inBounds(x+1, y)) {       // 1
    nhood[1] = image.gray(x+1, y);
  }
  if(inBounds(x ,y+1)) {       // 2
    nhood[2] = image.gray(x, y+1);
  }
  if(inBounds(x-1, y)) {       // 3
    nhood[3] = image.gray(x-1, y);
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
int Image::outer_contour4(Shape &s) {

  Point start;
  Point cpoint;
  int cnt = 0;
  int color = s.color;
  unsigned int label;

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
    label = mask.gray(cpoint.x, cpoint.y);
    if(label != 0 && label != s.label) {
      graph.addEdge(label, s.label);
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
void Image::next_outer_point4(Point &c, int color) {

  int dir;
  int nhood[4];   // neighborhood of pixels

  int x = c.x;
  int y = c.y;

  // initialize nhood
  for(int i = 0; i < 4; i++) {
    nhood[i] = -1;
  }

  // get neighbors
  if(inBounds(x , y-1)) {      // 0
    nhood[0] = image.gray(x, y-1);
  }
  if(inBounds(x+1, y)) {       // 1
    nhood[1] = image.gray(x+1, y);
  }
  if(inBounds(x ,y+1)) {       // 2
    nhood[2] = image.gray(x, y+1);
  }
  if(inBounds(x-1, y)) {       // 3
    nhood[3] = image.gray(x-1, y);
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
// read an image
// --------------------------------------------------------
void Image::read(istream &in) {
  image.read(in);
}

// --------------------------------------------------------
// write the im
// --------------------------------------------------------
void Image::write(ostream &out) {
  image.write(out);
}

// --------------------------------------------------------
// inBounds
// --------------------------------------------------------
bool Image::inBounds(int x, int y) {
  return image.valid(x, y);
} // inBounds()

// --------------------------------------------------------
// clearMask
// --------------------------------------------------------
void Image::clearMask() {
  mask = PNM(image.cols(), image.rows(), PNM::typePGM);
  mask = 0;
}

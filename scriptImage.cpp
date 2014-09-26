// ------------------------------------------------
// imageFunctions.cpp
// iProcessor
// ------------------------------------------------
#include <cstdlib>
using namespace std;

#include "script.h"
#include "image.h"

// ---------------------------------------------------------------------
// image functions
// ---------------------------------------------------------------------

void Script::imgRead() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != STRING) {
    s.pop();
    cerr << "invalid type, expected a string" << endl;
    return;
  }
  string filename = s.top().s; s.pop();
  Item item;
  item.type = IMAGE;
  image = new Image(filename);
  item.i = image;
  s.push(item);
}

void Script::imgWrite() {
  if (s.size() < 2) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != STRING) {
    s.pop();
    cerr << "invalid type, expected a string" << endl;
    return;
  }
  string filename = s.top().s; s.pop();
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->saveImage(filename);
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgDiscard() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  delete image;
}

void Script::imgShow() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  //image->show();
}

void Script::imgUpdate() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  //image->update();
}

void Script::imgHide() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  //image->hide();
}

void Script::imgWidth() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  Item a, b;
  a.type = IMAGE;
  a.i = image;
  s.push(a);
  b.type = NUMBER;
  b.n = (double)(image->im.cols());
  s.push(b);
}

void Script::imgHeight() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  Item a, b;
  a.type = IMAGE;
  a.i = image;
  s.push(a);
  b.type = NUMBER;
  b.n = (double)(image->im.rows());
  s.push(b);
}

void Script::imgInvert() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->invert();
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgIntensity() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->intensity();
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgThreshold() {
  if (s.size() < 2) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != NUMBER) {
    s.pop();
    cerr << "invalid type, expected a number" << endl;
    return;
  }
  double thresh = s.top().n; s.pop();
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->threshold((int)thresh);
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgAutoThresholdOtsu() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->threshold(image->threshold_otsu());
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgErode() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->erode();
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgDilate() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->dilate();
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgOpen() {
  if (s.size() < 2) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != NUMBER) {
    s.pop();
    cerr << "invalid type, expected a number" << endl;
    return;
  }
  double depth = s.top().n; s.pop();
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->open((int)depth);
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgClose() {
  if (s.size() < 2) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != NUMBER) {
    s.pop();
    cerr << "invalid type, expected a number" << endl;
    return;
  }
  double depth = s.top().n; s.pop();
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->close((int)depth);
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgLaplacian() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->filter(FILTER_LAPLACIAN);
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgMedian() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->filter(FILTER_MEDIAN);
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgSmooth() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->filter(FILTER_SMOOTH);
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgSobel() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->filter(FILTER_SOBEL);
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgConvolve() {
  if (s.size() < 2) {
    cerr << "imgConvolve: stack underflow" << endl;
    return;
  }

  // get matrix (as a list of lists)
  if (s.top().type != LIST) {
    s.pop();
    cerr << "invalid type, expected a list" << endl;
    return;
  }
  std::list<Item> l = s.top().l; s.pop();

  // get image
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();

  unsigned int rows = l.size();
  unsigned int cols;
  unsigned int x;
  unsigned int y;

  // allocate matrix
  double *matrix = new double[rows * rows];

  // iterate through the lists
  x = 0;
  y = 0;
  std::list<Item>::iterator iter1;
  std::list<Item>::iterator iter2;

  // rows
  for (iter1 = l.begin(); iter1 != l.end(); iter1++) {

    // make sure this is a list of lists
    if (iter1->type != LIST) {
      cerr << "not a list of lists" << endl;
      return;
    }

    // check that matrix is square
    cols = iter1->l.size();
    if (rows != cols) {
      cerr << "not a square matrix" << endl;
      return;
    }

    // columns
    for (iter2 = iter1->l.begin(); iter2 != iter1->l.end(); iter2++) {
      if (iter2->type != NUMBER) {
        cerr << "invalid type, expecting NUMBER" << endl;
        free(matrix);
        return;
      }
      matrix[x + (y * rows)] = iter2->n;
      x++;
    }

    y++;
    x = 0;
  }

  image->convolve(matrix, rows);
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgAND() {
  if (s.size() < 2) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  Image *image1 = s.top().i; s.pop();
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  Image *image2 = s.top().i; s.pop();
  image1->math(image2->im, MATH_AND);
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

void Script::imgFindShapes() {
  if (s.size() < 1) {
    cerr << "stack underflow" << endl;
    return;
  }
  if (s.top().type != IMAGE) {
    s.pop();
    cerr << "invalid type, expected an image" << endl;
    return;
  }
  image = s.top().i; s.pop();
  image->findShapes();
  Item item;
  item.type = IMAGE;
  item.i = image;
  s.push(item);
}

/*
void Script::blur() {
if(s.size() < 3) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double sigma = s.top().n; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double radius = s.top().n; s.pop();
image->blur(radius, sigma);
}

void Script::crop() {
if(s.size() < 5) {
cerr << "crop: stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "crop: invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "crop: invalid type" << endl;
return;
}
double y = s.top().n; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "crop: invalid type" << endl;
return;
}
double x = s.top().n; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "crop: invalid type" << endl;
return;
}
double h = s.top().n; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "crop: invalid type" << endl;
return;
}
double w = s.top().n; s.pop();
image->crop(Magick::Geometry(w,h,x,y));
}

void Script::despeckle() {
if(s.size() < 1) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
image->despeckle();
}

void Script::edge() {
if(s.size() < 2) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double radius = s.top().n; s.pop();
image->edge(radius);
}

void Script::equalize() {
if(s.size() < 1) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
image->equalize();
}

void Script::flip() {
if(s.size() < 1) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
image->flip();
}

void Script::flop() {
if(s.size() < 1) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
image->flop();
}

void Script::negate() {
if(s.size() < 1) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
image->negate();
}

void Script::normalize() {
if(s.size() < 1) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
image->normalize();
}

void Script::quantize() {
if(s.size() < 2) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double value = s.top().n; s.pop();
image->quantizeColors((unsigned int)value);
image->quantize();
}

void Script::imgroll() {
if(s.size() < 3) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double rows = s.top().n; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double cols = s.top().n; s.pop();
image->roll((int)cols, (int)rows);
}

void Script::rotate() {
if(s.size() < 2) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double degrees = s.top().n; s.pop();
image->rotate(degrees);
}

void Script::sample() {
if(s.size() < 3) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double h = s.top().n; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double w = s.top().n; s.pop();
image->sample(Magick::Geometry(w,h));
}

void Script::scale() {
if(s.size() < 3) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double h = s.top().n; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double w = s.top().n; s.pop();
image->scale(Magick::Geometry(w,h));
}

void Script::sharpen() {
if(s.size() < 3) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double sigma = s.top().n; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double radius = s.top().n; s.pop();
image->sharpen(radius, sigma);
}

void Script::shear() {
if(s.size() < 3) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double yangle = s.top().n; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double xangle = s.top().n; s.pop();
image->shear(xangle, yangle);
}

void Script::threshold() {
if(s.size() < 2) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double thresh = s.top().n; s.pop();
image->threshold(thresh);
}

void Script::zoom() {
if(s.size() < 3) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double h = s.top().n; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
double w = s.top().n; s.pop();
image->zoom(Magick::Geometry(w,h));
}

void Script::imgadd() {
if(s.size() < 2) {
return;
}

// get first image
if(s.top().type != IMAGE) {
s.pop();
return;
}
image = s.top().i; s.pop();

// check for second image or number
if(!(s.top().type == IMAGE || s.top().type == NUMBER)) {
s.pop();
return;
}

double v;
Image *i;
Image *i2;

// add a constant
if(s.top().type == NUMBER) {
v = s.top().n; s.pop();

// add constant to the image
//i2 = &(*image + v);
}

// add two images
else {
i = s.top().i; s.pop();

// add the images
i2 = &(*image + *i);
}

// push new image onto stack
Item item;
item.type = IMAGE;
item.i = i2;
s.push(item);
}

void Script::ROI() {
if(s.size() < 5) {
cerr << "stack underflow" << endl;
return;
}
if(s.top().type != IMAGE) {
s.pop();
cerr << "invalid type" << endl;
return;
}
image = s.top().i; s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
int y2 = (int)(s.top().n); s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
int x2 = (int)(s.top().n); s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
int y1 = (int)(s.top().n); s.pop();
if(s.top().type != NUMBER) {
s.pop();
cerr << "invalid type" << endl;
return;
}
int x1 = (int)(s.top().n); s.pop();
//image->setROI(x1,y1,x2,y2);
// make the compiler happy for now
x1 = x2; y1 = y2; x2 = y1; y2 = x1;
cerr << "imgROI not yet implemented" << endl;
}
*/

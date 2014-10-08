#include <algorithm>
#include <vector>
using namespace std;

//#include <alloca.h>

#include "image.h"

// -------------------------
// check this pixel for edge
// 8 connected
// -------------------------
bool Image::edge8(int x, int y, int color) {

  // check for neighbors
  for (int j = -1; j < 2; j++) {
    for (int i = -1; i < 2; i++) {
      if (im.valid(x + i, y + j) && (im.gray(x + 1, y + j) == color)) {
        return true;
      }
    }
  }

  return false;
}

// -------------------------
// check this pixel for edge
// 4 connected
// -------------------------
bool Image::edge4(int x, int y, int color) {

  // check for neighbors
  if (im.valid(x, y - 1) && (im.gray(x, y - 1) == color))
    return true;
  if (im.valid(x, y + 1) && (im.gray(x, y + 1) == color))
    return true;
  if (im.valid(x - 1, y) && (im.gray(x - 1, y) == color))
    return true;
  if (im.valid(x + 1, y) && (im.gray(x + 1, y) == color))
    return true;

  return false;
}

// -------------------------
// update the ROI
// -------------------------
//void Image::setROI(const QRect &r) {
//  roi = r;
//}

// -------------------------------------
// calculate histogram
//
// plane:
//   0 : gray values in a color image
//   1 : red
//   2 : green
//   3 : blue
//   ignored if this is a gray image
// -------------------------------------
int *Image::calc_histogram(int plane) {

  int *data = new int[256];

  int x, y;
  PNM_Color color;

  // initialize histogram
  for (int i = 0; i < 256; i++) {
    data[i] = 0;
  }

  switch (im.type()) {
  case 1:
    break;
  case 2:
    for (y = 0; y < im.rows(); y++) {
      for (x = 0; x < im.cols(); x++) {
        if (im.valid(x, y)) {
          data[im.gray(x, y)]++;
        }
      }
    }
    break;
  case 3:
    for (y = 0; y < im.rows(); y++) {
      for (x = 0; x < im.cols(); x++) {
        if (im.valid(x, y)) {
          color = im.color(x, y);
          switch (plane) {
          case 0:  // gray
            data[im.gray(x, y)]++;
            break;
          case 1:  // red
            data[color.red()]++;
            break;
          case 2:  // green
            data[color.green()]++;
            break;
          case 3:  // blue
            data[color.blue()]++;
            break;
          }
        }
      }
    }
    break;
  default:
    break;
  }

  return data;
}

// -------------------------------------
// calculate minimum gray value
// -------------------------------------
int Image::calc_min_gray() {
  int pixel;
  int min = 256;

  // for each pixel
  for (int y = 0; y < im.rows(); y++) {
    for (int x = 0; x < im.cols(); x++) {
      pixel = im.gray(x, y);
      min = (min < pixel) ? min : pixel;
    }
  }
  return min;
}


// -------------------------------------
// calculate maximum gray value
// -------------------------------------
int Image::calc_max_gray() {
  int pixel;
  int max = -1;

  // for each pixel
  for (int y = 0; y < im.rows(); y++) {
    for (int x = 0; x < im.cols(); x++) {
      pixel = im.gray(x, y);
      max = (max > pixel) ? max : pixel;
    }
  }
  return max;
}


// -------------------------------------
// calculate mean gray value
// -------------------------------------
double Image::calc_mean_gray() {
  int mean = 0;
  int count = 0;

  // for each pixel
  for (int y = 0; y < im.rows(); y++) {
    for (int x = 0; x < im.cols(); x++) {
      mean += im.gray(x, y);
      count++;
    }
  }
  return((double)mean / (double)count);
}


// -------------------------------------
// calculate median gray value
//
//   assumes that the center location is sufficient
// -------------------------------------
int Image::calc_median_gray() {
  int size = im.rows() * im.cols();
  vector<int> array;
  array.resize(size);

  // for each pixel
  int index = 0;
  for (int y = 0; y < im.rows(); y++) {
    for (int x = 0; x < im.cols(); x++) {
      array[index++] = im.gray(x, y);
    }
  }
  sort(array.begin(), array.end());
  return(array[(im.rows()*im.cols()) / 2]);
}


// -------------------------------------
// count values in image
// -------------------------------------
int Image::count_gray(int color) {
  int count = 0;

  // for each pixel
  for (int y = 0; y < im.rows(); y++) {
    for (int x = 0; x < im.cols(); x++) {
      count += im.gray(x, y) == color ? 1 : 0;
    }
  }

  return count;
}

// -------------------------------------
// convert RGB to gray
// -------------------------------------
int Image::rgbToGray(int red, int green, int blue) {
  return (int)((double)red * 0.2126 + (double)green * 0.7152 + (double)blue * 0.0722);
}


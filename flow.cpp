// ---------------------------------------------------------------------
// curvature flow
//
// this version is unconstrained
// future enhancement: add contraints:
//   anchors (if scaling? or provide a mask?)
//   signs - stop when neighbors are moving same direction
//   topology - don't cross isophotes
// ---------------------------------------------------------------------

#include <iomanip>
#include <cstring>
using namespace std;

#include <math.h>
#include <stdlib.h>

#include "image.h"


void Image::flow(int iterations = 1, double step = 0.2) {

  // local variables
  double n, s, e, w, c, nw, ne, sw, se;
  double Ix, Iy, Ixy, Ixx, Iyy, k, gm, Ix2, Iy2;
  double kernel[9];
  int count;
  int value;
  string m;
  int x, y, i, j, it;

  // get size of image
  int rows, cols;
  rows = im.rows();
  cols = im.cols();

  // -------------------------------------
  // copy image to array
  // -------------------------------------

  // allocate new arrays
  //  double gimage[cols][rows];
  //  double oimage[cols][rows];
  double **gimage, **oimage;
  gimage = (double **)malloc(cols * sizeof(double *));
  for (i = 0; i < cols; i++) {
    gimage[i] = (double *)malloc(rows * sizeof(double));
  }
  oimage = (double **)malloc(cols * sizeof(double *));
  for (i = 0; i < cols; i++) {
    oimage[i] = (double *)malloc(rows * sizeof(double));
  }

  // copy data
  for (i = 0, x = 0; x <= cols; x++, i++) {
    for (j = 0, y = 0; y <= rows; y++, j++) {
      oimage[i][j] = (double)(im.gray(x, y));
    }
  }

  // -------------------------------------
  // flow for each iteration
  // -------------------------------------
  for (it = 0; it < iterations; it++) {

    // swap input and output arrays
    double tmp[cols][rows];
    memcpy(tmp, gimage, sizeof(double) * cols * rows);
    memcpy(gimage, oimage, sizeof(double) * cols * rows);
    memcpy(oimage, tmp, sizeof(double) * cols * rows);

    // -------------------------------------
    // process each pixel in image
    // -------------------------------------
    for (y = 0; y < rows; y++) {
      for (x = 0; x < cols; x++) {

        // 3x3 kernel
        kernel[4] = gimage[x][y];
        for (i = 0; i < 9; i++) {
          kernel[i] = kernel[4];
        }
        count = 0;
        for (j = y - 1; j <= y + 1; j++) {
          for (i = x - 1; i <= x + 1; i++) {
            if ((i >= 0) && (i < cols) && (j >= 0) && (j < rows)) {
              kernel[count] = gimage[i][j];
            }
            count++;
          }
        }

        // get neighbors
        c = kernel[4];
        n = kernel[1];
        e = kernel[5];
        s = kernel[7];
        w = kernel[3];
        nw = kernel[0];
        ne = kernel[2];
        sw = kernel[6];
        se = kernel[8];

        // calculate derivatives using central differences
        Ix = (-w + e) / 2.0;
        Iy = (-n + s) / 2.0;
        Ixx = w + (-2.0 * c) + e;
        Iyy = n + (-2.0 * c) + s;
        Ixy = (nw + -ne + -sw + se) / 4.0;

        Ix2 = (Ix * Ix);
        Iy2 = (Iy * Iy);

        // -------------------------------------
        // future speed enhancement: combine k and gm calculation into one
        // -------------------------------------

        // calculate mean curvature
        k = ((1.0 + (Iy2))*Ixx - 2.0*Ix*Ixy*Iy + Iyy*(1.0 + (Ix2))) / pow(1.0 + (Ix2)+(Iy2), 1.5);

        // calculate gradient magnitude
        gm = sqrt(1.0 + (Ix2)+(Iy2));

        // calculate new pixel value
        oimage[x][y] = c + (k * gm * step);

      }
    }

  }

  // -------------------------------------
  // copy array to image
  // -------------------------------------
  for (i = 0, x = 0; x < cols; x++, i++) {
    for (j = 0, y = 0; y < rows; y++, j++) {
      value = (int)oimage[i][j];
      im.gray(x, y, value);
    }
  }

}

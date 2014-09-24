// ---------------------------------------------------------------------
// derivative based calculations
// ---------------------------------------------------------------------

#include <cmath>

#include "image.h"
#include "pnm.h"

#define GRAY(r, g, b) (double)r * 0.2126 + (double)g * 0.7152 + (double)b * 0.0722

// -------------------------------------
// calculate derivatives
// -------------------------------------

void Image::derivative(const int op) {

  PNM *temp = new PNM(im);
  int grays[9];
  PNM_Color color;
  int hue, sat, val, alpha;
  int x, y, i, j;
  int value;
  int count;
  float c, n, s, e, w, ne, nw, se, sw;
  float Ix, Iy, Ixy, Ixx, Iyy, k;

  for (y = 0; y <= temp->rows(); y++) {
    for (x = 0; x <= temp->cols(); x++) {

      // 3x3 kernel
      grays[4] = temp->gray(x, y);
      for (i = 1; i < 9; i++) {
        grays[i] = grays[4];
      }
      count = 0;
      for (j = y - 1; j <= y + 1; j++) {
        for (i = x - 1; i <= x + 1; i++) {
          if (temp->valid(i, j)) {
            grays[count] = temp->gray(i, j);
          }
          count++;
        }
      }

      c = (float)grays[4];
      n = (float)grays[1];
      e = (float)grays[5];
      s = (float)grays[7];
      w = (float)grays[3];
      nw = (float)grays[0];
      ne = (float)grays[2];
      sw = (float)grays[6];
      se = (float)grays[8];

      switch (op) {
      case DERIVATIVE_IX:
        Ix = (-w + e) / 2;
        value = (int)Ix;
        break;

      case DERIVATIVE_IY:
        Iy = (-n + s) / 2;
        value = (int)Iy;
        break;

      case DERIVATIVE_IXY:
        Ixy = (nw + -ne + -sw + se) / 4;
        value = (int)Ixy;
        break;

      case DERIVATIVE_IXX:
        Ixx = w + (-2 * c) + e;
        value = (int)Ixx;
        break;

      case DERIVATIVE_IYY:
        Iyy = n + (-2 * c) + s;
        value = (int)Iyy;
        break;

      case DERIVATIVE_K:
        Ix = (-w + e) / 2;
        Iy = (-n + s) / 2;
        Ixx = w + (-2 * c) + e;
        Iyy = n + (-2 * c) + s;
        Ixy = (nw + -ne + -sw + se) / 4;
        k = (((Iy * Iy)) * Ixx - 2 * Ix * Ixy * Iy + Iyy * ((Ix * Ix))) /
          (float)sqrt((1 + (Ix * Ix) + (Iy * Iy))*
          (1 + (Ix * Ix) + (Iy * Iy))*
          (1 + (Ix * Ix) + (Iy * Iy)));
        value = (int)k;
        break;

      case DERIVATIVE_GM:
        Ix = (-w + e) / 2;
        Iy = (-n + s) / 2;
        value = (int)sqrt(1 + (Ix * Ix) + (Iy * Iy));
        break;

      case DERIVATIVE_GD:
        Ix = (-w + e) / 2;
        Iy = (-n + s) / 2;
        value = (int)atan2((double)Iy, (double)Ix);
        break;
      }

      // update pixel
      value = abs(value);
      value = (value > 255) ? 255 : value;
      value = (value < 0) ? 0 : value;
      temp->gray(x, y, value);
    }

  }

  im = temp;

}



// -----------------------------------------------------------------
//
// filter.cpp
//
// image filters
// -----------------------------------------------------------------

#include "image.h"
#include <cmath>


// -----------------------------------------------------------------
// intensity
// -----------------------------------------------------------------

void Image::intensity() {

  int x, y;

  for (y = 0; y < im.rows(); y++) {
    for (x = 0; x < im.cols(); x++) {
      if (im.valid(x, y)) {
        im.gray(x, y, im.gray(x, y));
      }
    }
  }
}

// -----------------------------------------------------------------
// filter
// -----------------------------------------------------------------

void Image::filter(const int op) {

  int grays[9];
  PNM_Color color;
  PNM_Color colors[9];
  int x, y, i, j;
  int value, valueR, valueG, valueB;
  int sum, sumR, sumG, sumB;
  unsigned char min, minR, minG, minB;
  unsigned char slist[9], slistR[9], slistG[9], slistB[9];
  int count;
  
  switch (im.type()) {
    case 1:
      break;
    case 2:
      for (y = 0; y < im.rows(); y++) {
        for (x = 0; x < im.cols(); x++) {
          if (im.valid(x, y)) {

            // 3x3 kernel
            count = 0;
            for (j = y - 1; j <= y + 1; j++) {
              for (i = x - 1; i <= x + 1; i++) {
                if (im.valid(i, j)) {
                  grays[count++] = im.gray(i, j);
                }
              }
            }

            switch (op) {
              case FILTER_MEDIAN:
                // sort list
                for (int j = 0; j < (count / 2) + 1; j++) {
                  min = 255;
                  for (int i = j; i < count; i++) {
                    min = (min < grays[i]) ? min : grays[i];
                  }
                  slist[j] = min;
                }
                value = slist[count / 2];
                break;
              case FILTER_LAPLACIAN:
                value = grays[1] + grays[3] + grays[5] + grays[7] - (4 * grays[4]);
                break;
              case FILTER_SMOOTH:
                sum = 0;
                for (int i = 0; i < count; i++) {
                  sum += grays[i];
                }
                value = sum / count;
                break;
              case FILTER_SOBEL:
                int Sx, Sy;
                Sy = grays[6] + (2 * grays[7]) + grays[8] -
                  grays[0] - (2 * grays[1]) - grays[2];
                Sx = grays[2] + (2 * grays[5]) + grays[8] -
                  grays[0] - (2 * grays[3]) - grays[6];
                value = abs(Sx) + abs(Sy);
                break;
            }

            // update pixel
            value = (value > 255) ? 255 : value;
            value = (value < 0) ? 0 : value;
            im.gray(x, y, value);

          }
        }
      }
      break;
    case 3:
      for (y = 0; y < im.rows(); y++) {
        for (x = 0; x < im.cols(); x++) {
          if (im.valid(x, y)) {

            // 3x3 kernel
            count = 0;
            for (j = y - 1; j <= y + 1; j++) {
              for (i = x - 1; i <= x + 1; i++) {
                if (im.valid(i, j)) {
                  colors[count++] = im.color(i, j);
                }
              }
            }

            switch (op) {
              case FILTER_MEDIAN:
                // sort colors
                for (int j = 0; j < (count / 2) + 1; j++) {
                  minR = 255;
                  minG = 255;
                  minB = 255;
                  for (int i = j; i < count; i++) {
                    minR = (minR < colors[i].red()) ? minR : colors[i].red();
                    minG = (minG < colors[i].green()) ? minG : colors[i].green();
                    minB = (minB < colors[i].blue()) ? minB : colors[i].blue();
                  }
                  slistR[j] = minR;
                  slistG[j] = minG;
                  slistB[j] = minB;
                }
                valueR = slistR[count / 2];
                valueG = slistG[count / 2];
                valueB = slistB[count / 2];
                break;
              case FILTER_LAPLACIAN:
                valueR = colors[1].red() +
                  colors[3].red() +
                  colors[5].red() +
                  colors[7].red() -
                  (4 * colors[4].red());
                valueG = colors[1].green() +
                  colors[3].green() +
                  colors[5].green() +
                  colors[7].green() -
                  (4 * colors[4].green());
                valueB = colors[1].blue() +
                  colors[3].blue() +
                  colors[5].blue() +
                  colors[7].blue() -
                  (4 * colors[4].blue());
                break;
              case FILTER_SMOOTH:
                sumR = sumG = sumB = 0;
                for (int i = 0; i < count; i++) {
                  sumR += colors[i].red();
                  sumG += colors[i].green();
                  sumB += colors[i].blue();
                }
                valueR = sumR / count;
                valueG = sumG / count;
                valueB = sumB / count;
                break;
              case FILTER_SOBEL:
                int Syr, Syg, Syb, Sxr, Sxg, Sxb;
                Syr = colors[6].red() + (2 * colors[7].red()) + colors[8].red() -
                  colors[0].red() - (2 * colors[1].red()) - colors[2].red();
                Sxr = colors[2].red() + (2 * colors[5].red()) + colors[8].red() -
                  colors[0].red() - (2 * colors[3].red()) - colors[6].red();
                Syg = colors[6].green() + (2 * colors[7].green()) + colors[8].green() -
                  colors[0].green() - (2 * colors[1].green()) - colors[2].green();
                Sxg = colors[2].green() + (2 * colors[5].green()) + colors[8].green() -
                  colors[0].green() - (2 * colors[3].green()) - colors[6].green();
                Syb = colors[6].blue() + (2 * colors[7].blue()) + colors[8].blue() -
                  colors[0].blue() - (2 * colors[1].blue()) - colors[2].blue();
                Sxb = colors[2].blue() + (2 * colors[5].blue()) + colors[8].blue() -
                  colors[0].blue() - (2 * colors[3].blue()) - colors[6].blue();

                valueR = abs(Sxr) + abs(Syr);
                valueG = abs(Sxg) + abs(Syb);
                valueB = abs(Sxg) + abs(Syb);
                break;
            }

            // clamp pixel values
            valueR = (valueR > 255) ? 255 : valueR;
            valueR = (valueR < 0) ? 0 : valueR;
            valueG = (valueG > 255) ? 255 : valueG;
            valueG = (valueG < 0) ? 0 : valueG;
            valueB = (valueB > 255) ? 255 : valueB;
            valueB = (valueB < 0) ? 0 : valueB;

            // update pixel
            color.red(valueR);
            color.green(valueG);
            color.blue(valueB);
            im.color(x, y, color);
          }
        }
      }
      break;
    default:
      break;
  }

}

// -----------------------------------------------------------------
// convolve
// -----------------------------------------------------------------

void Image::convolve(double *k, int size) {

  int x, y, i, j;
  double *kernel;
  double sum;
  int lo, hi;

  im.convert(PNM::typePGM);

  // calculate pixel offsets
  lo = -(size / 2);
  hi = (size % 2) ? size / 2 : (size / 2) - 1;

  for (y = 0; y < im.rows(); y++) {
    for (x = 0; x < im.cols(); x++) {
      if (im.valid(x, y)) {
        sum = 0.0;
        kernel = k;
        for (j = lo; j <= hi; j++) {
          for (i = lo; i <= hi; i++) {
            if (im.valid(x + i, y + j)) {
              sum += (double) (im.gray(x + i, y + j)) * *kernel;
            }
            kernel++;
          }
        }
        sum = (sum < 0) ? -sum : sum;
        sum = (sum > 255) ? 255 : sum;
        sum = (sum < 0) ? 0 : sum;
        im.gray(x, y, (int) sum);
      }
    }
  }

}


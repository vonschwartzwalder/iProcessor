// -----------------------------------------------------------------
//
// morphology.cpp
//
// morphological operators
// -----------------------------------------------------------------

#include <cmath>
using namespace std;

#include "image.h"

// -------------------------------------
// threshold image
// -------------------------------------
bool Image::threshold(int t) {
  int x, y;
  int gray, red, green, blue;
  PNM_Color color;
  bool success = true;

  switch (im.type()) {
  case 1:
    break;
  case 2:
    for (y = 0; y < im.rows(); y++) {
      for (x = 0; x < im.cols(); x++) {
        if (im.valid(x, y)) {
          gray = im.gray(x, y);

          // dark objects, set below threshold
          if (t < 0) {
            gray = (gray < -t) ? 255 : 0;
          }

          // light objects, set above threshold
          else {
            gray = (gray > t) ? 255 : 0;
          }
          im.gray(x, y, gray);
        }
      }
    }
    break;
  case 3:
    for (y = 0; y < im.rows(); y++) {
      for (x = 0; x < im.cols(); x++) {
        if (im.valid(x, y)) {
          color = im.color(x, y);

          // dark objects, set below threshold
          if (t < 0) {
            red = (color.red() < -t) ? 255 : 0;
            green = (color.green() < -t) ? 255 : 0;
            blue = (color.blue() < -t) ? 255 : 0;
          }

          // light objects, set above threshold
          else {
            red = (color.red() > t) ? 255 : 0;
            green = (color.green() > t) ? 255 : 0;
            blue = (color.blue() > t) ? 255 : 0;
          }
          color.set(red, green, blue);
          im.color(x, y, color);
        }
      }
    }
    break;
  }

  return success;

}

// -------------------------------------
// auto thresholding using iterative selection
// -------------------------------------
int Image::threshold_is() {

  // compute histogram
  int *gray_hist_data;
  gray_hist_data = calc_histogram(0);

  // compute mean
  double mean_gray = 0.0;
  int count = (im.cols() * im.rows());
  for (int i = 0; i < 256; i++) {
    mean_gray += gray_hist_data[i] * i;
  }
  mean_gray = mean_gray / (double)count;

  // initialize threshold to first guess
  int thresh_1 = (int)mean_gray;
  int thresh_2;

  double num_1, num_2;
  double den_1, den_2;

  int pass = 1;

  // calculate new threshold until it doesn't change
  bool done = false;
  while (!done) {

    // numerator 1
    num_1 = 0.0;
    for (int i = 0; i < thresh_1; i++) {
      num_1 += i * gray_hist_data[i];
    }

    // denominator 1
    den_1 = 0.0;
    for (int i = 0; i < thresh_1; i++) {
      den_1 += gray_hist_data[i];
    }
    den_1 *= 2.0;

    // numerator 2
    num_2 = 0.0;
    for (int i = thresh_1; i < 256; i++) {
      num_2 += i * gray_hist_data[i];
    }

    // denominator 2
    den_2 = 0.0;
    for (int i = thresh_1; i < 256; i++) {
      den_2 += gray_hist_data[i];
    }
    den_2 *= 2.0;

    // new threshold
    thresh_2 = (int)((num_1 / den_1) + (num_2 / den_2));
    if (thresh_2 < 0) thresh_2 = 0;

    // check for done
    done = (thresh_1 == thresh_2) ? true : false;

    // set new value
    thresh_1 = thresh_2;

    pass++;

  }

  // return threshold
  return thresh_1;
}

// -------------------------------------
// auto thresholding using Kittler Illingworth criterion
// -------------------------------------
int Image::threshold_ki() {

  // compute histogram
  int *gray_hist_data;
  gray_hist_data = calc_histogram(0);

  int thresh;
  double P1, P2, mu1, mu2, var1, var2, J;
  double last_J = 1e10;

  // find minimum J and use that threshold
  for (int t = 0; t < 256; t++) {

    // P1
    P1 = 0.0;
    for (int i = 0; i <= t; i++) {
      P1 += gray_hist_data[i];
    }

    // P2
    P2 = 0.0;
    for (int i = t + 1; i < 256; i++) {
      P2 += gray_hist_data[i];
    }

    // mu1
    mu1 = 0.0;
    for (int i = 0; i <= t; i++) {
      mu1 += i * gray_hist_data[i];
    }
    mu1 = (P1 == 0.0) ? 0.0 : (mu1 / P1);

    // mu2
    mu2 = 0.0;
    for (int i = t + 1; i < 256; i++) {
      mu2 += i * gray_hist_data[i];
    }
    mu2 = (P2 == 0.0) ? 0.0 : (mu2 / P2);

    // var1
    var1 = 0.0;
    for (int i = 0; i <= t; i++) {
      var1 += gray_hist_data[i] * (i - mu1) * (i - mu1);
    }
    var1 = (P1 == 0.0) ? 0.0 : (var1 / P1);
    //    var1 = sqrt(var1);

    // var2
    var2 = 0.0;
    for (int i = t + 1; i < 256; i++) {
      var2 += gray_hist_data[i] * (i - mu2) * (i - mu2);
    }
    var2 = (P2 == 0.0) ? 0.0 : (var2 / P2);
    //    var2 = sqrt(var2);

    // J
    J = 1.0 + (2.0 * (P1 * log(var1) + P2 * log(var2))) -
      (2.0 * (P1 * log(P1) + P2 * log(P2)));

    // threshold
    thresh = (J < last_J) ? t : thresh;

    last_J = J;

  }

  // return threshold
  return thresh;
}

// -------------------------------------
// auto thresholding using Otsu method
// -------------------------------------
int Image::threshold_otsu() {
  int x;
  int y;
  int t;
  int gray;
  long NumPixels;
  long H[256];   //maximum gray values
  double q1, mu1, mu2, MU = 0.0, p;
  int Split;
  double sigmaB, Max, qt1, diff;
  int delta = 15;
  int MAXGRAY = 256;


  // -------------------------------------
  // build histogram
  // -------------------------------------
  int *gray_hist_data;
  gray_hist_data = calc_histogram(0);
  for (x = 0; x < MAXGRAY; x++) {
    H[x] = gray_hist_data[x];
    MU += x * gray_hist_data[x]; // mean gray value
  }
  NumPixels = (im.cols() * im.rows());
  MU /= NumPixels;

  // -------------------------------------
  //Find Otsu
  // -------------------------------------

  //Set initial conditions
  q1 = (double)H[0] / (double)NumPixels;
  mu1 = 0;
  mu2 = MU - mu1;    //mu2
  diff = (mu2 - mu1);
  Max = q1 * (1 - q1) * (diff * diff);
  Split = 0;

  for (t = 1; t < MAXGRAY; t++) {
    p = (double)H[t] / (double)NumPixels;
    qt1 = q1 + p;
    mu1 = (q1 * mu1 + t * p) / qt1;
    mu2 = (MU - qt1 * mu1) / (1 - qt1);
    diff = (mu2 - mu1);
    sigmaB = qt1 * (1 - qt1) * (diff * diff);
    q1 = qt1;
    if (sigmaB > Max) {
      Split = t;
      Max = sigmaB;
    }
  }

  if (((MAXGRAY - Split) > (MAXGRAY - delta)) || (Split < delta)) {
    int midcut = (int)(NumPixels / 2);
    int count = 0;
    //Do Median Cut
    for (x = 0; x < MAXGRAY; x++) {
      count += H[x];
      if (count <= midcut)
        Split = x;
      else
        break;
    }
  }

  return Split;
}

// -------------------------------------
// dilate
// -------------------------------------
void Image::dilate() {

  int x, y, i, j;
  int gray;
  int max, maxR, maxG, maxB;
  PNM_Color color;
  bool done = true;
  PNM temp = *new PNM(im);

  switch (im.type()) {
    case 1:
    for (y = 0; y < im.rows(); y++) {
      for (x = 0; x < im.cols(); x++) {
        gray = im.gray(x, y);
        if (gray != 0) {
          done = false;
          for (j = y - 1; j <= y + 1 && !done; j++) {
            for (i = x - 1; i <= x + 1; i++) {
              if (im.valid(i, j)) {
                gray = im.gray(i, j);
                if (gray == 0) {
                  temp.gray(x, y, 0);
                  done = true;
                  break;
                }
              }
            }
          }
        }
      }
    }
    break;
    case 2:
    for (y = 0; y < im.rows(); y++) {
      for (x = 0; x < im.cols(); x++) {
        max = 0;
        for (j = y - 1; j <= y + 1; j++) {
          for (i = x - 1; i <= x + 1; i++) {
            if (im.valid(i, j)) {
              gray = im.gray(i, j);
              max = (max > gray) ? max : gray;
            }
          }
        }
        temp.gray(x, y, max);
      }
    }
    break;
    case 3:
    for (y = 0; y < im.rows(); y++) {
      for (x = 0; x < im.cols(); x++) {
        maxR = 0;
        maxG = 0;
        maxB = 0;
        for (j = y - 1; j <= y + 1; j++) {
          for (i = x - 1; i <= x + 1; i++) {
            if (im.valid(i, j)) {
              color = im.color(i, j);
              maxR = (maxR > color.red()) ? maxR : color.red();
              maxG = (maxG > color.green()) ? maxG : color.green();
              maxB = (maxB > color.blue()) ? maxB : color.blue();
            }
          }
        }
        color.set(maxR, maxG, maxB);
        temp.color(x, y, color);
      }
    }
    break;
  }

  im = temp;
}


// -------------------------------------
// erode
// -------------------------------------
void Image::erode() {

  int x, y, i, j;
  int gray;
  int min, minR, minG, minB;
  PNM_Color color;
  bool done = true;
  PNM temp = *new PNM(im);

  switch (im.type()) {
  case 1:
    for (y = 0; y < im.rows(); y++) {
      for (x = 0; x < im.cols(); x++) {
        gray = im.gray(x, y);
        if (gray != 1) {
          done = false;
          for (j = y - 1; j <= y + 1 && !done; j++) {
            for (i = x - 1; i <= x + 1; i++) {
              if (im.valid(i, j)) {
                gray = im.gray(i, j);
                if (gray == 1) {
                  temp.gray(x, y, 1);
                  done = true;
                  break;
                }
              }
            }
          }
        }
      }
    }
    break;
  case 2:
    for (y = 0; y < im.rows(); y++) {
      for (x = 0; x < im.cols(); x++) {
        min = 255;
        for (j = y - 1; j <= y + 1; j++) {
          for (i = x - 1; i <= x + 1; i++) {
            if (im.valid(i, j)) {
              gray = im.gray(i, j);
              min = (min < gray) ? min : gray;
            }
          }
        }
        temp.gray(x, y, min);
      }
    }
    break;
  case 3:
    for (y = 0; y < im.rows(); y++) {
      for (x = 0; x < im.cols(); x++) {
        minR = 255;
        minG = 255;
        minB = 255;
        for (j = y - 1; j <= y + 1; j++) {
          for (i = x - 1; i <= x + 1; i++) {
            if (im.valid(i, j)) {
              color = im.color(i, j);
              minR = (minR < color.red()) ? minR : color.red();
              minG = (minG < color.green()) ? minG : color.green();
              minB = (minB < color.blue()) ? minB : color.blue();
            }
          }
        }
        color.set(minR, minG, minB);
        temp.color(x, y, color);
      }
    }
    break;
  }

  im = temp;
}

void Image::open(int depth = 1) {
  for (int i = 0; i < depth; i++) {
    erode();
  }
  for (int i = 0; i < depth; i++) {
    dilate();
  }
}

void Image::close(int depth = 1) {
  for (int i = 0; i < depth; i++) {
    dilate();
  }
  for (int i = 0; i < depth; i++) {
    erode();
  }
}

// -------------------------------------
// invert the pixmap
// -------------------------------------
void Image::invert() {

  int x, y;
  int gray;
  PNM_Color color;

  switch (im.type()) {
  case 1:
    im.convert(PNM::typePGM);
  case 2:
    for (y = 0; y < im.rows(); y++) {
      for (x = 0; x < im.cols(); x++) {
        if (im.valid(x, y)) {
          gray = im.gray(x, y);
          im.gray(x, y, (unsigned int)(255 - gray));
        }
      }
    }
    break;
  case 3:
    for (y = 0; y < im.rows(); y++) {
      for (x = 0; x < im.cols(); x++) {
        if (im.valid(x, y)) {
          color = im.color(x, y);
          color.set((int)(255 - color.red()),
            (int)(255 - color.green()),
            (int)(255 - color.blue()));
          im.color(x, y, color);
        }
      }
    }
    break;
  }
}


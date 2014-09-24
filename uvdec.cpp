#include <math.h>
#include "image.h"

// -------------------------------------
// uvdec
//
// u, v decomposition
// -------------------------------------
void uvdec_actual(double*, double*, int, double, double, int, int);

bool Image::uvdec(double lambda, double delta, int iterations) {
  int gray;
  PNM_Color color;
  bool success = true;

  // get rows and columns
  int rows = im.rows();
  int cols = im.cols();

  // create double arrays
  double *img;
  double *out;
  img = new double[rows * cols];
  out = new double[rows * cols];

  switch (im.type()) {
  case 1:
    break;
  case 2:
    // copy image to input array
    for (int j = 0, y = 0; y < im.rows(); y++, j++) {
      for (int i = 0, x = 0; x < im.cols(); x++, i++) {
        if (im.valid(x, y)) {
          gray = im.gray(x, y);
          img[j*cols + i] = (double)gray;
        }
      }
    }

    // run uvdec
    uvdec_actual(img, out, iterations, lambda, delta, cols, rows);

    // copy output array to image
    for (int j = 0, y = 0; y < im.rows(); y++, j++) {
      for (int i = 0, x = 0; x < im.cols(); x++, i++) {
        gray = (int)out[j*cols + i];
        im.gray(x, y, gray);
      }
    }
    break;
  case 3:
    // copy image to arrays
    for (int j = 0, y = 0; y < im.rows(); y++, j++) {
      for (int i = 0, x = 0; x < im.cols(); x++, i++) {
        if (im.valid(x, y)) {
          color = im.color(x, y);
          img[j*cols + i] = (double)rgbToGray(color.red(), color.green(), color.blue());
        }
      }
    }

    // run uvdec
    uvdec_actual(img, out, iterations, lambda, delta, cols, rows);

    // copy output array to image
    for (int j = 0, y = 0; y < im.rows(); y++, j++) {
      for (int i = 0, x = 0; x < im.cols(); x++, i++) {
        gray = (int)out[j*cols + i];
        color.set(gray, gray, gray);
        im.color(x, y, color);
      }
    }
    break;
  }

  return success;

}

void
uvdec_actual(double* im, double* Out, int niter, double lambda, double dt, int cols, int fils)
{
  double *vxatat, *vyatat, *vxadat, *vyadat, *vxatad, *vyatad, *vxadad, *vyadad, *tv;
  double divatatij, divatadij, divadatij, divadadij, lapx, lapy;
  double modgrad_u, uxijad, uyijad, uxijat, uyijat;
  int     i, j, for_i, back_i, for_j, back_j;

  vxatat = new double[fils*cols];
  vxatad = new double[fils*cols];
  vyatat = new double[fils*cols];
  vyatad = new double[fils*cols];
  vxadad = new double[fils*cols];
  vxadat = new double[fils*cols];
  vyadad = new double[fils*cols];
  vyadat = new double[fils*cols];
  tv = new double[fils*cols];

  for (i = 0; i < fils; i++)  {
    for (j = 0; j < cols; j++)  {
      Out[j*fils + i] = im[j*fils + i];
    }
  }

  for (int k = 1; k < niter; k++) {
    // Farem simetria sobre una linea imaginaria (estil Coloma)
    // Neumann Boundary Conditions sobre aquesta linia imaginaria
    for (i = 0; i < fils; i++)  {
      for (j = 0; j < cols; j++)  {
        for_i = i + 1;
        for_j = j + 1;
        back_i = i - 1;
        back_j = j - 1;

        if (for_i >= fils) for_i = i;
        if (for_j >= cols) for_j = j;
        if (back_i < 0) back_i = i;
        if (back_j < 0) back_j = j;
        uxijad = Out[j*fils + for_i] - Out[j*fils + i];
        uyijad = Out[for_j*fils + i] - Out[j*fils + i];
        uxijat = Out[j*fils + i] - Out[j*fils + back_i];
        uyijat = Out[j*fils + i] - Out[back_j*fils + i];

        // ++ (discretitzaci� del gradient)
        modgrad_u = sqrt(uxijad*uxijad + uyijad*uyijad);
        if (modgrad_u > 0.0)  {
          vxadad[j*fils + i] = (uxijad / modgrad_u);
          vyadad[j*fils + i] = (uyijad / modgrad_u);
        }
        else {
          vxadad[j*fils + i] = 0;
          vyadad[j*fils + i] = 0;
        }

        // +- (discretitzaci� del gradient)
        modgrad_u = sqrt(uxijad*uxijad + uyijat*uyijat);
        if (modgrad_u > 0.0)  {
          vxadat[j*fils + i] = (uxijad / modgrad_u);
          vyadat[j*fils + i] = (uyijat / modgrad_u);
        }
        else {
          vxadat[j*fils + i] = 0;
          vyadat[j*fils + i] = 0;
        }

        // -+ (discretitzaci� del gradient)
        modgrad_u = sqrt(uxijat*uxijat + uyijad*uyijad);
        if (modgrad_u > 0.0)  {
          vxatad[j*fils + i] = (uxijat / modgrad_u);
          vyatad[j*fils + i] = (uyijad / modgrad_u);
        }
        else {
          vxatad[j*fils + i] = 0;
          vyatad[j*fils + i] = 0;
        }

        // -- (discretitzaci� del gradient)
        modgrad_u = sqrt(uxijat*uxijat + uyijat*uyijat);
        if (modgrad_u > 0.0)  {
          vxatat[j*fils + i] = (uxijat / modgrad_u);
          vyatat[j*fils + i] = (uyijat / modgrad_u);
        }
        else {
          vxatat[j*fils + i] = 0;
          vyatat[j*fils + i] = 0;
        }
      }
    }

    // Same thing for the divergences
    for (i = 0; i < fils; i++)  {
      for (j = 0; j < cols; j++)  {
        for_i = i + 1;
        for_j = j + 1;
        back_i = i - 1;
        back_j = j - 1;

        if (for_i >= fils) for_i = i;
        if (for_j >= cols) for_j = j;
        if (back_i < 0) back_i = i;
        if (back_j < 0) back_j = j;

        // ++ (discretitzaci� del gradient)
        divatatij = vxadad[j*fils + i] - vxadad[j*fils + back_i] + vyadad[j*fils + i] - vyadad[back_j*fils + i];
        // +- (discretitzaci� del gradient)
        divatadij = vxadat[j*fils + i] - vxadat[j*fils + back_i] + vyadat[for_j*fils + i] - vyadat[j*fils + i];
        // -+ (discretitzaci� del gradient)
        divadatij = vxatad[j*fils + for_i] - vxatad[j*fils + i] + vyatad[j*fils + i] - vyatad[back_j*fils + i];
        // -- (discretitzaci� del gradient)
        divadadij = vxatat[j*fils + for_i] - vxatat[j*fils + i] + vyatat[for_j*fils + i] - vyatat[j*fils + i];

        tv[j*fils + i] = 0.25*(divatatij + divatadij + divadatij + divadadij);
      }
    }

    for (i = 0; i < fils; i++)  {
      for (j = 0; j < cols; j++)  {
        for_i = i + 1;
        for_j = j + 1;
        back_i = i - 1;
        back_j = j - 1;

        if (for_i >= fils) for_i = i;
        if (for_j >= cols) for_j = j;
        if (back_i < 0) back_i = i;
        if (back_j < 0) back_j = j;

        lapx = (tv[j*fils + for_i] - 2 * tv[j*fils + i] + tv[j*fils + back_i]);
        lapy = (tv[for_j*fils + i] - 2 * tv[j*fils + i] + tv[back_j*fils + i]);

        Out[j*fils + i] = Out[j*fils + i] + dt*(lambda*(im[j*fils + i] - Out[j*fils + i]) - lapx - lapy);
      }
    }
  }

  delete[] vxatat;
  delete[] vxadat;
  delete[] vyatat;
  delete[] vyadat;
  delete[] vxatad;
  delete[] vxadad;
  delete[] vyatad;
  delete[] vyadad;
  delete[] tv;

}


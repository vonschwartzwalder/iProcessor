#include <cmath>
using namespace std;

#include "image.h"


// ------------
//  image math
// ------------
void Image::math(const PNM &img, const int op) {

  int x, y, i, j;
  PNM_Color color1, color2;
  int red, red1, red2, green, green1, green2, blue, blue1, blue2;
  PNM tmp = *new PNM(img);

  // convert this image to color  (speed and space enhancements later)
  im.convert(PNM::typePPM);

  // convert the mask to color  (speed and space enhancements later)
  tmp.convert(PNM::typePPM);

  // visit each pixel
  for (y = 0, j = 0; y < im.rows(); y++, j++) {
    for (x = 0, i = 0; x < im.cols(); x++, i++) {
      if (im.valid(x, y)) {

        // get this pixel
        color1 = im.color(x, y);
        red1 = color1.red();
        green1 = color1.green();
        blue1 = color1.blue();
        if (tmp.valid(i, j)) {

          // get mask pixel
          color2 = tmp.color(i, j);
          red2 = color2.red();
          green2 = color2.green();
          blue2 = color2.blue();

          switch (op) {
          case MATH_AND:
            // AND pixels
            color1.set(red1 & red2, green1 & green2, blue1 & blue2);
            break;
          case MATH_OR:
            // OR pixels
            color1.set(red1 | red2, green1 | green2, blue1 | blue2);
            break;
          case MATH_XOR:
            // XOR pixels
            color1.set(red1 ^ red2, green1 ^ green2, blue1 ^ blue2);
            break;
          case MATH_ADD:
            // ADD pixels
            red = red1 + red2;
            green = green1 + green2;
            blue = blue1 + blue2;
            red = (red > 255) ? 255 : red;
            green = (green > 255) ? 255 : green;
            blue = (blue > 255) ? 255 : blue;
            color1.set(red, green, blue);
            break;
          case MATH_SUB:
            // SUBTRACT pixels
            red = red1 - red2;
            green = green1 - green2;
            blue = blue1 - blue2;
            red = (red < 0) ? 0 : red;
            green = (green < 0) ? 0 : green;
            blue = (blue < 0) ? 0 : blue;
            color1.set(red, green, blue);
            break;
          case MATH_MUL:
            // MULTIPLY pixels
            red = red1 * red2;
            green = green1 * green2;
            blue = blue1 * blue2;
            red = (red > 255) ? 255 : red;
            green = (green > 255) ? 255 : green;
            blue = (blue > 255) ? 255 : blue;
            color1.set(red, green, blue);
            break;
          case MATH_DIV:
            // DIVIDE pixels
            color1.set(red1 / red2, green1 / green2, blue1 / blue2);
            break;
          case MATH_MOD:
            // MODULO pixels
            color1.set(red1 % red2, green1 % green2, blue1 % blue2);
            break;
          case MATH_DIF:
            // DIFFERENCE pixels
            color1.set(abs(red1 - red2), abs(green1 - green2), abs(blue1 - blue2));
            break;
          case MATH_MAX:
            // MAXIMUM pixels
            color1.set((red1 > red2 ? red1 : red2),
              (green1 > green2 ? green1 : green2),
              (blue1 > blue2 ? blue1 : blue2));
            break;
          case MATH_MIN:
            // MINIMUM pixels
            color1.set((red1 < red2 ? red1 : red2),
              (green1 < green2 ? green1 : green2),
              (blue1 < blue2 ? blue1 : blue2));
            break;
          }

          // set new pixel
          im.color(x, y, color1);
        }
      }
    }
  }
}


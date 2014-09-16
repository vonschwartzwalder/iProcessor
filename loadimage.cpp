#include "image.h"

// ----------------------------------------
// load an image from a file, given a name
// ----------------------------------------
PNM *Image::loadImage(string name) {
  return new PNM(name);  
}
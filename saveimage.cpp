#include <string>
using namespace std;

#include "image.h"

// ----------------------------------------
// This function saves an image to a file
// Returns TRUE if the image was successfully saved
// ----------------------------------------
bool Image::saveImage(string name) {
  try {
    im.write(name);
    return true;
  }
  catch (PNM_IOException) {
    return false;
  }
}


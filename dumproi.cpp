#include "image.h"

#include <iomanip>

// dump a region of interest
bool Image::dumpROI() {

  int x,y;
  int pixel;

  cout << "dumping "
       << roi.left << "," << roi.top
       << " to "
       << roi.right << "," << roi.bottom
       << endl;
    
  // for each scanline
  for (y = roi.top; y < roi.bottom; y++) {
    // for each pixel
    for (x = roi.left; x < roi.right; x++) {
      if(im.valid(x, y)) {
        pixel = im.gray(x,y);
        cout << setw(3) << pixel << " ";
      }
    }
    cout << endl;
  }
  return true;
}
  

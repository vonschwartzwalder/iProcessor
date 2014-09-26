// -----------------------------------------------
// fill a shape
// -----------------------------------------------
#include "image.h"

void Image::fillShape(PNM *image, Shape &shape, PNM_Color &fill) {
  Point point;
  list<Point> points;
  list<Point>::iterator iter;
  for(iter = shape.points.begin(); iter != shape.points.end(); iter++) {
    point.x = iter->x;
    point.y = iter->y;
    points.push_back(point);
  }
  image->drawPolygon(points, fill);
}

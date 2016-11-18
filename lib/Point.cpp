#include "Point.h"

Point::Point() {
  x =0;
  y = 0;
}

Point::Point(double x, double y){
  this->x = x;
  this->y = y;
}
Point::Point(Point* point){
  this->x = point->x;
  this->y = point->y;
}

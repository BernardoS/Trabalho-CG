#ifndef Rectangle_H
#define Rectangle_H

#include "Point.h"
#include "Polygon.h"
#include <GL/gl.h>

class Rectangle : public Polygon {
public:
	double angle;
	double width;
  double height;
  Rectangle ();
	Rectangle (double side);
	Rectangle (double width, double height, GLfloat color[3]);
	void draw();
};

#endif // Rectangle_H

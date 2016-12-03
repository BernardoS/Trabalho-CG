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
	double z;
  Rectangle ();
	Rectangle (double side);
	Rectangle (double side, Polygon* polygon);
	Rectangle (double width, double height, Polygon* polygon);
	Rectangle (double width, double height, GLfloat color[3]);
	Rectangle (double width, double height, GLfloat color[3], GLuint tex);
	Rectangle (double width, double height, GLfloat color[3], GLuint tex, GLfloat* emission, GLfloat* ambient, GLfloat* difuse, GLfloat* specular, GLfloat* shininess);
	void draw();
	void depth(double z);
	double depth();
};

#endif // Rectangle_H

#ifndef Circle_H
#define Circle_H

#include "Point.h"
#include "Polygon.h"
#include <GL/gl.h>

class Circle : public Polygon{
public:
	double angle;
	double radius;
	Circle();
	Circle(double radius);
	Circle (double radius, GLfloat color[3]);
	Circle(Circle* circle);
	void draw();
	bool in(double x, double y, double r);
	bool in(Circle* circle);
	bool out(double x, double y, double r);
	bool out(Circle* circle);
};

#endif // Circle_H

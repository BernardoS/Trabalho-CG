#ifndef Circle_H
#define Circle_H

#include "Point.h"
#include "Polygon.h"
#include <GL/gl.h>

class Circle : public Polygon{
public:
	double angle;
	double radius;
	double z;
	Circle();
	Circle(double radius);
	Circle (double radius, GLfloat color[3]);
	Circle (double radius, GLfloat color[3], GLuint tex);
	Circle (double radius, GLfloat color[3], GLuint tex, GLfloat* emission, GLfloat* ambient, GLfloat* difuse, GLfloat* specular, GLfloat* shininess);
	Circle(Circle* circle);
	void draw();
	void drawWall(); // luiz
	bool in(double x, double y, double r);
	bool in(Circle* circle);
	bool out(double x, double y, double r);
	bool out(Circle* circle);
	void depth(double newDepth);
	double depth();
};

#endif // Circle_H

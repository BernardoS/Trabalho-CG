#ifndef Point_H
#define Point_H

#include <GL/glut.h>

class Point {
public:
	GLfloat x;
	GLfloat y;
	Point ();
	Point(double x, double y);
	Point(Point* p);
};

#endif // Point_H

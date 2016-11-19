#ifndef Polygon_H
#define Polygon_H

#include <GL/gl.h>


class Point;

class Polygon {
private:
	Point* pos;
	GLuint tex;
public:
	double color[3];
  Polygon ();
	Polygon(GLuint texture);
	Polygon (Point* pos);
	Polygon (Point* pos, double color[3]);
	Polygon(Polygon* polygon);
	void position(double x , double y);
	void position(Point* newPos);
	Point* position();
	void texture(GLuint tex);
	GLuint texture();
};

#endif // Polygon_H

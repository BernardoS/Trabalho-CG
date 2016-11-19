#ifndef Polygon_H
#define Polygon_H

#include <GL/gl.h>


class Point;

class Polygon {
private:
	Point* pos;
	GLuint tex;
	GLfloat materialEmission[];
	GLfloat materialAmbient[];
	GLfloat materialDifuse[];
	GLfloat materialSpecular[];
	GLfloat materialShininess[];
public:
	GLfloat color[3];
  Polygon ();
	Polygon(GLuint texture);
	Polygon (Point* pos);
	Polygon (Point* pos, GLfloat color[3]);
	Polygon(Polygon* polygon);
	void position(double x , double y);
	void position(Point* newPos);
	Point* position();
	// Material stuff
	void texture(GLuint tex);
	GLuint texture();
};

#endif // Polygon_H

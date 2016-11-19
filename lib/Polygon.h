#ifndef Polygon_H
#define Polygon_H

#include <GL/gl.h>


class Point;

class Polygon {
private:
	Point* pos;
	GLuint tex;
	//Material stuff
	GLfloat* materialEmission = NULL;
	GLfloat* materialAmbient = NULL;
	GLfloat* materialDifuse = NULL;
	GLfloat* materialSpecular = NULL;
	GLfloat* materialShininess = NULL;
public:
	GLfloat color[3];
  Polygon ();
	Polygon (Point* pos);
	Polygon(GLuint texture);
	Polygon (Point* pos, GLfloat color[3]);
	Polygon (Point* pos, GLfloat color[3], GLuint tex);
	Polygon (Point* pos, GLfloat color[3], GLuint tex, GLfloat* emission, GLfloat* ambient, GLfloat* difuse, GLfloat* specular, GLfloat* shininess);
	Polygon(Polygon* polygon);
	void position(double x , double y);
	void position(Point* newPos);
	Point* position();
	// Material stuff
	void texture(GLuint tex);
	GLuint texture();
	void emission(GLfloat* newParam);
	GLfloat* emission();
	void ambient(GLfloat* newParam);
	GLfloat* ambient();
	void difuse(GLfloat* newParam);
	GLfloat* difuse();
	void specular(GLfloat* newParam);
	GLfloat* specular();
	void shininess(GLfloat* newParam);
	GLfloat* shininess();
};

#endif // Polygon_H

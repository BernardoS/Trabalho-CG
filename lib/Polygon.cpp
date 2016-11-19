#include <cmath>
#include "Polygon.h"
#include "Point.h"
#include <iostream>
#include <GL/gl.h>


Polygon::Polygon(){
  for (size_t i = 0; i < 3; i++) color[i] = 0;
  pos = new Point();
}

Polygon::Polygon(GLuint texture){
  tex = texture;
}

Polygon::Polygon(Point* pos, GLfloat color[3]){
  for (size_t i = 0; i < 3; i++) this->color[i] = color[i];
  this->pos = pos;
}

Polygon::Polygon(Point* pos, GLfloat color[3], GLuint tex){
  for (size_t i = 0; i < 3; i++) this->color[i] = color[i];
  this->pos = pos;
  this->tex = tex;
}

Polygon::Polygon (Point* pos, GLfloat color[3], GLuint tex, GLfloat* emission, GLfloat* ambient, GLfloat* difuse, GLfloat* specular, GLfloat* shininess){
  for (size_t i = 0; i < 3; i++) this->color[i] = color[i];
  this->pos = pos;
  this->tex = tex;
  materialEmission = emission;
  materialAmbient = ambient;
  materialDifuse = difuse;
  materialSpecular = specular;
  materialShininess = shininess;
}

Polygon::Polygon(Point* pos){
  for (size_t i = 0; i < 3; i++) this->color[i] = 0;
  this->pos = pos;
}

Polygon::Polygon(Polygon* p) {
  for (size_t i = 0; i < 3; i++) this->color[i] = p->color[i];
  this->pos = p->pos;
}

void Polygon::position(double x, double y) {
  pos = new Point(x, y);
}
void Polygon::position(Point* newPos) {
  pos = newPos;
}
Point* Polygon::position(){
  return pos;
}

// Material Stuff
void Polygon::texture(GLuint texture) {
  tex = texture;
}

GLuint Polygon::texture(){
  return tex;
}

void Polygon::emission(GLfloat* newParam) {
  materialEmission = newParam;
}
GLfloat* Polygon::emission() {
  return materialEmission;
}
void Polygon::ambient(GLfloat* newParam){
  materialAmbient = newParam;
}
GLfloat* Polygon::ambient(){
  return materialAmbient;
}
void Polygon::difuse(GLfloat* newParam){
  materialDifuse = newParam;
}
GLfloat* Polygon::difuse(){
  return materialDifuse;
}
void Polygon::specular(GLfloat* newParam){
  materialSpecular = newParam;
}
GLfloat* Polygon::specular(){
  return materialSpecular;
}
void Polygon::shininess(GLfloat* newParam){
  materialShininess = newParam;
}
GLfloat* Polygon::shininess(){
  return materialShininess;
}

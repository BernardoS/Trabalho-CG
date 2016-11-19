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
  materialEmission = vector<GLfloat>(emission, emission + 4);
  materialAmbient = vector<GLfloat>(ambient, ambient + 4);
  materialDifuse = vector<GLfloat>(difuse, difuse + 4);
  materialSpecular = vector<GLfloat>(specular, specular + 4);
  materialShininess = vector<GLfloat>(specular, specular + 1);
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

void Polygon::emission(GLfloat* newParam){
  materialEmission.assign(newParam, newParam + 4);
}

GLfloat* Polygon::emission() {
  if (materialEmission.size() > 0)
    return &materialEmission[0];
  return NULL;
}

void Polygon::ambient(GLfloat* newParam){
  materialAmbient.assign(newParam, newParam + 4);
}
GLfloat* Polygon::ambient(){
  if (materialAmbient.size() > 0)
    return &materialAmbient[0];
  return NULL;
}
void Polygon::difuse(GLfloat* newParam){
  materialDifuse.assign(newParam, newParam + 4);
}
GLfloat* Polygon::difuse(){
  if (materialDifuse.size() > 0){
    // std::cout << materialDifuse[0] << std::endl;
    return &materialDifuse[0];
  }
  return NULL;
}
void Polygon::specular(GLfloat* newParam){
  materialSpecular.assign(newParam, newParam + 4);
}
GLfloat* Polygon::specular(){
  if (materialSpecular.size() > 0)
    return &materialSpecular[0];
  return NULL;
}
void Polygon::shininess(GLfloat* newParam){
  materialShininess.assign(newParam, newParam + 1);
}
GLfloat* Polygon::shininess(){
  if (materialShininess.size() > 0)
    return &materialShininess[0];
  return NULL;
}

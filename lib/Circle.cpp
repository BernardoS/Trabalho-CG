#include <cmath>
#include "Circle.h"
#include "Point.h"
#include <iostream>

void Circle::depth(double newDepth){
  z = newDepth;
}

double Circle::depth(){
  return z;
}

Circle::Circle () : Polygon(){
  this->radius = 1;
  angle = 0;
  sphere = false;
}

Circle::Circle (double radius, GLfloat color[3]) : Polygon(new Point(), color){
  this->radius = radius;
  angle = 0;
  sphere = false;
}

Circle::Circle (double radius, GLfloat color[3], GLuint tex) : Polygon(new Point(), color, tex){
  this->radius = radius;
  angle = 0;
  sphere = false;
}

Circle::Circle (double radius, GLfloat color[3], GLuint tex, GLfloat* emission, GLfloat* ambient, GLfloat* difuse, GLfloat* specular, GLfloat* shininess) : Polygon(new Point(), color, tex, emission, ambient, difuse, specular, shininess){
  this->radius = radius;
  angle = 0;
  sphere = false;
}
Circle::Circle (double radius) : Polygon(new Point()){
  this->radius = radius;
  angle = 0;
  sphere = false;
}
Circle::Circle (Circle* circle) : Polygon(circle->position(), circle->color, circle->texture(), circle->emission(), circle->ambient(), circle->difuse(), circle->specular(), circle->shininess()){
  radius = circle->radius;
  angle = 0;
  sphere = false;
}
void Circle::draw() {
  GLfloat* materialEmission = emission();
  GLfloat* materialAmbient = ambient();
  GLfloat* materialDifuse = difuse();
  GLfloat* materialSpecular = specular();
  GLfloat* materialShininess = shininess();
  glPushMatrix();
    if (materialEmission != NULL)glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    if (materialAmbient != NULL) glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    if (materialDifuse != NULL) glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuse);
    if (materialSpecular != NULL) glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    if (materialShininess != NULL) glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);
    glBindTexture (GL_TEXTURE_2D, texture());
    glTranslatef(position()->x,position()->y,0);
    glColor3f(color[0],color[1],color[2]);
    if (sphere) {
      gluSphere(gluNewQuadric(), radius, 100, 100);
    } else {
      const float n = 1000;

      // Tampa de baixo
      glBegin(GL_TRIANGLE_FAN);
        glNormal3f(0, 0, 0);
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);
        for(float i = 0; i <= n; i++){
          float angle = 2.0 * 3.1415926 * i / n;
          float x = radius * cos(angle);
          float y = radius * sin(angle);
          const float normX = -x;
          const float normY = -y;
          double len = sqrt(normX*normX + normY*normY);
          glNormal3f(normX/len, normY/len, 0);
          glTexCoord2f(x, y);
          glVertex3f(x, y,0);
        }
      glEnd();
      //
      // // Tampa de cima
      // glBegin(GL_TRIANGLE_FAN);
      //   glNormal3f(0, 0, 0);
      //   glTexCoord2f(0, 0);
      //   glVertex3f(0, 0, 0);
      //   for(float i = 0; i <= n; i++){
      //     float angle = 2.0 * 3.1415926 * i / n;
      //     float x = radius * cos(angle);
      //     float y = radius * sin(angle);
      //     double len = sqrt(x*x + y*y);
      //     glNormal3f(-x/len, -y/len, 0);
      //     // glTexCoord2f(x, y);
      //     glVertex3f(x, y,z);
      //   }
      // glEnd();
      //
      // // Parede
      // for(float i = 0; i < n/2; i++){
      //   float angle = 2.0 * 3.1415926 * i / (n/2);
      //   float nextAngle = 2.0 * 3.1415926 * (i+1) / (n/2);
      //   glBegin(GL_QUADS);
      //     float x = radius * cos(angle);
      //     float nextX = radius * cos(nextAngle);
      //     float y = radius * sin(angle);
      //     float nextY = radius * sin(nextAngle);
      //     // glNormal3f(0, 1, 0);
      //     // glTexCoord2f(x, y);
      //     glVertex3f(nextX, nextY, z);
      //     glVertex3f(x, y, z);
      //     glVertex3f(x, y, 0);
      //     glVertex3f(nextX, nextY, 0);
      //   glEnd();
      // }
    }
  glPopMatrix();
}

bool Circle::in(double x, double y, double r) {
  double d1 = std::abs(pow((position()->x - x),2));
  double d2 = std::abs(pow((position()->y - y),2));
  return radius >= sqrt(d1 + d2) + r;
}
bool Circle::in(Circle* circle) {
  double d1 = std::abs(pow((position()->x - circle->position()->x),2));
  double d2 = std::abs(pow((position()->y - circle->position()->y),2));
  return radius >= sqrt(d1 + d2) + circle->radius;
}
bool Circle::out(double x, double y, double r) {
  double d1 = std::abs(pow((position()->x - x),2));
  double d2 = std::abs(pow((position()->y - y),2));
  return radius <= sqrt(d1 + d2) - r;
}
bool Circle::out(Circle* circle) {
  double d1 = std::abs(pow((position()->x - circle->position()->x),2));
  double d2 = std::abs(pow((position()->y - circle->position()->y),2));
  return radius <= sqrt(d1 + d2) - circle->radius;
}

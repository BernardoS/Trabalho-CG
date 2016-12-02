#include <GL/glut.h>
#include "Rectangle.h"
#include "Point.h"
#include "Polygon.h"
#include <iostream>

Rectangle::Rectangle (double width, double height, GLfloat color[3]) : Polygon(new Point(), color){
  this->width = width;
  this->height = height;
}

Rectangle::Rectangle (double side, Polygon* polygon) : Polygon(polygon->position(), polygon->color, polygon->texture(), polygon->emission(), polygon->ambient(), polygon->difuse(), polygon->specular(), polygon->shininess()) {
  width = side;
  height = side;
}

Rectangle::Rectangle (double width, double height, Polygon* polygon) : Polygon(polygon->position(), polygon->color, polygon->texture(), polygon->emission(), polygon->ambient(), polygon->difuse(), polygon->specular(), polygon->shininess()){
  this->width = width;
  this->height = height;
}

Rectangle::Rectangle (double width, double height, GLfloat color[3], GLuint tex) : Polygon(new Point(), color, tex){
  this->width = width;
  this->height = height;
}

Rectangle::Rectangle (double width, double height, GLfloat color[3], GLuint tex, GLfloat* emission, GLfloat* ambient, GLfloat* difuse, GLfloat* specular, GLfloat* shininess) : Polygon(new Point(), color, tex, emission, ambient, difuse, specular, shininess){
  this->width = width;
  this->height = height;
}

Rectangle::Rectangle (double side) : Polygon() {
  width = side;
  height = side;
}
Rectangle::Rectangle () : Polygon(){
  width = 1;
  height = 1;
}

void Rectangle::draw() {
  GLfloat* materialEmission = emission();
  GLfloat* materialAmbient = ambient();
  GLfloat* materialDifuse = difuse();
  GLfloat* materialSpecular = specular();
  GLfloat* materialShininess = shininess();

  glPushMatrix();

    if (materialEmission != NULL) glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    if (materialAmbient != NULL) glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
    if (materialDifuse != NULL) glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuse);
    if (materialSpecular != NULL) glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
    if (materialShininess != NULL) glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

    glClearColor (0, 0, 0, 0);
    glBindTexture (GL_TEXTURE_2D, texture());
    glTranslatef(position()->x, position()->y, 0);
    glRotatef(angle, 0, 0, 1);
    glColor3f(color[0], color[1], color[2]);

    glBegin(GL_QUADS); // parte de cima
      glNormal3f(0, 1, 0);

      glTexCoord3f(0, 0, 0);
      glVertex3f(-width/2.0, -height/2.0, height);

      glTexCoord3f(1, 0, 0);
      glVertex3f(width/2.0, -height/2.0, height);

      glTexCoord3f(1, 1, 0);
      glVertex3f(width/2.0, height/2.0, height);

      glTexCoord3f(0, 1, 0);
      glVertex3f(-width/2.0, height/2.0, height);
    glEnd();

    glBegin(GL_QUADS); // parte de baixo
      glNormal3f(0, -1, 0);

      glTexCoord3f(0, 0, 0);
      glVertex3f(-width/2.0, -height/2.0, height);

      glTexCoord3f(1, 0, 0);
      glVertex3f(width/2.0, -height/2.0, height);

      glTexCoord3f(1, 1, 0);
      glVertex3f(width/2.0, height/2.0, height);

      glTexCoord3f(0, 1, 0);
      glVertex3f(-width/2.0, height/2.0, height);
    glEnd();

  glPopMatrix();
}

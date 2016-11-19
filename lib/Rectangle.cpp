#include <GL/glut.h>
#include "Rectangle.h"
#include "Point.h"
#include <iostream>

Rectangle::Rectangle (double width, double height, GLfloat color[3]) : Polygon(new Point(), color){
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
  glPushMatrix();
    glTranslatef(position()->x,position()->y,0);
    glRotatef(angle, 0, 0, 1);
    glColor3f(color[0],color[1],color[2]);
    glBegin(GL_QUADS);
      glVertex3f(-width/2.0, -height/2.0, 0);
      glVertex3f(-width/2.0, height/2.0, 0);
      glVertex3f(width/2.0, height/2.0, 0);
      glVertex3f(width/2.0, -height/2.0, 0);
    glEnd();
  glPopMatrix();
}

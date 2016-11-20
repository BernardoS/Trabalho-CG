#include <cmath>
#include "Car.h"
#include "Point.h"
#include <iostream>
#include <vector>
#include <GL/gl.h>

void Car::makeBody(GLfloat color[3]) {
  angle = 0;
  double height = (radius*sqrt(2));
  body = new Rectangle(height/3, height, color, Circle::texture(), emission(), ambient(), difuse(), specular(), shininess());
}
void Car::makeCannon() {
  GLfloat color[3] = {27/256.0,94/256.0,32/256.0};
  GLfloat emission[] = {0.10, 0.10, 0.10, 1};
	GLfloat ambient[] = {color[0], color[1], color[2], 0.2};
	GLfloat difuse[] = {color[0], color[1], color[2], 0.2};
	GLfloat specular[] = {0, 0, 0, 1};
	GLfloat shininess[] = {0.0};
  double width = body->width/2.0;
  cannon = new Rectangle(width/2.0, width*2, color, 0, emission, ambient, difuse, specular, shininess);
}

void Car::makeWheels() {
  //Wheels
  GLfloat colorWheel[3] = {66/256.0,66/256.0,66/256.0};
  GLfloat emissionWheel[] = {0.10, 0.10, 0.10, 1};
	GLfloat ambientWheel[] = {colorWheel[0], colorWheel[1], colorWheel[2], 0.2};
	GLfloat difuseWheel[] = {colorWheel[0], colorWheel[1], colorWheel[2], 0.2};
	GLfloat specularWheel[] = {0, 0, 0, 1};
	GLfloat shininessWheel[] = {0.0};

  for (size_t i = 0; i < 4; i++) {
    wheels[i] = *(new Rectangle(body->width/2.0, body->height/3, colorWheel, 0, emissionWheel, ambientWheel, difuseWheel, specularWheel, shininessWheel));
    //Cracks
    GLfloat colorCrack[3] = {158/256.0, 158/256.0, 158/256.0};
    for (size_t j = 0; j < 4; j++) {
      cracks[i].push_back(new Rectangle(wheels[i].width, wheels[i].height/10.0, colorCrack));
    }
  }
}
void Car::makeAxis() {
  GLfloat color[3] = {33/256.0,33/256.0,33/256.0};
  GLfloat emission[] = {0.10, 0.10, 0.10, 1};
	GLfloat ambient[] = {color[0], color[1], color[2], 0.2};
	GLfloat difuse[] = {color[0], color[1], color[2], 0.2};
	GLfloat specular[] = {0, 0, 0, 1};
	GLfloat shininess[] = {0.0};
  double width = body->width/2.0;
  for (size_t i = 0; i < 4; i++) {
    axis[i] = *(new Rectangle(width, width/2.0,color, 0, emission, ambient, difuse, specular, shininess));
  }
}
Car::Car () : Circle(){
  GLfloat color[3] = {139/256.0,195/256.0,74/256.0};
  makeBody(color);
  makeAxis();
  makeCannon();
  makeWheels();
}
Car::Car (double radius) : Circle(radius){
  GLfloat color[3] = {139/256.0,195/256.0,74/256.0};
  makeBody(color);
  makeAxis();
  makeCannon();
  makeWheels();
}
Car::Car (Circle* area) : Circle(area){
  makeBody(area->color);
  makeAxis();
  makeCannon();
  makeWheels();
}
void Car::moveWheels(double speed) {
  speed = wheels[0].angle + speed;
  if (speed > 0) {
    speed = (speed <= 45) ?  speed : 45;
  } else {
    speed = (speed >= -45) ?  speed : -45;
  }
  wheels[0].angle = speed;
  wheels[1].angle = speed;
}
void Car::moveWheels() {
  wheels[0].angle = 0;
  wheels[1].angle = 0;
}
void Car::moveCannon(double speed) {
  if (((int)angle % 360) > 90 || ((int) angle % 360) < -90) speed = -speed;
  speed = cannon->angle + speed;
  if (speed > 0) {
    speed = (speed <= 45) ?  speed : 45;
  } else {
    speed = (speed >= -45) ?  speed : -45;
  }
  cannon->angle = speed;
}
void Car::move(double speed) {
  double regulator = 10.0;
  angle += (speed >= 0) ? wheels[0].angle/regulator : -wheels[0].angle/regulator;
  double radians = (speed >= 0) ? angle* 0.0174533 : (angle - 180)* 0.0174533;
  double x = 0;
  double y = std::abs(speed);
  x = -y*sin(radians);
  y = y*cos(radians);
  position(new Point(position()->x + x,position()->y + y));
  moveCracks(speed);
}

void Car::moveCracks(double speed) {
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      double y = cracks[i][j]->position()->y + speed;
      cracks[i][j]->position(0,y);
      double distanceFromCenterOfWheel = cracks[i][j]->position()->y + -wheels[i].height/2.0 + wheels[i].height/3.0*(j%4);
      GLfloat color[3] = {158/256.0, 158/256.0, 158/256.0};
      if(distanceFromCenterOfWheel > wheels[i].height/2.0) {
        cracks[i].erase(cracks[i].begin() + j);
        cracks[i].push_back(new Rectangle(wheels[i].width, wheels[i].height/10.0, color));
      }
      if(distanceFromCenterOfWheel < -wheels[i].height/2.0) {
        cracks[i].erase(cracks[i].begin() + j);
        cracks[i].insert(cracks[i].begin(), new Rectangle(wheels[i].width, wheels[i].height/10.0, color));
      }
    }
  }
}

void Car::positionCannon() {
  cannon->position(0,body->height/2.0);
}
void Car::positionAxis() {
  axis[0].position(-body->width/2.0 - axis[0].width/2.0, body->height/6.0 + axis[0].height/2.0);
  axis[1].position(body->width/2.0 + axis[1].width/2.0, body->height/6.0 + axis[1].height/2.0);
  axis[2].position(-body->width/2.0 - axis[2].width/2.0, -body->height/2.0 + body->height/6.0 + axis[2].height/2.0);
  axis[3].position(body->width/2.0 + axis[3].width/2.0, -body->height/2.0 + body->height/6.0 + axis[2].height/2.0);
}
void Car::positionWheels() {
  wheels[0].position(axis[0].position()->x - wheels[0].width, axis[0].position()->y);
  wheels[1].position(axis[1].position()->x + wheels[1].width, axis[1].position()->y);
  wheels[2].position(axis[2].position()->x - wheels[2].width, axis[2].position()->y);
  wheels[3].position(axis[3].position()->x + wheels[3].width, axis[3].position()->y);
  for (size_t i = 0; i < 16; i++) {
    cracks[i/4][i%4]->position(0, cracks[i/4][i%4]->position()->y);
  }
}
void Car::position(Point *newPos) {
  Circle::position(newPos);
  positionAxis();
  positionCannon();
  positionWheels();
  // angle = wheels[0].angle;
}
void Car::position(double x , double y){
  Circle::position(x, y);
  positionAxis();
  positionCannon();
  positionWheels();
}

Point* Car::position() {
  return Circle::position();
}

void Car::texture(GLuint tex){
  Circle::texture(tex);
  body->texture(tex);
}


Circle* Car::shoot() {
  GLfloat materialEmission[] = {0.10, 0.10, 0.10, 1};
	GLfloat materialAmbient[] = {0, 0, 0, 0.2};
	GLfloat materialDifuse[] = {0, 0, 0, 0.2};
	GLfloat materialSpecular[] = {0, 0, 0, 1};
	GLfloat materialShininess[] = {0.0};
  GLfloat color[3] = {0,0,0};

  Circle* shot = new Circle(0.005, color, 0, materialEmission, materialAmbient, materialDifuse, materialSpecular, materialShininess);
  shot->position(new Point(position()));
  double radians = cannon->angle* 0.0174533;
  double initX = sin(-radians)*(cannon->height/2.0 + 0.005/2.0) + sin(-radians)*cannon->width;
  double initY = cannon->position()->y + cannon->height/2.0 + 0.005/2.0;
  double x = initX*cos(angle* 0.0174533) - initY*sin(angle* 0.0174533);
  double y = initX*sin(angle* 0.0174533) + initY*cos(angle* 0.0174533);
  shot->position(shot->position()->x + x, shot->position()->y + y);
  shot->angle = cannon->angle + angle;
  return shot;
}


void Car::angulation(double angle) {
  this->angle = angle;
}
double Car::angulation(){
  return angle;
}
int teste = 0;
void Car::draw() {
  glPushMatrix();
    // Circle::draw();
    glTranslatef(position()->x, position()->y,0);
    glRotatef(angle, 0, 0, 1);
    cannon->draw();
    body->draw();
    for (size_t i = 0; i < 4; i++)
      axis[i].draw();
    for (size_t i = 0; i < 4; i++){
      wheels[i].draw();
      glPushMatrix();
        glTranslatef(wheels[i].position()->x, wheels[i].position()->y, 0);
        if (i == 0 || i == 1)
          glRotatef(wheels[i].angle, 0, 0, 1);
        for (size_t j = 0; j < 4; j++) {
          glPushMatrix();
            glTranslatef(0, -wheels[i].height/2.0 + wheels[i].height/3.0*(j%4), 0);
            cracks[i][j]->draw();
          glPopMatrix();
        }
      glPopMatrix();
    }
  glPopMatrix();
}

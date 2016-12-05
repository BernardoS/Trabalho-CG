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
  body->depth(body->width/2);
}

double Car::depth(){
  return body->depth();
}
void Car::depth(double z){
  body->depth(z);
}
void Car::makeCannon() {
  GLfloat color[3] = {27/256.0,94/256.0,32/256.0};
  GLfloat emission[] = {0, 0, 0, 1};
	GLfloat ambient[] = {color[0], color[1], color[2], 1};
	GLfloat difuse[] = {color[0], color[1], color[2], 1};
	GLfloat specular[] = {0, 0, 0, 1};
	GLfloat shininess[] = {100.0};
  double width = body->width/2.0;
  cannon = new Rectangle(width/2.0, width*2, color, 0, emission, ambient, difuse, specular, shininess);
  cannon->depth(width/2);
}

void Car::makeWheels() {
  //Wheels
  GLfloat colorWheel[3] = {17/256.0,17/256.0,17/256.0};
  GLfloat emissionWheel[] = {0, 0, 0, 1};
	GLfloat ambientWheel[] = {colorWheel[0], colorWheel[1], colorWheel[2], 1};
	GLfloat difuseWheel[] = {colorWheel[0], colorWheel[1], colorWheel[2], 1};
	GLfloat specularWheel[] = {0, 0, 0, 1};
	GLfloat shininessWheel[] = {0.0};

  for (size_t i = 0; i < 4; i++) {
    wheels[i] = *(new Circle(body->width/2.0, colorWheel, 0, emissionWheel, ambientWheel, difuseWheel, specularWheel, shininessWheel));
    wheels[i].flip = true;
    wheels[i].depth(depth());
  }
}
void Car::makeAxis() {
  GLfloat color[3] = {33/256.0,33/256.0,33/256.0};
  GLfloat emission[] = {0, 0, 0, 1};
	GLfloat ambient[] = {color[0], color[1], color[2], 1};
	GLfloat difuse[] = {color[0], color[1], color[2], 1};
	GLfloat specular[] = {0, 0, 0, 1};
	GLfloat shininess[] = {0.0};
  double width = body->width/2.0;
  for (size_t i = 0; i < 4; i++) {
    axis[i] = *(new Rectangle(width, width/2.0,color, 0, emission, ambient, difuse, specular, shininess));
    axis[i].depth(width/2.0);
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
void Car::moveCannon(double speedX, double speedZ) {
  if (((int)angle % 360) > 90 || ((int) angle % 360) < -90){
    speedX = -speedX;
  }
  speedZ = -speedZ;
  // std::cout << speedX << std::endl;
  if(speedX != 0){
    speedX = cannon->angle + speedX;
    if (speedX > 0) {
      speedX = (speedX <= 45) ?  speedX : 45;
    } else {
      speedX = (speedX >= -45) ?  speedX : -45;
    }
    cannon->angle = speedX;
  }
  if (speedZ != 0) {
    speedZ = cannon->angleZ + speedZ;
    if (speedZ >= 0) {
      cannon->angleZ = (speedZ <= 45) ?  speedZ : 45;
    }
  }
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
  wheels[0].position(axis[0].position()->x - wheels[0].radius/2, axis[0].position()->y);
  wheels[1].position(axis[1].position()->x + wheels[1].radius/2, axis[1].position()->y);
  wheels[2].position(axis[2].position()->x - wheels[2].radius/2, axis[2].position()->y);
  wheels[3].position(axis[3].position()->x + wheels[3].radius/2, axis[3].position()->y);
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
	GLfloat materialAmbient[] = {0, 0, 0, 1};
	GLfloat materialDifuse[] = {0, 0, 0, 1};
	GLfloat materialSpecular[] = {0, 0, 0, 1};
	GLfloat materialShininess[] = {0.0};
  GLfloat color[3] = {0,0,0};

  Circle* shot = new Circle(0.005, color, 0, materialEmission, materialAmbient, materialDifuse, materialSpecular, materialShininess);
  shot->sphere = true;
  shot->position(new Point(position()));
  double radians = cannon->angle* 0.0174533;
  double initX = sin(-radians)*(cannon->height/2.0 + 0.005/2.0) + sin(-radians)*cannon->width;
  double initY = cannon->position()->y + cannon->height/2.0 + 0.005/2.0;
  double x = initX*cos(angle* 0.0174533) - initY*sin(angle* 0.0174533);
  double y = initX*sin(angle* 0.0174533) + initY*cos(angle* 0.0174533);
  shot->position(shot->position()->x + x, shot->position()->y + y);
  shot->angle = cannon->angle + angle;
  shot->angleZ = cannon->angleZ;
  shot->depth(wheels[0].radius + body->height/2 - cannon->height);
  return shot;
}


void Car::angulation(double angle) {
  this->angle = angle;
}
double Car::angulation(){
  return angle;
}
void Car::light() {
  glPushMatrix();
    GLfloat position[] = {0, 0, 0.01, 1.0};
    GLfloat difuse[] = {1.0, 1.0, 0, 1};
    GLfloat specular[] = {1.0, 1.0, 0, 1};
    GLfloat direction[] = {0.0, 1.0, 0};
    // GLfloat ambientLight0[] = {1, 1, 1, 1};
    // glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight0);
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, difuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 2);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);
  glPopMatrix();
}
int teste = 0;
void Car::draw(bool light) {
  glPushMatrix();
    Circle::draw();
    glTranslatef(position()->x, position()->y,wheels[0].radius);
    glRotatef(angle, 0, 0, 1);
    if (light) this->light();
    glPushMatrix();
      glTranslatef(0, 0, depth()/2 - cannon->depth()/2);
      cannon->draw();
    glPopMatrix();
    body->draw();
    for (size_t i = 0; i < 4; i++)
      axis[i].draw();
    for (size_t i = 0; i < 4; i++){
      wheels[i].draw();
    }
  glPopMatrix();
}

double Car::getWidth() {return body->width;}
double Car::getHeight() {return body->height;}

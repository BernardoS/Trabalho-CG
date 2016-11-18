#ifndef Car_H
#define Car_H

#include "Circle.h"
#include "Rectangle.h"
#include "Point.h"
#include <vector>

class Car : public Circle{
private:
  double angle;
  Rectangle* body;
  Rectangle axis[4];
  Rectangle wheels[4];
  Rectangle* cannon;
  std::vector<Rectangle*> cracks[4];
  double wheelAngle;
  void makeCannon();
  void makeAxis();
  void makeBody(double color[3]);
  void makeWheels();
  void positionCannon();
  void positionWheels();
  void positionAxis();
  void moveCracks(double speed);
public:
  Car();
  Car(Circle* area);
  Car(double radius);
  double angulation();
  Circle* shoot();
  void angulation(double angle);
  void draw();
  void moveWheels(double angle);
  void moveWheels();
  void moveCannon(double angle);
  void move(double speed);
  void position(double x , double y);
	void position(Point* newPos);
	Point* position();
};

#endif // Car_H

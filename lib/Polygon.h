#ifndef Polygon_H
#define Polygon_H

class Point;

class Polygon {
private:
	Point* pos;
public:
	double color[3];
  Polygon ();
	Polygon (Point* pos);
	Polygon (Point* pos, double color[3]);
	Polygon(Polygon* polygon);
	void position(double x , double y);
	void position(Point* newPos);
	Point* position();
};

#endif // Polygon_H

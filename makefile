all: trabalhocg staticlib

rebuild: clean all

trabalhocg: trabalhocg.cpp Point.a Polygon.a Circle.a Rectangle.a Car.a libtinyxml2.a -lglut -lGLU -lGL

clean:
	$(RM) ./lib/*.o trabalhocg libtinyxml2.a Point.a Polygon.a Circle.a Rectangle.a Car.a

run: clean trabalhocg
	./trabalhocg ${ARGS}

staticlib: libtinyxml2.a Point.a Polygon.a Circle.a Rectangle.a Car.a

libtinyxml2.a: ./lib/tinyxml2.o
	$(AR) $(ARFLAGS) $@ $^

tinyxml2.o: ./lib/tinyxml2.cpp ./lib/tinyxml2.h

Point.a: ./lib/Point.o
	$(AR) $(ARFLAGS) $@ $^

Point.o: ./lib/Point.cpp ./lib/Point.h

Polygon.a: ./lib/Polygon.o
	$(AR) $(ARFLAGS) $@ $^

Polygon.o: ./lib/Polygon.cpp ./lib/Polygon.h

Circle.a: ./lib/Circle.o
	$(AR) $(ARFLAGS) $@ $^

Circle.o: ./lib/Circle.cpp ./lib/Circle.h

Rectangle.a: ./lib/Rectangle.o
	$(AR) $(ARFLAGS) $@ $^

Rectangle.o: ./lib/Rectangle.cpp ./lib/Rectangle.h

Car.a: ./lib/Car.o
	$(AR) $(ARFLAGS) $@ $^

Car.o: ./lib/Car.cpp ./lib/Car.h

#include "lib/tinyxml2.h"
#include "lib/CImg.h"
#include "lib/Point.h"
#include "lib/Circle.h"
#include "lib/Rectangle.h"
#include "lib/Car.h"
#include <GL/glut.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string.h>

bool keyPressed[256];
bool gameOn = false;
bool gameOver = false;

using namespace tinyxml2;
using namespace cimg_library;
#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

struct janela_t {
	double width;
	double height;
	int color[3];
	char const* title;
} window;

double relativeX(double x) {
	return x/window.width;
}

double relativeY(double y) {
	int const height = window.height;
	return double(std::abs(height-y))/height;
}

double X;
double Y;
double carSpeed = 0.1;
double shotSpeed = 0.2;
double enemySpeed = 0.1;
double enemyShotSpeed = 0.2;
double enemyShotFreq = 0.0005;
int shotsTaken = 0;

Point* p = new Point();
Rectangle* shot = new Rectangle(0.0);
double shotPos = 0;
std::vector<Circle*> shots;
std::vector<Circle*> enemiesShots;
std::vector<Car*> Inimigos;
Rectangle* LargadaChegada = new Rectangle();
Circle Arena[2];
Car* Jogador = new Car();
std::vector<double> decayingAngle;

std::string readXml(XMLDocument &doc) {
  XMLElement* aplicacao = doc.FirstChildElement("aplicacao");
	XMLElement* carro = aplicacao->FirstChildElement("carro");
	XMLElement* carroInimigo = aplicacao->FirstChildElement("carroInimigo");
	carroInimigo->QueryDoubleAttribute("freqTiro", &enemyShotFreq);
	carroInimigo->QueryDoubleAttribute("velCarro", &enemySpeed);
	carroInimigo->QueryDoubleAttribute("velTiro", &enemyShotSpeed);
	carro->QueryDoubleAttribute("velTiro", &shotSpeed);
	carro->QueryDoubleAttribute("velCarro", &carSpeed);
	XMLElement* arquivoDaArena = aplicacao->FirstChildElement("arquivoDaArena");
	std::string path = arquivoDaArena->Attribute("caminho");
	path += arquivoDaArena->Attribute("nome");
	path += ".";
	path += arquivoDaArena->Attribute("tipo");
	return path;
}

void readSvg(XMLDocument &doc) {
	XMLElement* svg = doc.FirstChildElement();
	XMLElement* current = svg->FirstChildElement();
	while (current) {
		if (strcmp(current->Value(), "circle") == 0) {
			double cx, cy, r;
			current->QueryDoubleAttribute( "cx", &cx);
			current->QueryDoubleAttribute( "cy", &cy);
			current->QueryDoubleAttribute( "r", &r);
			const char* fill = current->Attribute("fill");
			if (strcmp(fill, "red") == 0) {
				double color[3] = {1,0,0};
				Car* Inimigo = new Car(new Circle(relativeX(r), color));
				Inimigo->position(relativeX(cx), relativeY(cy));
				Inimigos.push_back(Inimigo);
			} else if (strcmp(fill, "blue") == 0) {
				window.width = r*2;
				window.height = r*2;
				double color[3] = {0,0,1};
				Arena[0] = *(new Circle(relativeX(r),color));
				Arena[0].position(relativeX(cx), relativeY(cy));
			} else if (strcmp(fill, "white") == 0) {
				double color[3] = {1,1,1};
				Arena[1] = *(new Circle(relativeX(r), color));
				Arena[1].position(relativeX(cx), relativeY(cy));
			} else if (strcmp(fill, "green") == 0) {
				double color[3] = {0,1,0};
				Jogador = new Car(new Circle(relativeX(r),color));
				Jogador->position(relativeX(cx), relativeY(cy));
			}
		} else {
			double x, y, width, height;
			current->QueryDoubleAttribute( "x", &x);
			current->QueryDoubleAttribute( "y", &y);
			current->QueryDoubleAttribute( "width", &width);
			current->QueryDoubleAttribute( "height", &height);
			double color[3] = {0,0,0};
			LargadaChegada = new Rectangle(relativeX(width),relativeX(height),color);
			LargadaChegada->position(relativeX(x) + LargadaChegada->width/2.0, relativeY(y) - LargadaChegada->height/2.0);
		}
		current = current->NextSiblingElement();
	}
	decayingAngle = std::vector<double>(Inimigos.size(), 0.0);
}

void printTime(double timeDiff){
	static double totalTime = 0;
	totalTime += timeDiff/1000.0;
  //Create a string to be printed
	char str[200];
	sprintf(str, "Tempo: %.3f", totalTime);
	glPushMatrix();
  glRasterPos3f(0, 0, 0);
	glColor3ub(0,0,0);
  //Print each of the other Char at time
	const char* tmpStr = str;
  while(*tmpStr){
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
      tmpStr++;
  }
	glPopMatrix();
}


void keyUpFunc(unsigned char c, int i1, int i2) {
  keyPressed[int(c)] = false;
}
void keyPressFunc(unsigned char c, int i1, int i2) {
  keyPressed[int(c)] = true;
}
void displayFunc(){
	glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClear (GL_COLOR_BUFFER_BIT);
	Point* center = Arena[0].position();
	printTime(0);
	glPushMatrix();
	glTranslatef(relativeX(window.width)/2.0 - center->x, relativeX(window.height)/2.0 - center->y,0);
	glPushMatrix();
		Arena[0].draw();
		Arena[1].draw();
		LargadaChegada->draw();
		for (size_t i = 0; i < Inimigos.size(); i++) {
			Inimigos[i]->draw();
		}
		Jogador->draw();
	glPopMatrix();
	for (size_t i = 0; i < shots.size(); i++) {
		glPushMatrix();
			shots[i]->draw();
		glPopMatrix();
	}
	for (size_t i = 0; i < enemiesShots.size(); i++) {
		glPushMatrix();
			enemiesShots[i]->draw();
		glPopMatrix();
	}
	shotsTaken = 1;
	glPopMatrix();
	if (gameOver) {
		char str[200];
		if (shotsTaken > 0) sprintf(str, "You lose, %d shot(s) taken", shotsTaken);
		else sprintf(str, "You win!");
		glPushMatrix();
		glColor3ub(0,0,0);
		glRasterPos2f(relativeX(window.width)/2.0 - 4*relativeX(strlen(str)), relativeX(window.height)/2.0);
		//Print each of the other Char at time
		char* tmpStr = str;
		while(*tmpStr){
	      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
	      tmpStr++;
	  }
		glPopMatrix();
	}
	glutSwapBuffers();
}


bool canCircleMove(Circle* position) {
	for (size_t i = 0; i < Inimigos.size(); i++) {
		if (!Inimigos[i]->out(position) && Inimigos[i] != position){
			return false;
		}
	}
	if (std::find(Inimigos.begin(), Inimigos.end(), position) != Inimigos.end() && !position->out(Jogador)){
		return false;
	}
	return Arena[0].in(position) && Arena[1].out(position);
}
bool canEnemyShotMove(Circle* position) {
	if (!Jogador->out(position)) {
		return false;
	}
	return Arena[0].in(position) && Arena[1].out(position);
}

void killTarget(Circle* position) {
	for (size_t i = 0; i < Inimigos.size(); i++) {
		if (!Inimigos[i]->out(position)){
			Inimigos.erase(Inimigos.begin() + i);
		}
	}
}

void updatePlayer(double timeDiff) {
	double speed = relativeX(carSpeed*timeDiff);
	Point* oldPosition = new Point(Jogador->position());
	if ((keyPressed[int('w')] || keyPressed[int('W')])) {
		Jogador->move(speed);
	}
	if ((keyPressed[int('s')] || keyPressed[int('S')])) {
		Jogador->move(-speed);
	}
	if ((keyPressed[int('d')] || keyPressed[int('D')])) {
		Jogador->moveWheels(-1);
	}
	if ((keyPressed[int('a')] || keyPressed[int('A')])) {
		Jogador->moveWheels(1);
	}
	if (!canCircleMove(Jogador)) {
		Jogador->position(oldPosition);
	}
}

double mouseX = 0;
void motionFunc(int x, int y) {
	double relative = relativeX(x);
	if (relative > mouseX) Jogador->moveCannon(-1);
	else Jogador->moveCannon(1);
	mouseX = relative;
}

void mouseFunc(int button, int state, int x, int y) {
	if (button == 0 && state == 1)
		shots.push_back(Jogador->shoot());
}

void updateShot(double timeDiff) {
	double speed = relativeX(shotSpeed*timeDiff);
	for (size_t i = 0; i < shots.size(); i++) {
		double radians = shots[i]->angle * 0.0174533;
		Point* position = shots[i]->position();
		position->x += -speed*sin(radians);
		position->y += speed*cos(radians);
		if (!canCircleMove(shots[i])) {
			killTarget(shots[i]);
			shots.erase(shots.begin() + i);
		}
	}
}
void updateEnemiesShot(double timeDiff) {
	double speed = relativeX(shotSpeed*timeDiff);
	for (size_t i = 0; i < enemiesShots.size(); i++) {
		double radians = enemiesShots[i]->angle * 0.0174533;
		Point* position = enemiesShots[i]->position();
		position->x += -speed*sin(radians);
		position->y += speed*cos(radians);
		if (!canEnemyShotMove(enemiesShots[i])) {
			if (!enemiesShots[i]->out(Jogador)) shotsTaken++;
			enemiesShots.erase(enemiesShots.begin() + i);
		}
	}
}

void shoot() {
	for (size_t i = 0; i < Inimigos.size(); i++)
		enemiesShots.push_back(Inimigos[i]->shoot());
}

double vectorialAngle(Point* A, Point* B) {
	double dotProduct = A->x * B->x + A->y * B->y;
	double magnitude = sqrt(pow(A->x, 2) + pow(A->y, 2)) * sqrt(pow(B->x, 2) + pow(B->y, 2));
	if (acos(dotProduct/magnitude)*57.2958 > 0) {
		return acos(dotProduct/magnitude)*57.2958;
	}
	return 0;
}

double angle() {
	static Point* initalPos = Jogador->position();
	static Point* center = Arena[0].position();
	static Point* previousPos = Jogador->position();
	Point* currentPos = Jogador->position();
	static double acc = 0;
	static double newPositionAngle = 0;
	Point* initialVector = new Point(initalPos->x - center->x, initalPos->y - center->y);
	double aux = vectorialAngle(new Point(previousPos->x - center->x, previousPos->y - center->y), new Point(currentPos->x - center->x, currentPos->y - center->y));
	double currentAngle = vectorialAngle(initialVector, new Point(currentPos->x - center->x, currentPos->y - center->y));
	double previousAngle = vectorialAngle(initialVector, new Point(previousPos->x - center->x, previousPos->y - center->y));
	double aux2 = newPositionAngle == aux ? 0 : aux;
	if (center->y <= currentPos->y) {
		acc += previousAngle < currentAngle ? aux2 : -aux2;
	} else {
		acc += previousAngle > currentAngle ? aux2 : -aux2;
	}
	newPositionAngle = aux;
	previousPos = currentPos;
	return acc;
}

void updateGame(double timeDiff) {
	double position = angle();
	if (position != 0) gameOn = true;
	if (!gameOver && gameOn) printTime(timeDiff);
	if(position >= 360) gameOver = true;
}


void moveEnemies(double timeDiff) {
	double r = ((double) rand() / (RAND_MAX));
	// std::cout << r << std::endl;
	double speed = relativeX(enemySpeed*timeDiff);
	for (size_t i = 0; i < Inimigos.size(); i++) {
		Point* oldPosition = new Point(Inimigos[i]->position());
		Inimigos[i]->move(speed);
		if (!canCircleMove(Inimigos[i])) {
			decayingAngle.insert(decayingAngle.begin() + i, r*45.0);
			Inimigos[i]->moveWheels((Arena[0].in(Inimigos[i])) ? -r : r);
		} else if(decayingAngle[i]-- <= 0) {
			Inimigos[i]->moveWheels();
		}
		if (!canCircleMove(Inimigos[i])) {
			Inimigos[i]->position(oldPosition);
		}
	}
}

void updateEnemies(double timeDiff) {
	static GLdouble lastTimeEnemiesShot = 0;
	updateEnemiesShot(timeDiff);
	if (lastTimeEnemiesShot > 1/enemyShotFreq) {
		lastTimeEnemiesShot = 0;
		shoot();
	}
	moveEnemies(timeDiff);
	lastTimeEnemiesShot += timeDiff;
}

void idleFunc() {
	static GLdouble previousTime = 0;
	GLdouble currentTime;
	GLdouble timeDiference;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	timeDiference = currentTime - previousTime;
	previousTime = currentTime;
	updatePlayer(timeDiference);
	updateShot(timeDiference);
	updateEnemies(timeDiference);
	updateGame(timeDiference);
	glutPostRedisplay();
}

void init() {
  glClearColor(1,1,1,0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
}

int main(int argc, char **argv) {
	if (argc <= 1) {
		std::cerr << "Missing arguments" << std::endl;
		return 1;
	}
	std::string path = argv[1];
	XMLDocument xml;
	path += "config.xml";
	xml.LoadFile(path.c_str());
	if (xml.Error()) {
		std::cerr << xml.ErrorName() << std::endl;
		return xml.ErrorID();
	}
	std::string svgFile = readXml(xml);
	XMLDocument svg;
	svg.LoadFile(svgFile.c_str());
	readSvg(svg);
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(window.width, window.height);
  glutCreateWindow("Arena");
	init();
	glutKeyboardFunc(keyPressFunc);
  glutKeyboardUpFunc(keyUpFunc);
	glutDisplayFunc(displayFunc);
	glutMouseFunc(mouseFunc);
	glutPassiveMotionFunc(motionFunc);
	glutIdleFunc(idleFunc);
	glutMainLoop();
  return 0;
}

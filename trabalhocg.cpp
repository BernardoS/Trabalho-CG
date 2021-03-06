#include "lib/tinyxml2.h"
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
#include "lib/bmpread.h"

bool keyPressed[256];
bool gameOn = false;
bool gameOver = false;
bool nightMode = false;

using namespace tinyxml2;
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

GLuint loadTexture(const char* filename){
	GLuint texture = 0;
	bmpread_t bitmap;
	if(!bmpread(filename, 0, &bitmap)){
			fprintf(stderr, "%s: error loading bitmap file\n", filename);
			exit(1);
	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data);
	bmpread_free(&bitmap);
	return texture;
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
std::vector<Circle*> shots;
std::vector<Circle*> enemiesShots;
std::vector<Car*> Inimigos;
Rectangle* LargadaChegada = new Rectangle();
Circle Arena[2];
Car* Jogador = new Car();
std::vector<double> decayingAngle;

GLuint floorTexture;
GLuint carTexture;
GLuint startTexture;

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

void makeArena(const char* fill, double r, double x, double y){
	if (strcmp(fill, "blue") == 0) {
		window.width = r*2;
		window.height = r*2;

		GLfloat color[3] = {0,0,1};
		GLfloat materialEmission[] = {0, 0, 0, 1};
		GLfloat materialAmbient[] = {0.5, 0.5, 0.5, 1};
		GLfloat materialDifuse[] = {0, 0, 0, 1};
		GLfloat materialSpecular[] = {0, 0, 0, 1};
		GLfloat materialShininess[] = {100.0};

		Arena[0] = *(new Circle(relativeX(r),color, 0, materialEmission, materialAmbient, materialDifuse, materialSpecular, materialShininess));
		Arena[0].position(relativeX(x), relativeY(y));
	} else {
		GLfloat color[3] = {1,1,1};

		GLfloat materialEmission[] = {0, 0, 0, 1};
		GLfloat materialAmbient[] = {0.5, 0.5, 0.5, 1};
		GLfloat materialDifuse[] = {0, 0, 0, 1};
		GLfloat materialSpecular[] = {0, 0, 0, 1};
		GLfloat materialShininess[] = {0.0};

		Arena[1] = *(new Circle(relativeX(r), color, 0, materialEmission, materialAmbient, materialDifuse, materialSpecular, materialShininess));
		Arena[1].position(relativeX(x), relativeY(y));
	}
}
void makeEnemy(double r, double x, double y){
	GLfloat color[3] = {1,0,0};

	GLfloat materialEmission[] = {0.1, 0, 0, 1};
	GLfloat materialAmbient[] = {1, 0, 0, 1};
	GLfloat materialDifuse[] = {1, 0, 0, 1};
	GLfloat materialSpecular[] = {1, 0, 0, 1};
	GLfloat materialShininess[] = {100.0};

	Car* Inimigo = new Car(new Circle(relativeX(r), color, 0, materialEmission, materialAmbient, materialDifuse, materialSpecular, materialShininess));
	Inimigo->position(relativeX(x), relativeY(y));
	Inimigos.push_back(Inimigo);
}

void makePlayer(double r, double x, double y){
	GLfloat color[3] = {0,1,0};

	GLfloat materialEmission[] = {0, 0.1, 0, 1};
	GLfloat materialAmbient[] = {0, 1, 0, 1};
	GLfloat materialDifuse[] = {0, 1, 0, 1};
	GLfloat materialSpecular[] = {0, 1, 0, 1};
	GLfloat materialShininess[] = {100.0};

	Jogador = new Car(new Circle(relativeX(r), color, 0, materialEmission, materialAmbient, materialDifuse, materialSpecular, materialShininess));
	Jogador->position(relativeX(x), relativeY(y));
	Arena[0].depth(Jogador->depth()*8);
	Arena[1].depth(Jogador->depth()*8);
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
				makeEnemy(r, cx, cy);
			} else if (strcmp(fill, "blue") == 0) {
				makeArena(fill, r, cx, cx);
			} else if (strcmp(fill, "white") == 0) {
				makeArena(fill, r, cx, cy);
			} else if (strcmp(fill, "green") == 0) {
				makePlayer(r, cx, cy);
			}
		} else {
			double x, y, width, height;
			current->QueryDoubleAttribute( "x", &x);
			current->QueryDoubleAttribute( "y", &y);
			current->QueryDoubleAttribute( "width", &width);
			current->QueryDoubleAttribute( "height", &height);
			GLfloat color[3] = {0,0,0};
			GLfloat materialEmission[] = {0.1, 0.1, 0.1, 0.5};
			GLfloat materialAmbient[] = {0, 0, 0, 0.5};
			GLfloat materialDifuse[] = {0, 0, 0, 0.5};
			GLfloat materialSpecular[] = {0, 0, 0, 1};
			GLfloat materialShininess[] = {0.0};
			LargadaChegada = new Rectangle(relativeX(width),relativeX(height),color, 0, materialEmission, materialAmbient, materialDifuse, materialSpecular, materialShininess);
			LargadaChegada->position(relativeX(x) + LargadaChegada->width/2.0, relativeY(y) - LargadaChegada->height/2.0);
			LargadaChegada->depth(0.0001);
		}
		current = current->NextSiblingElement();
	}
	decayingAngle = std::vector<double>(Inimigos.size(), 0.0);
}

void printGameStatus() {
	char str[200];
	if (shotsTaken > 0) sprintf(str, "You lose, %d shot(s) taken", shotsTaken);
	else sprintf(str, "You win!");
	glMatrixMode (GL_PROJECTION);
	//Push to recover original PROJECTION MATRIX
	glPushMatrix();
			glLoadIdentity ();
			glOrtho (0, 1, 0, 1, -1, 1);
			glPushAttrib(GL_ENABLE_BIT);
	        glDisable(GL_LIGHTING);
	        glDisable(GL_TEXTURE_2D);
	        //Draw text in the x, y, z position
	        glColor3f(1,1,1);
	        glRasterPos3f(0.5, 0.5, 0);
	        const char* tmpStr;
	        tmpStr = str;
	        while( *tmpStr ){
	            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
	            tmpStr++;
	        }
	    glPopAttrib();
	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);
}

void printTime(double timeDiff){
	static double totalTime = 0;
	totalTime += timeDiff/1000.0;
  //Create a string to be printed
	char str[200];
	sprintf(str, "Tempo: %.3f", totalTime);
	//Draw text considering a 2D space (disable all 3d features)
	//Push to recover original PROJECTION MATRIX
	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity ();
		glOrtho(0, 1, 0, 1, -1, 1);
		glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        //Draw text in the x, y, z position
        glColor3f(1,1,1);
        glRasterPos3f(0, 0, 0);
        const char* tmpStr;
        tmpStr = str;
        while( *tmpStr ){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
            tmpStr++;
        }
    glPopAttrib();
		glMatrixMode (GL_MODELVIEW);
	glPopMatrix();
}

double camDist = 0;
double camXYAngle = 0;
double camXZAngle = 0;
int camAngle = 0;
double camX = 0;
double camY = 0;
int toggleCam = 0;
double camposx = 1;
double camposy = 100;
double camposz = 100;
double camtheta = 0;

void keyUpFunc(unsigned char c, int i1, int i2) {
  keyPressed[int(c)] = false;
	if (c == 'n') nightMode = !nightMode;
	if (c == '1') toggleCam = 1;
	if (c == '2') toggleCam = 2;
	if (c == '3') toggleCam = 3;
	if (c == '0') toggleCam = 0;
	if (c == 'f') camposx += 1;
	if (c == 'h') camposy += 0.01;
	if (c == 'k') camposz += 1;
	if (c == 'g') camposx -= 1;
	if (c == 'j') camposy -= 0.01;
	if (c == 'l') camposz -= 1;
	if (c == '+') camtheta += 1;
	if (c == '-') camtheta -= 1;
}
void keyPressFunc(unsigned char c, int i1, int i2) {
  keyPressed[int(c)] = true;
}

void makeLight() {
	GLfloat ambientLight0[] = {1, 1, 1, 1};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
}

void displayFunc(){
	glMatrixMode(GL_MODELVIEW);
	glClearColor (0, 0, 0,0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	makeLight();
	printTime(0);
	if (gameOver) printGameStatus();
	// cout << "player at ( " << Jogador->position()->x << " , " << Jogador->position()->y << " , " << Jogador->depth() << " )" << endl;
	// cout << "camera position ( " << Jogador->position()->x + (Jogador->getWidth() / 2) * cos(Jogador->angulation() * M_PI / 180);
	// cout << " , " << Jogador->position()->y + (Jogador->getHeight() / 2) * sin(Jogador->angulation() * M_PI / 180);
	// cout << " , " << Jogador->depth() << " )" << endl;
	// cout << "camera looking at ( " << Jogador->position()->x + (Jogador->radius) * cos(Jogador->angulation() * M_PI / 180);
	// cout << " , " << Jogador->position()->y + (Jogador->radius) * sin(Jogador->angulation() * M_PI / 180);
	// cout << " , " << Jogador->depth() << " )" << endl;
	if(toggleCam == 1) { // cockpit cam
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// fov, aspect ratio, near, far
		gluPerspective(45, 1, 0.0001, 100);
		glMatrixMode(GL_MODELVIEW);
		// camera position, camera looking at, up vector
		gluLookAt(
			Jogador->position()->x + (Jogador->getWidth() / 2) * -sin(Jogador->angulation() * M_PI / 180),
			Jogador->position()->y + (Jogador->getHeight() / 2) * cos(Jogador->angulation() * M_PI / 180),
			Jogador->depth(),
			Jogador->position()->x + (Jogador->radius) * -sin(Jogador->angulation() * M_PI / 180),
			Jogador->position()->y + (Jogador->radius) * cos(Jogador->angulation() * M_PI / 180),
			Jogador->depth(),
			0, 0, 1
		);
	} else if(toggleCam == 2) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// fov, aspect ratio, near, far
		gluPerspective(45, 1, 0.0001, 100);
		glMatrixMode(GL_MODELVIEW);
		glTranslatef(0.05, 0, 0);
		// camera position, camera looking at, up vector
		gluLookAt(
			Jogador->position()->x + (Jogador->getWidth() * 2/3) * -sin(Jogador->angulation() * M_PI / 180),
			Jogador->position()->y + (Jogador->getWidth() * 2/3) * cos(Jogador->angulation() * M_PI / 180),
			Jogador->depth(),
			Jogador->position()->x + (Jogador->getWidth()) * -sin(Jogador->angulation() * M_PI / 180),
			Jogador->position()->y + (Jogador->getWidth()) * cos(Jogador->angulation() * M_PI / 180),
			Jogador->depth(),
			0, 0, 1
		);
	} else if(toggleCam == 3) { // third person cam
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// fov, aspect ratio, near, far
		gluPerspective(100, 1, 0.0001, 100);
		glMatrixMode(GL_MODELVIEW);
		// camera position, camera looking at, up vector
		gluLookAt(
			Jogador->position()->x - (Jogador->radius) * -sin(Jogador->angulation() * M_PI / 180),
			Jogador->position()->y - (Jogador->radius) * cos(Jogador->angulation() * M_PI / 180),
			Jogador->depth() * 3,
			Jogador->position()->x,
			Jogador->position()->y,
			Jogador->depth(),
			0, 0, 1
		);
	} else { /* default 2d camera */ }

	glPushMatrix();
		glPushMatrix();
			Arena[0].draw();
			// Arena[0].drawWall(); // luiz
			Arena[1].draw();
			// Arena[1].drawWall(); // luiz
			LargadaChegada->draw();
			for (size_t i = 0; i < Inimigos.size(); i++) {
				Inimigos[i]->draw(false, false, false);
			}
			Jogador->draw(nightMode, false, false);
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
	glPopMatrix();
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
double mouseY = 0;
int lastX = 0;
int lastY = 0;
int buttonDown = 0;
void motionFunc(int x, int y) {
	// std::cout << (relativeY(y) - mouseY >= 0) << std::endl;
	double xMove = (relativeX(x) > mouseX) ? -1 : 1;
	double yMove = (relativeY(y) >= mouseY) ? 1 : -1;
	Jogador->moveCannon(xMove, yMove*0.05);
	mouseX = relativeX(x);
	mouseY = relativeY(y);
}

void mouseFunc(int button, int state, int x, int y) {
	if (button == 0 && state == 1) {
		buttonDown = 0;
		shots.push_back(Jogador->shoot());
	}
	if (button == GLUT_LEFT_BUTTON &&
					state == GLUT_DOWN) {
						lastX = x;
						lastY = y;
						buttonDown = 1;
	}
}

void mouse_motion(int x, int y)
{
    if (!buttonDown)
        return;

    camXYAngle += x - lastX;
    camXZAngle += y - lastY;

    camXYAngle = (int)camXYAngle % 360;
    camXZAngle = (int)camXZAngle % 360;

    lastX = x;
    lastY = y;
    glutPostRedisplay();
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
	if (position > 0) gameOn = true;
	if (!gameOver && gameOn) printTime(timeDiff);
	if(position >= 360) gameOver = true;
}

void moveEnemies(double timeDiff) {
	double r = ((double) rand() / (RAND_MAX));
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
	if (nightMode) {
		glDisable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}
	else {
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
	}
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHT0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);

	floorTexture = loadTexture("textures/floor.bmp");
	// startTexture = loadTexture("textures/start.bmp");
	carTexture = loadTexture("textures/car.bmp");
	Arena[1].texture(floorTexture);
	Arena[0].texture(floorTexture);
	// LargadaChegada->texture(startTexture);
	Jogador->texture(carTexture);
	for (size_t i = 0; i < Inimigos.size(); i++) {
		Inimigos.at(i)->texture(carTexture);
	}
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window.width, window.height);
  glutCreateWindow("Arena");
	init();
	glutKeyboardFunc(keyPressFunc);
  glutKeyboardUpFunc(keyUpFunc);
	glutDisplayFunc(displayFunc);
	glutMouseFunc(mouseFunc);
	glutPassiveMotionFunc(motionFunc);
	glutMotionFunc(mouse_motion);
	glutIdleFunc(idleFunc);
	glutMainLoop();
  return 0;
}

#include <gl/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <ctime>
#include <deque>

#define UNUSED(param) (void)(param)

// Directiile de deplasare ale Snake
#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4

char title[] = "Snake";
float walls = 30.0f;

//Pozitia si directia initiale si viteza de deplasare a lui Snake
int direction = DOWN;
int speed = 150;
bool moved = false;
std::deque< std::deque<float> > Snake_body;

//Pentru generarea de FOOD
bool available_food = false;

//Coordonatele lui FOOD
int Food_coord[8];


void display1() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	char text[] = "GAME OVER!";
	glColor3f(0.0, 1.0, 0.0);
	glRasterPos2f(0.0f, 0.0f);
	for (int i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);


	glutSwapBuffers();
}

void display2() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	char text[] = "You've won!";
	glColor3f(1.0, 0.0, 1.0);
	glRasterPos2f(0.0f, 0.0f);
	for (int i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);


	glutSwapBuffers();
}

void FOOD() {
	if (!available_food) {
		while (true) {
			//Intial Snake nu atinge FOOD, asa ca coolides este false
			bool collides = false;

			//Creem o variabila temporara pentru FOOD
			int temp_Food_coord[2] = { Food_coord[0] = 2 * (rand() % ((int)walls + 1)) - (int)walls,
				Food_coord[1] = 2 * (rand() % ((int)walls + 1)) - (int)walls };

			//Coincide Snake cu temporara FOOD?(parti comune)
			for (unsigned int a = 0; a < Snake_body.size(); a++) {
				if (temp_Food_coord[0] == Snake_body[a][0] &&
					temp_Food_coord[1] == Snake_body[a][1]) {
					collides = true;
				}
			}

			//Daca parti din Snake nu coincid cu FOOD, atunci FOOD ia coordonatele variabilei temporare
			if (collides == false) {
				Food_coord[0] = temp_Food_coord[0];
				Food_coord[1] = temp_Food_coord[1];

				available_food = true;

				break;
			}
		}
	}

	glLoadIdentity();
	glTranslatef(Food_coord[0], Food_coord[1], -40.0f);


	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex2d(1.0f, 1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2d(1.0f, -1.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2d(-1.0f, -1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2d(-1.0f, 1.0f);
	glEnd();
}


void moveSnake(int new_direction) {
	direction = new_direction;

	int last_part = Snake_body.size() - 1;
	std::deque<float> new_body = Snake_body[last_part];

	if (direction == UP) {
		//Snake s-a ciocnit de coada lui?
		for (unsigned int a = 0; a < Snake_body.size(); a++) {
			if (Snake_body[0][0] == Snake_body[a][0] &&
				Snake_body[0][1] + 2.0f == Snake_body[a][1]) {
				glutDisplayFunc(display1);
			}
		}

		//Snake s-a ciocnit de un perete?
		if (Snake_body[0][1] == walls) {
			glutDisplayFunc(display1);
		}

		//Snake a prins FOOD?
		if (Snake_body[0][0] == Food_coord[0] &&
			Snake_body[0][1] + 2.0f == Food_coord[1]) {

			//Micsorarea lungimii lui Snake cand a prins FOOD
			Snake_body.pop_back();
			available_food = false;
		}

		new_body[1] = Snake_body[0][1] + 2.0f;
	}
	else if (direction == DOWN) {
		//Snake s-a ciocnit de coada lui?
		for (unsigned int a = 0; a < Snake_body.size(); a++) {
			if (Snake_body[0][0] == Snake_body[a][0] &&
				Snake_body[0][1] - 2.0f == Snake_body[a][1]) {
				glutDisplayFunc(display1);
			}
		}

		//Snake s-a ciocnit de un perete?
		if (Snake_body[0][1] == -walls) {
			glutDisplayFunc(display1);
		}

		//Snake a prins FOOD?
		if (Snake_body[0][0] == Food_coord[0] &&
			Snake_body[0][1] - 2.0f == Food_coord[1]) {

			//Micsorarea lungimii lui Snake cand a prins FOOD
			Snake_body.pop_back();
			available_food = false;
		}

		new_body[1] = Snake_body[0][1] - 2.0f;
	}
	else {
		new_body[1] = Snake_body[0][1];
	}

	if (direction == LEFT) {
		//Snake s-a ciocnit de coada lui?
		for (unsigned int a = 0; a < Snake_body.size(); a++) {
			if (Snake_body[0][0] - 2.0f == Snake_body[a][0] &&
				Snake_body[0][1] == Snake_body[a][1]) {
				glutDisplayFunc(display1);
			}
		}

		//Snake s-a ciocnit de un perete?
		if (Snake_body[0][0] == -walls) {
			glutDisplayFunc(display1);
		}

		//Snake a prins FOOD?
		if (Snake_body[0][0] - 2.0f == Food_coord[0] &&
			Snake_body[0][1] == Food_coord[1]) {

			//Micsorarea lungimii lui Snake cand a prins FOOD
			Snake_body.pop_back();
			available_food = false;
		}

		new_body[0] = Snake_body[0][0] - 2.0f;
	}
	else if (direction == RIGHT) {
		//Snake s-a ciocnit de coada lui?
		for (unsigned int a = 0; a < Snake_body.size(); a++) {
			if (Snake_body[0][0] + 2.0f == Snake_body[a][0] &&
				Snake_body[0][1] == Snake_body[a][1]) {
				glutDisplayFunc(display1);
			}
		}

		//Snake s-a ciocnit de un perete?
		if (Snake_body[0][0] == walls) {
			glutDisplayFunc(display1);
		}

		//Snake a prins FOOD?
		if (Snake_body[0][0] + 2.0f == Food_coord[0] &&
			Snake_body[0][1] == Food_coord[1]) {

			//Micsorarea lungimii lui Snake cand a prins FOOD
			Snake_body.pop_back();
			available_food = false;
		}

		new_body[0] = Snake_body[0][0] + 2.0f;
	}
	else {
		new_body[0] = Snake_body[0][0];
	}

	Snake_body.push_front(new_body);
	Snake_body.pop_back();


	//Sfarsitul jocului
	if (Snake_body.size() == 1)
	{

		glutDisplayFunc(display2);

	}

	glutPostRedisplay();
}

//Setarea tastaturii. Snake nu poate fi deplasat in spate
void keyboard(int key, int x, int y) {
	UNUSED(x);
	UNUSED(y);

	switch (key) {
	case GLUT_KEY_UP: {
		if (direction == LEFT || direction == RIGHT) {
			moved = true;
			moveSnake(UP);
		}

		break;
	}

	case GLUT_KEY_DOWN: {
		if (direction == LEFT || direction == RIGHT) {
			moved = true;
			moveSnake(DOWN);
		}

		break;
	}

	case GLUT_KEY_LEFT: {
		if (direction == UP || direction == DOWN) {
			moved = true;
			moveSnake(LEFT);
		}

		break;
	}

	case GLUT_KEY_RIGHT: {
		if (direction == UP || direction == DOWN) {
			moved = true;
			moveSnake(RIGHT);
		}

		break;
	}
	}

	glutPostRedisplay();
}


void initGL() {
	glMatrixMode(GL_PROJECTION);
	gluPerspective(75.0f, 1, 0.0f, 35.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//Miscare lui Snake pe o directie
void moveSnakeAuto(int value) {
	if (!moved) {
		UNUSED(value);

		if (direction == UP) {
			moveSnake(UP);
		}
		else if (direction == DOWN) {
			moveSnake(DOWN);
		}
		else if (direction == LEFT) {
			moveSnake(LEFT);
		}
		else if (direction == RIGHT) {
			moveSnake(RIGHT);
		}
	}
	else {
		moved = false;
	}

	glutTimerFunc(speed, moveSnakeAuto, 0);
}



void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//Ordinea varfurilor este in sensul acelor de ceasornic

	//Se face bucla recursiva peste dimensiunea snake si deseneaza fiecare parte la coordonatele sale
	for (unsigned int a = 0; a < Snake_body.size(); a++) {
		glLoadIdentity();
		glTranslatef(Snake_body[a][0], Snake_body[a][1], -40.0f);

		glBegin(GL_POLYGON);

		glColor3f(0.0f, 0.5f, 0.0f);
		glVertex2d(1.0f, 1.0f);
		glColor3f(0.0f, 0.5f, 0.5f);
		glVertex2d(1.0f, -1.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2d(-1.0f, -1.0f);
		glColor3f(0.0f, 0.5f, 1.0f);
		glVertex2d(-1.0f, 1.0f);
		glEnd();


	}


	FOOD();

	glutSwapBuffers();
}



int main(int argc, char** argv) {


	glutInit(&argc, argv);

	glutInitWindowSize(600, 600);
	glutCreateWindow(title);

	glutDisplayFunc(display);
	glutSpecialFunc(keyboard);
	glutTimerFunc(speed, moveSnakeAuto, 0);

	//Lungimea lui Snake
	int initSize = 5;

	//Creearea lui Snake(coordonate)
	for (int a = 1; a <= initSize; a++) {
		std::deque<float> new_body;

		new_body.push_back(0.0f);
		new_body.push_back((walls + 2.0f + (initSize * 2)) - (a * 2));

		Snake_body.push_front(new_body);
	}

	srand(time(NULL));

	initGL();
	glutMainLoop();

	return 0;
}

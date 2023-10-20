#include <stdio.h>
#include <math.h>
#include <random>
#include <ctime> 
#include <glut.h>
//#include <Windows.h>
//#include <mmsystem.h>
int keyX = 450;
int keyY = 350;

int score = 0;
int countdownTime = 30;
int previousTime = 0;

const int ObstacleNumber = 5;
int ObstacleX[ObstacleNumber];
int ObstacleY[ObstacleNumber];

const int CollectableNumber = 5;
int CollectableX[CollectableNumber];
int CollectableY[CollectableNumber];

int HP = 5;

int TimePowerUpX = (rand() % 70 + 10) * 10;
int TimePowerUpY = (rand() % 50 + 10) * 10;
bool startFreezeTimerPowerUpTimer = false;
int freezeTimeCountDown = 10;

int ScoreMultiplierPowerUpX = (rand() % 70 + 10) * 10;
int ScoreMultiplierPowerUpY = (rand() % 50 + 10) * 10;
int scoreMultiplier = 1;
bool startPowerUpTimer = false;
int powerUpCountDown = 10;

bool AreObstaclesSpawned = false;
bool AreCollectablesSpawned = false;

bool reachedGoal = false;

//animations
float playerRotation = 0.0f;  
float collectableRotationAngle = 0.0f;
float timePowerUpRotationAngle = 0.0f;
float scorePowerUpRotationAngle = 0.0f;
float goalRotationAngle = 0.0f;
float backgroundRotationAngle = 0.0f;

//BONUS


void print(int x, int y, char* string)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}


void drawX(int x, int y) {
	glLineWidth(10.0);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x+50, y+50);
	glVertex2f(x + 50, y);
	glVertex2f(x, y+50);
	glEnd();
	glLineWidth(1.0);
}

void drawRect(int x, int y, int w, int h) {
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}
void drawObstacle(int x, int y) {
	glColor3f(1.0, 0.0, 0.0);
	drawRect(x, y, 80, 50);
	glColor3f(0.0, 0.0, 0.0);
	drawRect(x+5, y+5, 70, 40);
	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(2.0);  
	for (int i = 0; i < 70; i+=10) {
		glBegin(GL_LINES);
		glVertex2f(x + 10 + i, y);
		glVertex2f(x + 10 + i, y + 50);
		glEnd();

	}
	//glFlush();
}

void spawnObstacles() {
	for (int i = 0; i < ObstacleNumber; i++) {
		drawObstacle(ObstacleX[i], ObstacleY[i]);
	}
}

void drawCircle(int x, int y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}

void drawBordaries() {

	glColor3f(1.0, 0.0, 0.0);
	drawRect(0, 0, 900, 50);
	drawRect(0, 0, 50, 650);
	drawRect(850, 0, 50, 650);
	drawRect(0, 600, 900, 50);

	//bottom
	for (int i = 0; i <= 900; i += 50) {
		glPushMatrix();
		glColor3f(1.0, 0.5, 0.0);
		glBegin(GL_TRIANGLES);
		glVertex2f(i, 0);
		glVertex2f(i + 25, 50);
		glVertex2f(i + 50, 0);
		glEnd();
		glPopMatrix();
	}

	//up
	for (int i = 0; i <= 900; i += 50) {
		glPushMatrix();
		glColor3f(1.0, 0.5, 0.0);
		glTranslatef(450, 350, 0.0);
		glRotated(180, 0, 0, 1);
		glTranslatef(-450, -350, 0.0);
		glBegin(GL_TRIANGLES);
		glVertex2f(i, 50);
		glVertex2f(i + 25, 100);
		glVertex2f(i + 50, 50);
		glEnd();
		glPopMatrix();
	}


	//right
	for (int i = 150; i <= 650; i += 50) {
		glPushMatrix();
		glColor3f(1.0, 0.5, 0.0);
		glTranslatef(450, 350, 0.0);
		glRotated(90, 0, 0, 1);
		glTranslatef(-450, -350, 0.0);
		glBegin(GL_TRIANGLES);
		glVertex2f(i, -100);
		glVertex2f(i + 25, -50);
		glVertex2f(i + 50, -100);
		glEnd();
		glPopMatrix();
	}

	//left
	for (int i = 200; i <= 700; i += 50) {
		glPushMatrix();
		glColor3f(1.0, 0.5, 0.0);
		glTranslatef(450, 350, 0.0);
		glRotated(270, 0, 0, 1);
		glTranslatef(-450, -350, 0.0);
		glBegin(GL_TRIANGLES);
		glVertex2f(i, -100);
		glVertex2f(i + 25, -50);
		glVertex2f(i + 50, -100);
		glEnd();
		glPopMatrix();
	}


}

void drawHealthBar(int HP) {

	for (int i = 55; i <= HP * 55; i += 55) {
		glColor3f(0.0, 1.0, 0.0);
		drawRect(i, 660, 50, 30);
	}
}

void drawCollectable(float x, float y, float rotationAngle) {
	glColor3f(1.0, 1.0, 0.0);

	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glRotatef(rotationAngle, 0.0, 0.0, 1.0);
	drawCircle(0, 0, 15);

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(-10, -5);
	glVertex2f(10, -5);
	glVertex2f(10, 5);
	glVertex2f(-10, 5);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(-5, -10);
	glVertex2f(5, -10);
	glVertex2f(5, 10);
	glVertex2f(-5, 10);
	glEnd();

	glPopMatrix();
}
void spawnCollectables() {
	for (int i = 0; i < CollectableNumber; i++) {
		drawCollectable(CollectableX[i], CollectableY[i] , collectableRotationAngle);
	}
}
void drawTimePowerUp(float rotationAngle) {
	glColor3f(1, 0.5, 0);

	glPushMatrix();
	glTranslatef(TimePowerUpX, TimePowerUpY, 0.0);
	glRotatef(rotationAngle, 0.0, 0.0, 1.0); 
	drawCircle(0, 0, 20);

	glColor3f(0, 0, 0);
	drawCircle(0, 0, 15);

	glColor3f(1, 0.5, 0);
	glLineWidth(3.0);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, 10);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(10, 10);
	glEnd();

	glPopMatrix();
}

void drawScoreMultiplierPowerUp(float rotationAngle) {
	glColor3f(1, 0.5, 0);

	glPushMatrix();
	glTranslatef(ScoreMultiplierPowerUpX, ScoreMultiplierPowerUpY, 0.0);
	glRotatef(rotationAngle, 0.0, 0.0, 1.0); 
	drawCircle(0, 0, 20);

	glColor3f(0, 0, 0);
	drawCircle(0, 0, 15);

	glColor3f(1, 0.5, 0);
	glLineWidth(3.0);

	glBegin(GL_LINES);
	glVertex2f(-10, 0);
	glVertex2f(0, 10);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(10, 10);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(-10, 0);
	glVertex2f(0, -10);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(10, -10);
	glEnd();

	glPopMatrix();
}

void drawGoal(float rotationAngle) {
	glColor3f(0.0, 1.0, 0.0);

	// Apply the rotation transformation
	glPushMatrix();
	glTranslatef(800, 575, 0.0);
	glRotatef(rotationAngle, 0.0, 1.0, 0.0); // Rotate around the Y-axis

	glBegin(GL_QUADS);
	glVertex2f(-50, -25);
	glVertex2f(50, -25);
	glVertex2f(50, 25);
	glVertex2f(-50, 25);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex2f(-40, -20);
	glVertex2f(40, -20);
	glVertex2f(40, 20);
	glVertex2f(-40, 20);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(2.0);
	glBegin(GL_LINES);

	for (float y = -20; y <= 20; y += 5) {
		glVertex2f(-50, y);
		glVertex2f(50, y);
	}
	for (float x = -40; x <= 40; x += 5) {
		glVertex2f(x, -25);
		glVertex2f(x, 25);
	}

	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	drawCircle(0, 0, 15);

	glPopMatrix();
}

void drawPlayer(int x, int y) {
	glColor3f(1.0, 0.0, 1.0);

	// Apply the rotation transformation
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glRotatef(playerRotation, 0.0, 0.0, 1.0);

	drawCircle(0, 0, 15);  // First primitive

	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2.0);
	glBegin(GL_LINES);  // Second primitive
	glVertex2f(-15, 0);
	glVertex2f(15, 0);
	glEnd();

	glBegin(GL_TRIANGLES);  // Third primitive
	glVertex2f(0, 10);
	glVertex2f(-10, 1);
	glVertex2f(10, 1);
	glEnd();

	drawRect(-10, -10, 20, 5);  // Fourth primitive

	glPopMatrix();

	//glFlush();
}

void drawBackground(float rotationAngle) {
	glPushMatrix();
	glRotatef(rotationAngle, 0.0, 0.0, 1.0); 
	//glColor3f(1.0, 1.0, 1.0);
	glColor4f(0.7, 0.7, 0.7, 0.3); 
	drawCircle(600, 600, 500);
	drawRect(0, 0, 300, 300);
	glPopMatrix();
}

void Display() {

	glClear(GL_COLOR_BUFFER_BIT);

	if (HP <= 0 || countdownTime == 0) {
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);  
		//glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1, 1, 1);
		char* p0s[20];
		sprintf((char*)p0s, "GAME OVER");
		print(300, 350, (char*)p0s);
		char* p1s[20];
		sprintf((char*)p1s, "Ending Score: %d", score);
		print(300, 300, (char*)p1s);
		//glFlush();
	}
	else if (reachedGoal) {
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0, 0, 0);
		char* p0s[20];
		sprintf((char*)p0s, "YOU REACHED THE GOAL YOU WON!");
		print(300, 350, (char*)p0s);
		char* p1s[20];
		sprintf((char*)p1s, "Ending Score: %d", score);
		print(300, 300, (char*)p1s);
		//glFlush();
	}
	else {
		drawBackground(backgroundRotationAngle);
		//player
		drawPlayer(keyX, keyY);

		//obstacles
		spawnObstacles();

		//boundaries
		drawBordaries();

		drawHealthBar(HP);

		//collectables
		spawnCollectables();

		drawTimePowerUp(timePowerUpRotationAngle);

		drawScoreMultiplierPowerUp(scorePowerUpRotationAngle);

		drawGoal(goalRotationAngle);

		if (startPowerUpTimer) {
			glColor3f(1.0, 1.0, 1.0);
			int powerUpTime = powerUpCountDown; //10 seconds
			char* p1s[20];
			sprintf((char*)p1s, "Score x2 Time: %d ", powerUpTime);
			print(100, 550, (char*)p1s);
		}

		if (startFreezeTimerPowerUpTimer) {
			glColor3f(1.0, 1.0, 1.0);
			int freezeTime = freezeTimeCountDown;
			char* p1s[20];
			sprintf((char*)p1s, "Time is Frozen For: %d ", freezeTime);
			print(400, 675, (char*)p1s);
		}

		glColor3f(1, 1, 1);
		char* p0s[20];
		sprintf((char*)p0s, "Score: %d", score);
		print(650, 675, (char*)p0s);

		glBegin(GL_LINES);
		glVertex2f(0, 650);
		glVertex2f(900, 650);
		glEnd();


		char* p1s[20];
		sprintf((char*)p1s, "Time: %d ", countdownTime);
		print(750, 675, (char*)p1s);

		char* p2s[20];
		sprintf((char*)p2s, "HP: ");
		print(0, 670, (char*)p2s);


	}




	glFlush();
}

bool willNotPassThroughObstacle(int x, int y) {
	for (int i = 0; i <= ObstacleNumber; i++) {
		if ((keyX + x + 15 >= ObstacleX[i] && keyX + x + 15 <= ObstacleX[i] + 80 && keyY + y + 15 >= ObstacleY[i] && keyY + y + 15 <= ObstacleY[i] + 50)
			||
			(keyX + x - 15 >= ObstacleX[i] && keyX + x - 15 <= ObstacleX[i] + 80 && keyY + y - 15 >= ObstacleY[i] && keyY + y - 15 <= ObstacleY[i] + 50))
			return false;
	}
	return true;
}
void checkCollectableCollision(int scoreMultiplier) {
	for (int i = 0; i <= CollectableNumber; i++) {
		if ((keyX + 15 >= CollectableX[i]-15 && keyX + 15 <= CollectableX[i] + 15 && keyY + 15 >= CollectableY[i] - 15 && keyY + 15 <= CollectableY[i] + 15)
			||
			(keyX - 15 >= CollectableX[i] - 15 && keyX - 15 <= CollectableX[i] + 15 && keyY - 15 >= CollectableY[i] - 15 && keyY - 15 <= CollectableY[i] + 15)) {
			score = score + (1*scoreMultiplier);
			CollectableX[i] = -500;
			CollectableY[i] = -500;
			//bool played = PlaySound("collectable.wav", NULL , SND_ASYNC);
		}
	}
}

void checkFreezeTimePowerUpCollision() {
	if ((keyX + 15 >= TimePowerUpX- 20 && keyX + 15 <= TimePowerUpX + 20 && keyY + 15 >= TimePowerUpY- 20 && keyY + 15 <= TimePowerUpY + 20)
		||
		(keyX - 15 >= TimePowerUpX- 20 && keyX - 15 <= TimePowerUpX + 20 && keyY - 15 >= TimePowerUpY- 20 && keyY - 15 <= TimePowerUpY + 20)) {
		TimePowerUpX = -100;
		TimePowerUpY = -100;
		startFreezeTimerPowerUpTimer = true;

	}
}

void startScoreMultiplierPowerUpTimer() {
	glColor3f(1.0, 1.0, 1.0);
	int powerUpTime = 10; //10 seconds
	char* p1s[20];
	sprintf((char*)p1s, "PowerUp Time: %d ", powerUpTime);
	print(500, 675, (char*)p1s);
}


void checkScoreMultiplierPowerUpCollision() {
	if ((keyX + 15 >= ScoreMultiplierPowerUpX - 20 && keyX + 15 <= ScoreMultiplierPowerUpX + 20 && keyY + 15 >= ScoreMultiplierPowerUpY - 20 && keyY + 15 <= ScoreMultiplierPowerUpY + 20)
		||
		(keyX - 15 >= ScoreMultiplierPowerUpX - 20 && keyX - 15 <= ScoreMultiplierPowerUpX + 20 && keyY - 15 >= ScoreMultiplierPowerUpY - 20 && keyY - 15 <= ScoreMultiplierPowerUpY + 20)) {
		ScoreMultiplierPowerUpX = -100;
		ScoreMultiplierPowerUpY = -100;
		scoreMultiplier = 2;
		startPowerUpTimer = true;
	}
}

void checkGoalCollision() {
	if ((keyX + 15 >= 750 && keyX + 15 <= 850 && keyY + 15 >= 550 && keyY + 15 <= 600)
		||
		(keyX - 15 >= 750 && keyX - 15 <= 850 && keyY - 15 >= 550 && keyY - 15 <= 600)) {
		reachedGoal = true;
		//glutPostRedisplay();
	}
}

void key(unsigned char k, int x, int y)
{
	if (k == 'd') {
		if (keyX + 65 < 900 && willNotPassThroughObstacle(10, 0)) {
			keyX += 10;
			playerRotation = -90.0f;
		}

		else {
			HP--;
		}
	}
	if (k == 'a') {
		if (keyX - 65 >= 0 && willNotPassThroughObstacle(-10, 0)) {
			keyX -= 10;
			playerRotation = 90.0f;
		}

		else {
			HP--;
		}
	}
	if (k == 'w') {
		if (keyY + 115 < 700 && willNotPassThroughObstacle(0, 10)) {
			keyY += 10;
			playerRotation = 0.0f;
		}

		else {
			HP--;
		}
	}
	if (k == 's') {
		if (keyY - 65 >= 0 && willNotPassThroughObstacle(0, -10)) {
			keyY -= 10;
			playerRotation = 180.0f;
		}

		else {
			HP--;
		}
	}

	checkFreezeTimePowerUpCollision();
	checkCollectableCollision(scoreMultiplier);
	checkScoreMultiplierPowerUpCollision();
	checkGoalCollision();
	glutPostRedisplay();
}


void timer(int val)
{
	if (countdownTime != 0 && !startFreezeTimerPowerUpTimer)
		countdownTime--;
	else {
		freezeTimeCountDown--;
		if (freezeTimeCountDown == 0)
			startFreezeTimerPowerUpTimer = false;
	}
	if (startPowerUpTimer && powerUpCountDown !=0) {
		powerUpCountDown--;
	}
	else {
		startPowerUpTimer = false;
		scoreMultiplier = 1;
	}
	glutPostRedisplay();
	glutTimerFunc(1000, timer, 0);
}

void collectableAnimation(int val) {
	collectableRotationAngle += 5.0f;
	glutPostRedisplay();
	glutTimerFunc(30, collectableAnimation, 0);
}

void timePowerUpAnimation(int val) {
	timePowerUpRotationAngle += 5.0f;
	glutPostRedisplay();
	glutTimerFunc(30, timePowerUpAnimation, 0);
}

void scorePowerUpAnimation(int val) {
	scorePowerUpRotationAngle += 5.0f;
	glutPostRedisplay();
	glutTimerFunc(30, scorePowerUpAnimation, 0);
}

void goalAnimation(int val) {
	goalRotationAngle += 5.0f;
	glutPostRedisplay();
	glutTimerFunc(100, goalAnimation, 0);
}

void backgroundAnimation(int val) {
	backgroundRotationAngle += 5.0f;
	glutPostRedisplay();
	glutTimerFunc(1000, backgroundAnimation, 0);
}
void Animation() {
	collectableRotationAngle += 1.0f;
	timePowerUpRotationAngle += 1.0f;
	scorePowerUpRotationAngle += 1.0f;
	goalRotationAngle += 1.0f;
	backgroundRotationAngle += 1.0f;
	glutPostRedisplay();
}

bool isInArray(int array[], int size, int value) {
	for (int i = 0; i < size; i++) {
		if (array[i] == value) {
			return true; 
		}
	}
	return false; 
}


void main(int argc, char** argr) {
	glutInit(&argc, argr);
	srand(time(NULL));
	//bool played = PlaySound("game-music.wav", NULL, SND_ASYNC);
	for (int i = 0; i < CollectableNumber; i++) {
		CollectableX[i] = (rand() % 70 + 10) * 10;
	}

	for (int i = 0; i < CollectableNumber; i++) {
		CollectableY[i] = (rand() % 50 + 10) * 10;
	}

	ObstacleX[0] = 200;
	ObstacleY[0] = 400;

	ObstacleX[1] = 600;
	ObstacleY[1] = 400;

	ObstacleX[2] = 300;
	ObstacleY[2] = 100;

	ObstacleX[3] = 550;
	ObstacleY[3] = 100;

	ObstacleX[4] = 500;
	ObstacleY[4] = 500;

	glutInitWindowSize(900, 700);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("2DGame");
	glutDisplayFunc(Display);
	glutKeyboardFunc(key);
	glutTimerFunc(0, timer, 0);
	//glutTimerFunc(0, collectableAnimation, 0);
	//glutTimerFunc(0, scorePowerUpAnimation, 0);
	//glutTimerFunc(0, timePowerUpAnimation, 0);
	//glutTimerFunc(0, goalAnimation, 0);
	//glutTimerFunc(0, backgroundAnimation, 0	);
	glutIdleFunc(Animation);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, 900, 0.0, 700);



	//BONUS

	glutMainLoop();
}

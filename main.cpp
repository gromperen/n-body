#include <bits/stdc++.h>
#include <GL/glut.h>

using namespace std;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define DRAW_MODE 1
// 1 = Pixel
// 2 = draw circle

#define DRAG 1
// bool 1/0
//

struct Particle {
	float x;
	float y;
	float r;
	float vx;
	float vy;
	float m;
};

struct Line {
    float x1;
    float y1;
    float x2;
    float y2;
} line;


int WIN;

int mouse_x, mouse_y;

bool PRESSED_LEFT = false, PRESSED_RIGHT = false;



float minDistance = 0.01;
float gravityStrength = 0.05;
float simulationSpeed = 0.002;

vector<Particle> particles;

void add_particle(float m, float x, float y, float vx = 0, float vy = 0) {
	Particle p;
	p.x = x;
	p.y = y;
	p.m = m;
	p.vx = vx;
	p.vy = vy;
	p.r = min(15.0d, m/M_PI);
	particles.push_back(p);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	if (DRAW_MODE == 1) {
		glBegin(GL_POINTS);
		for (Particle &p : particles)  {
			for (float i = 0; i < 2*3.14159; i+=0.1)
				glVertex2i(p.x, p.y);

		}
		glEnd();
	} else if (DRAW_MODE == 2) {
		for (Particle &p : particles)  {
			glBegin(GL_POLYGON);
			for (float i = 0; i < 2*3.14159; i+=0.1)
				glVertex2f(p.r * cos(i) + p.x, p.r*sin(i) + p.y);

			glEnd();
			//glVertex2i(p.x, p.y);
		}
	}

	glFlush();
	glutSwapBuffers();
}

void timer(int = 0) {
	display();

	for (int i = 0; i < particles.size(); ++i) {
		Particle &pi = particles[i];
		for (int j = i+1; j < particles.size(); ++j) {
			Particle &pj = particles[j];

			float d = sqrt((pi.x-pj.x)*(pi.x-pj.x) + (pi.y-pj.y)*(pi.y-pj.y));
			float force = min(minDistance, gravityStrength * pi.m * pj.m / (d*d)) * simulationSpeed;
			//if (pi.r + pj.r >= d) continue;
			// apply force and normalize
			pi.vx +=  force / pi.m * (pj.x - pi.x) / d;
			pj.vx +=  force / pj.m * (pi.x - pj.x) / d;
			pi.vy +=  force / pi.m * (pj.y - pi.y) / d;
			pj.vy +=  force / pj.m * (pi.y - pj.y) / d;
		}

		pi.x += pi.vx;
		pi.y += pi.vy;
	}

	glutTimerFunc(1, timer, 0);
}



void universe_circle(float radius = 100) {
	for (float i = 0; i < 2*3.14159; i+=0.01) {
		add_particle(10, SCREEN_WIDTH/2 + 100 * cos(i), SCREEN_WIDTH/2 + 100 * sin(i));
	}
}

void random_universe(int n = 1000) {

	for (int i = 0; i < n; ++i) {
		add_particle(10, (rand() % SCREEN_WIDTH), (rand() % SCREEN_HEIGHT));
	}
}

void mouse(int button, int state, int x, int y){
	mouse_x = x;
	mouse_y = y;
	if(DRAG){
		if(line.x2 != -1 && line.y2 != -1 && state == GLUT_UP && PRESSED_LEFT) {
			add_particle(10, line.x1, line.y1, line.x1 - line.x2, line.y1 - line.y2);
			line.x2 = line.y2 = -1;
		} else if (state == GLUT_UP && PRESSED_LEFT) {
			add_particle(10, mouse_x, mouse_y);
		} else{
			line.x1 = line.x2 = mouse_x;
			line.y1 = line.y2 = mouse_y;
		}

	} else {
		if (state == GLUT_UP && PRESSED_LEFT) {
			add_particle(10, mouse_x, mouse_y);
		}
	}

	if(button == GLUT_LEFT_BUTTON)
		PRESSED_LEFT = (state == GLUT_DOWN);
	else if(button == GLUT_RIGHT_BUTTON)
		PRESSED_RIGHT = (state == GLUT_DOWN);
}

void mouseMotion(int x, int y){
	mouse_x = x;
	mouse_y = y;
	if(DRAG && PRESSED_LEFT){
		line.x2 = mouse_x;
		line.y2 = mouse_y;
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	WIN = glutCreateWindow("super cool sim");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1);

	glutDisplayFunc(display);

	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	//universe_circle(50);
	universe_circle(100);
	//universe_circle(200);
	//universe_circle(200);

	timer();

	glutMainLoop();

	return 0;
}

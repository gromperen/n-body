#include <bits/stdc++.h>
#include <GL/glut.h>

using namespace std;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

struct Particle {
	float x;
	float y;
	float r;
	float vx;
	float vy;
	float m;
};


int WIN;


float minDistance = 10;
float gravityStrength = 0.05;
float simulationSpeed = 0.01;

vector<Particle> particles;

void add_particle(float m, float x, float y) {
	Particle p;
	p.x = x;
	p.y = y;
	p.m = m;
	p.r = min(15.0d, m/M_PI);
	particles.push_back(p);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

		//glBegin(GL_POINTS);
	for (Particle &p : particles)  {
		glBegin(GL_POLYGON);
		for (float i = 0; i < 2*3.14159; i+=0.1)
			glVertex2f(p.r * cos(i) + p.x, p.r*sin(i) + p.y);

		glEnd();
		//glVertex2i(p.x, p.y);
	}
		glEnd();

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
			if (pi.r + pj.r >= d) continue;
			// apply force and normalize
			pi.vx += simulationSpeed *  pj.m / (d*d) * (pj.x - pi.x) / d;
			pj.vx += simulationSpeed * pi.m  / (d*d) * (pi.x - pj.x) / d;
			pi.vy += simulationSpeed *  pj.m / (d*d) * (pj.y - pi.y) / d;
			pj.vy += simulationSpeed * pi.m  / (d*d) * (pi.y - pj.y) / d;
		}

		pi.x += pi.vx;
		pi.y += pi.vy;
	}

	glutTimerFunc(1, timer, 0);
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



	//))add_particle(10, -10, -10);
	//add_particle(15000, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	//for (int x = 1; x < SCREEN_WIDTH; x += 300)
		//for (int y = 1; y < SCREEN_HEIGHT; y += 300)
			//add_particle(10, x, y);

	///*
	//for (int i = 0; i < 1000; ++i) {
		//add_particle(10, (rand() % SCREEN_WIDTH), (rand() % SCREEN_HEIGHT));
	//}

	float radius = 100;
	for (float i = 0; i < 2*3.14159; i+=0.01) {
		add_particle(10, SCREEN_WIDTH/2 + 100 * cos(i), SCREEN_WIDTH/2 + 100 * sin(i));
	}
	// add_particle(10, 0, 0);
	// add_particle(10, 50, 50);
	// add_particle(10, 100, 50);

	timer();

	glutMainLoop();

	return 0;
}

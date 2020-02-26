// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.

#include <cmath>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include "../pglut.h"

#include <time.h> 
// Global Variables (Only what you need!)
double screen_x = 1800;
double screen_y = 1400;
std::vector<std::vector<int>> collisions;
std::vector<double> xpos;
std::vector<double> ypos;
std::vector<double> dx;
std::vector<double> dy;
std::vector<double> radius;
std::vector<double> colorr;
std::vector<double> colorg;
std::vector<double> colorb;
int draggingcircle = -1;
int mousex, mousey;
const double COLLISION_FRICTION = .98;
// 
// Functions that draw basic primitives
//
void DrawCircle(double x1, double y1, double radius)
{
	glBegin(GL_POLYGON);
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1,y1);
	glVertex2d(x2,y1);
	glVertex2d(x2,y2);
	glVertex2d(x1,y2);
	glEnd();
}

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1,y1);
	glVertex2d(x2,y2);
	glVertex2d(x3,y3);
	glEnd();
}

// Outputs a string of text at the specified location.
void DrawText(double x, double y, const char *string)
{
	void *font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
	
	int len, i;
	glRasterPos2d(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++) 
	{
		glutBitmapCharacter(font, string[i]);
	}

    glDisable(GL_BLEND);
}


//
// GLUT callback functions
//
struct vectortype
{
	double x;
	double y;
};

void collide(double& nx1,double& ny1,double& r1,double& dx1,double& dy1,double& nx2,double& ny2,double& r2,double& dx2,double& dy2)
{
	vectortype en; // Center of Mass coordinate system, normal component
	vectortype et; // Center of Mass coordinate system, tangential component
	vectortype u[2]; // initial velocities of two particles
	vectortype um[2]; // initial velocities in Center of Mass coordinates
	double umt[2]; // initial velocities in Center of Mass coordinates, tangent component
	double umn[2]; // initial velocities in Center of Mass coordinates, normal component
	vectortype v[2]; // final velocities of two particles
	double m[2];	// mass of two particles
	double M; // mass of two particles together
	vectortype V; // velocity of two particles together
	double size;
	int i;

	double xdif =nx1-nx2;
	double ydif = ny1-ny2;

	// set Center of Mass coordinate system
	size = sqrt(xdif * xdif + ydif * ydif);
	xdif /= size; ydif /= size; // normalize
	en.x = xdif;
	en.y = ydif;
	et.x = ydif;
	et.y = -xdif;

	// set u values
	u[0].x = dx1;
	u[0].y = dy1;
	m[0] = r1*r1;
	u[1].x = dx2;
	u[1].y = dy2;
	m[1] = r2 * r2;

	// set M and V
	M = m[0] + m[1];
	V.x = (u[0].x * m[0] + u[1].x * m[1]) / M;
	V.y = (u[0].y * m[0] + u[1].y * m[1]) / M;

	// set um values
	um[0].x = m[1] / M * (u[0].x - u[1].x);
	um[0].y = m[1] / M * (u[0].y - u[1].y);
	um[1].x = m[0] / M * (u[1].x - u[0].x);
	um[1].y = m[0] / M * (u[1].y - u[0].y);

	// set umt and umn values
	for (i = 0;i < 2;i++)
	{
		umt[i] = um[i].x * et.x + um[i].y * et.y;
		umn[i] = um[i].x * en.x + um[i].y * en.y;
	}

	// set v values
	for (i = 0;i < 2;i++)
	{
		v[i].x = umt[i] * et.x - umn[i] * en.x + V.x;
		v[i].y = umt[i] * et.y - umn[i] * en.y + V.y;
	}

	// reset particle values
	dx1=(v[0].x * COLLISION_FRICTION);
	dy1=(v[0].y * COLLISION_FRICTION);
	dx2=(v[1].x * COLLISION_FRICTION);
	dy2=(v[1].y * COLLISION_FRICTION);

} /* Collide */
// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (draggingcircle >= 0) {
		const double invstrength = 30;
		dx[draggingcircle] += (mousex - xpos[draggingcircle]) / invstrength;
		dy[draggingcircle] += (screen_y - mousey - ypos[draggingcircle]) / invstrength;
	}
	// Test lines that draw all three shapes and some text.
	// Delete these when you get your code working.
	for (int i = 0;i < xpos.size();i++) {
		collisions[i].clear();
		dy[i] -= .01;
		//dx[i] *= .9999;
		//dy[i] *= .9999;
	}
	for (int i = 0;i < xpos.size();i++) {
		for (int j = i + 1;j < xpos.size();j++) {
			double nx1 = xpos[i] + dx[i];
			double ny1 = ypos[i] + dy[i];
			double nx2 = xpos[j] + dx[j];
			double ny2 = ypos[j] + dy[j];
			double xdiff = nx1 - nx2;
			double ydiff = ny1 - ny2;
			double sumrad = radius[i] + radius[j];
			if (xdiff * xdiff + ydiff * ydiff < sumrad * sumrad) {
				collisions[i].push_back(j);
				
			}
		}
	}
	for (int i=0;i<xpos.size();i++){
		if (collisions[i].size() >= 1) {
			for (int c = 0;c < collisions[i].size();c++) {
				int j = collisions[i][c];
				double nx1 = xpos[i] + dx[i];
				double ny1 = ypos[i] + dy[i];
				double nx2 = xpos[j] + dx[j];
				double ny2 = ypos[j] + dy[j];
				double tr=colorr[i];
				double tg=colorg[i];
				double tb=colorb[i];
				colorr[i]=colorr[j];
				colorg[i]=colorg[j];
				colorb[i]=colorb[j];
				colorr[j]=tr;
				colorg[j]=tg;
				colorb[j]=tb;

				collide(nx1, ny1, radius[i], dx[i], dy[i], nx2, ny2, radius[j], dx[j], dy[j]);
				//xpos[i] -= dx[i];
				//ypos[i] -= dy[i];
			}
			
		}
		else {
			for (int c=0;c < collisions[i].size();c++) {
				dx[collisions[i][c]] = 0;
				dy[collisions[i][c]] = 0;
			}
		}
		
		
		
		if (xpos[i] + dx[i] + radius[i] >= screen_x || xpos[i] + dx[i] - radius[i] < 0) {
			dx[i] = -dx[i];
		}
		if (ypos[i] + dy[i] + radius[i] >= screen_y || ypos[i] + dy[i] - radius[i] < 0) {
			dy[i] = -dy[i];
		}
		xpos[i] += dx[i];
		ypos[i] += dy[i];
		glColor3d(colorr[i],colorg[i],colorb[i]);
		DrawCircle(xpos[i], ypos[i], radius[i]);
	}
	glutSwapBuffers();
	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		case 'b':
			// do something when 'b' character is hit.
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}


// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse_move( int x, int y) {
	mousex = x;
	mousey = y;
	
}
void mouse(int mouse_button, int state, int x, int y)
{
	mousex = x;
	mousey = y;
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		bool colliding = false;
		draggingcircle = -1;
		for (int i = 0;i < xpos.size();i++) {
			double xdiff = xpos[i] - x;
			double ydiff = ypos[i] - (screen_y-y);
			if (xdiff * xdiff + ydiff * ydiff < radius[i] * radius[i]) {
				draggingcircle = i;
				colliding = true;
			}
		}
		if (!colliding) {
			int i = xpos.size();
			dx.push_back(0);
			dy.push_back(0);
			radius.push_back(0);
			colorr.push_back(0);
			colorg.push_back(0);
			colorb.push_back(0);
			xpos.push_back(0);
			ypos.push_back(0);
			collisions.push_back(std::vector< int >());

			dx[i] = (std::rand() % 100 - 50) / 50.0;
			dy[i] = (rand() % 100 - 50) / 50.0;
			radius[i] = rand() % 25 + 25;
			colorr[i] = (rand() % 206 + 50) / 256.0;
			colorg[i] = (rand() % 206 + 50) / 256.0;
			colorb[i] = (rand() % 206 + 50) / 256.0;
			xpos[i] = x;
			ypos[i] = screen_y - y;
		}
		
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
		draggingcircle = -1;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
}


int main(int argc, char **argv)
{
	srand(time(NULL));
	for (int i = 0;i < 5;i++) {
		dx.push_back(0);
		dy.push_back(0);
		radius.push_back(0);
		colorr.push_back(0);
		colorg.push_back(0);
		colorb.push_back(0);
		xpos.push_back(0);
		ypos.push_back(0);
		collisions.push_back(std::vector< int >());
		
		dx[i] = (rand() % 100 - 50)/50.0;
		dy[i] = (rand() % 100 - 50)/50.0;
		radius[i] = rand() % 25 + 25;
		colorr[i] = (rand() % 206 + 50)/256.0;
		colorg[i] = (rand() % 206 + 50)/256.0;
		colorb[i] = (rand() % 206 + 50)/256.0;
		xpos[i] = rand() % (int)(screen_x-radius[i]*2)+radius[i];
		ypos[i] = rand() % (int)(screen_y - radius[i] * 2)+radius[i];

	}

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("Bouncing Circles");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_move);

	glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}

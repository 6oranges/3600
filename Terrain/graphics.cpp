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
#include "graphics.h"
#include "terrain.h"
#include "../pglut.h"
#include "glider.h"
#include <chrono>
// Global Variables (Only what you need!)
double screen_x = 700;
double screen_y = 500;
struct Input
{
	bool forward=false,left=false,right=false;
};
viewtype current_view = perspective_view;

Glider gGlider;
Input gInput;
double gTheta=45*3.141592653589793238/180;
double timet=0;
double max(double x,double y){
	if (x>y){
		return x;
	}
	else{
		return y;
	}
}
double myFunction(double x, double y){
	x=x/10;
	y=y/10;
	double a= sin(x/10)*cos(y/15)*10+.79045*sin((x)/(.653)+.3523)+.6543*sin((y)/(.853)+.5623)+.5441*sin((x)/(.553)+.9897)+.8443*sin((y)/(.553)+.8923)+.7*sin(x+.436)*sin(y)+.1*sin(x)*sin(y);
	return 3*a;
	//return 2*sin(0.4*y)+0.75+1.5*cos(0.3*x)+4*sin(0.2*x)*cos(0.3*y)+6*sin(0.11*x)*cos(0.03*y);
	//return y*x+5*x+3*y+x*x;
}
double mySand(double x, double y, double z){
	x=x/5;
	y=y/5;
	return (sin(x/10)*cos(y/10)*sin(x+y/3)*cos(.2+x/7+y)+1)/2;
}
double myRed(double x, double y, double z){
	return (1-((z-3)/16+.5))/2+mySand(x,y,z)/2;//(sin(x/2)*cos(y)*sin(x*2+.2)+1)/2;
}
double myGreen(double x, double y, double z){
	return -max(-((z-3)/16+.75),-1)-mySand(x,y,z)/4*2;
}
double myBlue(double x, double y, double z){
	return 0;
}
double myOpaque(double x, double y, double z){
	return 1;
}
double wFunction(double x, double y){
	x=x/5;
	y=y/5;
	return sin(gTheta*3)+.79045*sin((x+gTheta*4)/(.653)+.3523)+.6543*sin((y+gTheta*8)/(.853)+.5623)+.5441*sin((x)/(.553)+.9897)+.8443*sin((y)/(.553)+.8923)+.7*sin(x+.436)*sin(y)+.1*sin(x)*sin(y)+3;
	//return 2*sin(0.4*y)+0.75+1.5*cos(0.3*x)+4*sin(0.2*x)*cos(0.3*y)+6*sin(0.11*x)*cos(0.03*y);
}
double wRed(double x, double y, double z){
	return (z-3)/16+.5+sin(x+6*gTheta)*cos(y+9*gTheta);
}
double wGreen(double x, double y, double z){
	return wRed(x,y,z);
}
double wBlue(double x, double y, double z){
	return 1;
}
double wAlpha(double x, double y, double z){
	return .5;
}
Terrain gTerrain(myFunction,myRed,myGreen,myBlue,myOpaque);
Terrain gWater(wFunction,wRed,wGreen,wBlue,wAlpha);
double WIDTH=300,HEIGHT=300;
// 
// Functions that draw basic primitives
//
void DrawCircle(double x1, double y1, double radius)
{
	glBegin(GL_TRIANGLES);
	double px,py;
	px=x1+radius;
	py=y1;
	for(int i=0; i<32; i++)
	{
		double theta = (double)i/32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(px, py);
		glVertex2d(x, y);
		
		glVertex2d(x1, y1);
		px=x;
		py=y;
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

void DrawLine(double x1, double y1, double x2, double y2)
{
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
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

double getDeltaTime() {
	using namespace std::chrono;
	static steady_clock::time_point clock_begin = steady_clock::now();
	steady_clock::time_point clock_end = steady_clock::now();

	steady_clock::duration time_span = clock_end - clock_begin;

	double dt = double(time_span.count()) * steady_clock::period::num / steady_clock::period::den;
	clock_begin = clock_end;
	return dt;
}
double height(){
	return sin(gTheta*3)*4;
}
//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.

void display(void)
{
	glClearColor(0,.7,.9,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	double h = height();
	if(current_view == perspective_view)
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		double cx=gGlider.GetX();
		double cy=gGlider.GetY();
		gluLookAt(cx+WIDTH*cos(gTheta),cy+HEIGHT*sin(gTheta),gTerrain.GetZ(cx,cy)+150,  cx,cy,gTerrain.GetZ(cx,cy),  0,0,1);
	}
	else if(current_view == top_view)
	{
		glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
	}
	else // current_view == rat_view
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		
		double x = gGlider.GetX();
		double y = gGlider.GetY();
		double z_level = max(gTerrain.GetZ(x,y),gWater.GetZ(x,y))+1;
		double dx = gGlider.GetDX()*2;
		double dy = gGlider.GetDY()*2;
		double at_x = x + dx;
		double at_y = y + dy;
		double at_z =max(gTerrain.GetZ(at_x,at_y),gWater.GetZ(at_x,at_y))+1-.1;
		gluLookAt(x,y,z_level,  at_x, at_y, at_z,  0,0,1);
	}
	
	double dt=getDeltaTime();
	gTheta+=dt/5;
	if (gInput.left){
		gGlider.TurnLeft(dt);
	}
	if (gInput.right){
		gGlider.TurnRight(dt);
	}
	if (gInput.forward){
		gGlider.ScurryForward(dt);
	}
	glColor3ub(100,100,255);
	glColor3d(0, 0, 0);
	int x=gGlider.GetX();
	int y=gGlider.GetY();
	
	const double s=300;//Size of single segment
	const double d=5; // root of segments
	const double c =0;//s/d; // offset
	const double r=50; // triangles in center segment
	//gTerrain.DrawDecending(x-50-c,y-50-c,x+50-c,y+50-c,50,50,10,10);
	gTerrain.DrawDecending(x-s-c,y-s-c,x+s-c,y+s-c,r,r,d,d);
	//gTerrain.DrawUniform(x-100-c,y-100-c,x+100-c,y+100-c,100,100);
	glColor3d(0, 1, .5);
	gGlider.Draw();

	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4d(.1,.5,1,.5);
	gWater.DrawDecending(x-s-c,y-s-c,x+s-c,y+s-c,r,r,d,d);
	//gWater.DrawUniform(x-100-c,y-100-c,x+100-c,y+100-c,100,100);
	
	/*glBegin(GL_QUADS);
	glVertex3d(x-100,y-100,h);
	glVertex3d(x+100,y-100,h);
	glVertex3d(x+100,y+100,h);
	glVertex3d(x-100,y+100,h);

	glEnd();
*/
	glutSwapBuffers();
	glutPostRedisplay();
}

void SetTopView(int w, int h)
{
	// go into 2D mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double world_margin_x = 0.5;
	double world_margin_y = 0.5;
	gluOrtho2D(-world_margin_x, WIDTH+world_margin_x, 
		-world_margin_y, HEIGHT+world_margin_y);
	glMatrixMode(GL_MODELVIEW);
}

void SetPerspectiveView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble) w/(GLdouble) h;
	gluPerspective( 
	/* field of view in degree */ 38.0,
	/* aspect ratio */ aspectRatio,
	/* Z near */ .1, /* Z far */ 500.0);
	glMatrixMode(GL_MODELVIEW);
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
		case 'w':
			gInput.forward=true;
			break;
		case 'a':
			gInput.left=true;
			break;
		case 'd':
			gInput.right=true;
			break;
		case 'r':
			current_view = rat_view;
			SetPerspectiveView(screen_x, screen_y);
			break;
		case 'p':
			current_view = perspective_view;
			SetPerspectiveView(screen_x, screen_y);
			break;
		case 't':
			current_view = top_view;
			SetTopView(screen_x, screen_y);
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}
void keyboardup(unsigned char c, int x, int y)
{
	switch (c) 
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		case 'b':
			// do something when 'b' character is hit.
			break;
		case 'w':
			gInput.forward=false;
			break;
		case 'a':
			gInput.left=false;
			break;
		case 'd':
			gInput.right=false;
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
	screen_x = w;
	screen_y = h;
	glViewport(0, 0, w, h);

	if(current_view == top_view)
	{
		SetTopView(w,h);
	}
	else if(current_view == perspective_view)
	{
		SetPerspectiveView(w,h);
	}
	else // current_view == rat_view
	{
		SetPerspectiveView(w,h);
	}

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		gInput.left=true;
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
		gInput.left=false;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
		gInput.forward=true;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
		gInput.forward=false;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
	{
		gInput.right=true;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_UP) 
	{
		gInput.right=false;
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	srand(time(NULL));
	gGlider=Glider(.3,90,10,90);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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
		glutCreateWindow("Maze of Doom");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardup);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}

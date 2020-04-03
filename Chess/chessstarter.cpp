// Timothy Stander
// Chess animation starter kit.

#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include "../pglut.h"
#include "objloader.h"
#include "slider.h"
// Global Variables
// Some colors you can use, or make your own and add them
// here and in graphics.h
GLdouble redMaterial[] = {0.7, 0.1, 0.2, 1.0};
GLdouble greenMaterial[] = {0.1, 0.7, 0.4, 1.0};
GLdouble brightGreenMaterial[] = {0.1, 0.9, 0.1, 1.0};
GLdouble blueMaterial[] = {0.1, 0.2, 0.7, 1.0};
GLdouble whiteMaterial[] = {1.0, 1.0, 1.0, 1.0};

double screen_x = 600;
double screen_y = 500;
int gActiveSlider;
std::vector<Slider> gSliders;

double gElapsedTime=0;
double gTimeFlow=1;

double GetDeltaTime()
{
	static auto t_prev = std::chrono::high_resolution_clock::now();
	auto t_end = std::chrono::high_resolution_clock::now();
	double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_prev).count()/1000;
	t_prev=t_end;
	return elapsed_time_ms;
}

// NOTE: Y is the UP direction for the chess pieces.
double eye[3] = {4.5, 8, -4}; // pick a nice vantage point.
double at[3]  = {4.5, 0,     4};
//
// GLUT callback functions
//

// As t goes from t0 to t1, set v between v0 and v1 accordingly.
void Interpolate(double t, double t0, double t1,
	double & v, double v0, double v1)
{
	double ratio = (t - t0) / (t1 - t0);
	if (ratio < 0)
		ratio = 0;
	if (ratio > 1)
		ratio = 1;
	v = v0 + (v1 - v0)*ratio;
}
void Board(){
	glBegin(GL_QUADS);
	glNormal3d(0,1,0);
	for (int i=0;i<8;i++){
		for (int j=0;j<8;j++){
			if((i+j)%2==0){
				GLfloat mat_amb_diff1[] = {0.7f, 0.8f, 0.4f, 1.0f};
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);
			}
			else{
				GLfloat mat_amb_diff1[] = {0, 0.4f, 0.7f, 0.1};
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);
			}
			glVertex3d(i,0,j+1);
			glVertex3d(i+1,0,j+1);
			glVertex3d(i+1,0,j);
			glVertex3d(i,0,j);
		}
	}
	glNormal3d(0,0,-1);
	glVertex3d(0,0,0);
	glVertex3d(8,0,0);
	glVertex3d(8,-1,0);
	glVertex3d(0,-1,0);

	glNormal3d(-1,0,0);
	glVertex3d(0,-1,0);
	glVertex3d(0,-1,8);
	glVertex3d(0,0,8);
	glVertex3d(0,0,0);

	glNormal3d(0,0,1);
	glVertex3d(8,0,8);
	glVertex3d(0,0,8);
	glVertex3d(0,-1,8);
	glVertex3d(8,-1,8);

	glNormal3d(1,0,0);
	glVertex3d(8,-1,8);
	glVertex3d(8,-1,0);
	glVertex3d(8,0,0);
	glVertex3d(8,0,8);
	glEnd();
}
OBJModel gKingModel;
OBJModel gKnightModel;
OBJModel gPawnModel;
OBJModel gQueenModel;
OBJModel gRookModel;
OBJModel gBishopModel;
OBJModel gBoardModel;
enum piece_numbers{pawn=100,king,queen,rook,bishop,knight,board};
void initPieces(){
	std::ifstream gf("King.obj");
	gKingModel = OBJModel(gf);
	std::ifstream kf("Knight.obj");
	gKnightModel = OBJModel(kf);
	std::ifstream pf("Pawn.obj");
	gPawnModel = OBJModel(pf);
	std::ifstream qf("Queen.obj");
	gQueenModel = OBJModel(qf);
	std::ifstream rf("Rook.obj");
	gRookModel = OBJModel(rf);
	std::ifstream bf("Bishop.obj");
	gBishopModel = OBJModel(bf);
	

	// Make the display lists for speed
	glNewList(king,GL_COMPILE);
	gKingModel.draw();
	glEndList();
	glNewList(pawn,GL_COMPILE);
	gPawnModel.draw();
	glEndList();
	glNewList(queen,GL_COMPILE);
	gQueenModel.draw();
	glEndList();
	glNewList(rook,GL_COMPILE);
	gRookModel.draw();
	glEndList();
	glNewList(bishop,GL_COMPILE);
	gBishopModel.draw();
	glEndList();
	glNewList(knight,GL_COMPILE);
	gKnightModel.draw();
	glEndList();
	glNewList(board,GL_COMPILE);
	glPushMatrix();
	glTranslated(.5,0,.5);
	Board();
	glPopMatrix();
	glEndList();
	
	gSliders.push_back(Slider(gTimeFlow,-10,10,15,15,200,15));
	gSliders.push_back(Slider(gElapsedTime,0,60,15,60,200,15));
	
}
void SetPerspectiveView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble) screen_x/(GLdouble) screen_y;
	gluPerspective( 
	/* field of view in degree */ 45.0,
	/* aspect ratio */ aspectRatio,
	/* Z near */ .1, /* Z far */ 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void SetTopView()
{
	// go into 2D mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,screen_x,0,screen_y);
	glViewport(0,0,screen_x,screen_y);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	gElapsedTime += GetDeltaTime()*gTimeFlow;
	double t = gElapsedTime;
	glEnable(GL_NORMALIZE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2],  at[0], at[1], at[2],  0,1,0); // Y is up!

	// Set the color for one side (white), and draw its 16 pieces.
	GLfloat mat_amb_diff1[] = {0.8f, 0.9f, 0.5f, 1.0f};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff1);
	const double pi = 3.141592653589793238;
	eye[0]=cos(t/4)*10+4;
	eye[2]=sin(t/4)*10+4;
	
	glCallList(board);
	
	if(t<51){
		glPushMatrix();
		glTranslatef(3, 0, 1);
		glRotated(t*64,0,1,0);
		glCallList(bishop);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(2, 0, 1);
		glRotated(t*64,0,1,0);
		glCallList(knight);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(7, 0, 1);
		glRotated(t*64,0,1,0);
		glCallList(knight);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(4, 0, 1);
		glRotated(t*32,0,1,0);
		glCallList(king);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(1, 0, 1);
		glRotated(t*64,0,1,0);
		glCallList(rook);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(8, 0, 1);
		glRotated(t*64,0,1,0);
		glCallList(rook);
		glPopMatrix();

		//Interpolate(t, 1.0, 3.0, z, 1, 7);
		glPushMatrix();
		glTranslatef(5, 0, 1);
		glRotated(t*16,0,1,0);
		glCallList(queen);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(6, 0, 1);
		glRotated(t*48,0,1,0);
		glCallList(bishop);
		glPopMatrix();
	}
	
	double z;
	for(int x=1; x<=8; x+=1)
	{
		Interpolate(t, x*2, x*2+1, z, 2,3);
		double j;
		Interpolate(t, x*2-1+17, x*2+17, j, 0,1);
		double k;
		Interpolate(t, x*2-1+33, x*2+33, k, 0,1);
		double l;
		Interpolate(t, 48, 49, l, 0,1);
		double m;
		Interpolate(t, 49, 51, m, 0,2);
		double ao;
		Interpolate(t, 54, 200, ao, 0,50);
		double a = (x/8.0)*pi+ao;
		double n1;
		Interpolate(t, 51, 54, n1, x+k-l,cos(a)*3+4);
		double n2;
		Interpolate(t, 51, 54, n2, z+j+k+l+m,sin(a)*3+4);
		glPushMatrix();
		glTranslatef(n1, 0, n2);
		glCallList(pawn);
		glPopMatrix();
	}

	// Set the color for one side (black), and draw its 16 pieces.
	GLfloat mat_amb_diff2[] = {0.1f, 0.5f, 0.8f, 0.1};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff2);

	//Interpolate(t, 4.0, 6.0, z, 4, 2);
	if (t<51){
		glPushMatrix();
		glTranslatef(4, 0, 8);
		glRotated(t*32,0,1,0);
		glCallList(king);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(2, 0, 8);
		glRotated(t*64,0,1,0);
		glCallList(knight);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(7, 0, 8);
		glRotated(t*64,0,1,0);
		glCallList(knight);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(3, 0, 8);
		glRotated(t*64,0,1,0);
		glCallList(bishop);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(5, 0, 8);
		glRotated(t*16,0,1,0);
		glCallList(queen);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(6, 0, 8);
		glRotated(t*64,0,1,0);
		glCallList(bishop);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(1, 0, 8);
		glRotated(t*64,0,1,0);
		glCallList(rook);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(8, 0, 8);
		glRotated(t*64,0,1,0);
		glCallList(rook);
		glPopMatrix();
	}
	

	for(int x=1; x<=8; x+=1)
	{
		Interpolate(t, x*2-1, x*2, z, 7,6);
		double j;
		Interpolate(t, x*2-1+16, x*2+16, j, 0,-1);
		double k;
		Interpolate(t, x*2-1+32, x*2+32, k, 0,-1);
		double l;
		Interpolate(t, 48, 49, l, 0,-1);
		double m;
		Interpolate(t, 49, 51, m, 0,-2);
		
		double ao;
		Interpolate(t, 54, 200, ao, 0,50);
		double a = -(x/8.0)*pi+ao;
		double n1;
		Interpolate(t, 51, 54, n1, x+k-l,cos(a)*3+4);
		double n2;
		Interpolate(t, 51, 54, n2, z+j+k+l+m,sin(a)*3+4);
		glPushMatrix();
		glTranslatef(n1, 0, n2);
		glCallList(pawn);
		glPopMatrix();
	}
	

	GLfloat light_position[] = {(float)sin(t/2)*10,5,(float)cos(t/2)*10, 0}; // light comes FROM this std::vector direction.
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // position first light


	SetTopView();
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	for (int i=0;i<gSliders.size();i++){
		if (i==gActiveSlider){
			gSliders[i].drawActive();
		}
		else{
			gSliders[i].draw();
		}
	}
	SetPerspectiveView();
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);


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

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	//SetPerspectiveView(w,h);

}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	y=screen_y-y;
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		gActiveSlider=-1;
		for (int i=0;i<gSliders.size();i++){
			if (gSliders[i].hovering(x,y)){
				gActiveSlider=i;
			}
		}
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
	}
}
void mousem(int x, int y)
{
	if (gActiveSlider!=-1){
		gSliders[gActiveSlider].drag(x,y);
	}
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	// set material's specular properties
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {50.0};
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	// set light properties
	GLfloat light_position[] = {(float)eye[0], (float)eye[1], (float)eye[2],1};
	GLfloat white_light[] = {1,1,1,1};
	GLfloat low_light[] = {.3f,.3f,.3f,1};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position); // position first light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light); // specify first light's color
	glLightfv(GL_LIGHT0, GL_SPECULAR, low_light);

	glEnable(GL_DEPTH_TEST); // turn on depth buffering
	glEnable(GL_LIGHT0);	 // enable the first light.
	glEnable(GL_CULL_FACE);
	initPieces();
}


int main(int argc, char **argv)
{
	std::cout<<"Use the sliders to control time"<<std::endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(10, 10);

	int fullscreen = 0;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("Time");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mousem);

	glClearColor(1,1,1,1);	
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}

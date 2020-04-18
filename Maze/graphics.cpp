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
#include "maze.h"
#include "../pglut.h"
#include "rat.h"
#include <chrono>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// Global Variables (Only what you need!)
double screen_x = 700;
double screen_y = 500;
struct Input
{
	bool forward=false,left=false,right=false;
};
viewtype current_view = top_view;
Maze gMaze(40,40);
Rat gRat;
Input gInput;
double gTheta=45*3.141592653589793238/180;
double timet=0;
// Textures
unsigned int texName[num_textures];
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
//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(current_view == perspective_view)
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		double cx=gMaze.GetWidth()/2;
		double cy=gMaze.GetHeight()/2;
		gluLookAt(cx*(1+cos(gTheta)),cy*(1+sin(gTheta)),10,  cx,cy,0,  0,0,1);
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
		double z_level = (sin(gTheta*4)+1)/2;
		double x = gRat.GetX();
		double y = gRat.GetY();
		double dx = gRat.GetDX();
		double dy = gRat.GetDY();
		double at_x = x + dx;
		double at_y = y + dy;
		double at_z = z_level;
		gluLookAt(x,y,z_level,  at_x, at_y, at_z,  0,0,1);
	}
	double dt=getDeltaTime();
	//timet+=dt;
	gTheta+=dt/5;
	if (timet>5){
		timet=0;
		gMaze.ReplaceWalls();
		gMaze.RemoveWalls();
	}
	if (gInput.left){
		gRat.TurnLeft(dt);
	}
	if (gInput.right){
		gRat.TurnRight(dt);
	}
	if (gInput.forward){
		gRat.ScurryForward(dt);
	}
	glColor3ub(100,100,255);
	glColor3d(0, 0, 0);
	gMaze.Draw();
	glColor3d(0, 1, 0);
	gRat.Draw();

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
	gluOrtho2D(-world_margin_x, gMaze.GetWidth()+world_margin_x, 
		-world_margin_y, gMaze.GetHeight()+world_margin_y);
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
void InitializeMyTextures()
{
	const char max_file_size = 100;
	char imageFiles[num_textures][max_file_size] = { "texture.png", "brick.jpg" };

	glGenTextures(num_textures, texName);

	for(int i=0; i<num_textures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(imageFiles[i], &width, &height, &nrChannels, 0);
		if (data)
		{	
			if (nrChannels==3){
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else if (nrChannels==4){
				gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			else{
				std::cout<<"Invalid number of channels"<<std::endl;
			}
			
			// NOTE: If the above command doesn't work, try it this way:
				//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				//glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	srand(time(NULL));
	gMaze.RemoveWalls();
	gRat=Rat(.3,90,2,180,&gMaze);
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
	InitializeMyTextures();

	glutMainLoop();

	return 0;
}

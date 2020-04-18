// CS 3600 Graphics Programming
// Spring, 2020
//
// This sample project shows how to import a picture file and use it as a texture map.

#include <iostream>
using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "../pglut.h"

// Global Variables (Only what you need!)
double screen_x = 700;
double screen_y = 500;

// Textures
const int num_textures = 2;
unsigned int texName[num_textures];

//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(1,1,1);

	glLoadIdentity();

	double eye[3] = {5, -1, 11}; // pick a nice vantage point.
	double at[3]  = {5, 3.5, 0};

	gluLookAt(eye[0], eye[1], eye[2],  at[0], at[1], at[2],  0,0,1); // Z is up!

	// Draw quads with texture
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texName[0]); // Sandra

	glBegin(GL_QUADS);
	glTexCoord2f(0,1); glVertex2d(-1,0); // All of Sandra
	glTexCoord2f(1,1); glVertex2d(5,0);
	glTexCoord2f(1,0); glVertex2d(5,8);
	glTexCoord2f(0,0); glVertex2d(-1,8);
	glEnd(); // GL_QUADS

	glBegin(GL_QUADS);
	glTexCoord2f(.2, .7); glVertex2d(5.1, 5); // Part of Sandra
	glTexCoord2f(.8, .5); glVertex2d(8.1, 5);
	glTexCoord2f(.6, .0); glVertex2d(8.1, 8);
	glTexCoord2f(.0, .2); glVertex2d(5.1, 8);
	glEnd(); // GL_QUADS

	glBindTexture(GL_TEXTURE_2D, texName[1]); // Brick

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2d(5.1, 2); // All of brick
	glTexCoord2f(1, 1); glVertex2d(10.1, 2);
	glTexCoord2f(1, 0); glVertex2d(10.1, 5);
	glTexCoord2f(0, 0); glVertex2d(5.1, 5);
	glEnd(); // GL_QUADS

	glBegin(GL_QUADS);
	glTexCoord2f(0, 3); glVertex2d(5.1, -1); // Repeating brick in 3x3 pattern
	glTexCoord2f(3, 3); glVertex2d(10.1, -1);
	glTexCoord2f(3, 0); glVertex2d(10.1, 2);
	glTexCoord2f(0, 0); glVertex2d(5.1, 2);
	glEnd(); // GL_QUADS

	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
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


void SetPerspectiveView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (double) w/(double) h;
	gluPerspective( 
	/* field of view in degree */ 38.0,
	/* aspect ratio */ aspectRatio,
	/* Z near */ 1, /* Z far */ 1000);
	glMatrixMode(GL_MODELVIEW);
}

// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	SetPerspectiveView(w,h);

}

// Your initialization code goes here.
void InitializeMyStuff()
{
	const char max_file_size = 100;
	char imageFiles[num_textures][max_file_size] = { "sandra880.jpg", "brick.jpg" };

	glGenTextures(num_textures, texName);

	for(int i=0; i<num_textures; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(imageFiles[i], &width, &height, &nrChannels, 0);
		if (data)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
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
		glutCreateWindow("Textures Demo");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);

	glClearColor(.2, .5, .1, 0);	
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}

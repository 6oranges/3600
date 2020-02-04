// CS 3600 Graphics Programming
// Spring, 2002
// Program #4 - SpringMan
// This program simulates various forces between particles, such as springs.

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <map>
using namespace std;
#include "../pglut.h"
#include "graphics.h"
#include "particle.h"
#include "forces.h"


// Global Variables
// Some colors you can use, or make your own and add them
// here and in graphics.h
GLdouble redMaterial[] = { 0.7, 0.1, 0.2, 1.0 };
GLdouble greenMaterial[] = { 0.1, 0.7, 0.4, 1.0 };
GLdouble brightGreenMaterial[] = { 0.1, 0.9, 0.1, 1.0 };
GLdouble blueMaterial[] = { 0.1, 0.2, 0.7, 1.0 };
GLdouble whiteMaterial[] = { 1.0, 1.0, 1.0, 1.0 };

double screen_x = 700;
double screen_y = 500;
double speed_m = 1;
bool mousedown=false;
int mousex=0;
int mousey=0;
int pindex=-1;
// The particle system.
ParticleSystem PS;

// 
// Functions that draw basic primitives
//
void DrawCircle(double x1, double y1, double radius)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < 32; i++)
	{
		double theta = (double)i / 32.0 * 2.0 * 3.1415926;
		double x = x1 + radius * cos(theta);
		double y = y1 + radius * sin(theta);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawLine(double x1, double y1, double x2, double y2)
{
	glBegin(GL_LINES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();
}

void DrawRectangle(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glVertex2d(x1, y1);
	glVertex2d(x2, y1);
	glVertex2d(x2, y2);
	glVertex2d(x1, y2);
	glEnd();
}

void DrawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
	glBegin(GL_TRIANGLES);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glVertex2d(x3, y3);
	glEnd();
}

// Outputs a string of text at the specified location.
void text_output(double x, double y, char* string)
{
	void* font = GLUT_BITMAP_9_BY_15;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int len, i;
	glRasterPos2d(x, y);
	len = (int)strlen(string);
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
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3dv(whiteMaterial);

	double DeltaT;
	DeltaT = getDeltaTime();
	if (DeltaT > .05) {
		DeltaT = 0;
	}
	else {
		DeltaT *= speed_m;
	}
	DeltaT=speed_m/60;
	
	//EulerStep(PS, DeltaT);
	//MidpointStep(PS, DeltaT);
	RungeKuttaStep(PS, DeltaT);

	int N = PS.GetNumParticles();
	int NF = PS.GetNumForces();

	// Check Resulting particles for wall collisions
	for (i = 0; i < N; i++)
	{
		Particle* p = PS.GetParticle(i);
		double radius = p->GetRadius();
		double x = p->GetPositionx();
		double y = p->GetPositiony();
		double xDir = p->GetDirectionx();
		double yDir = p->GetDirectiony();

		// bounce off left wall
		if (x - radius < 0)
		{
			p->SetPositionx(radius);
			p->SetDirectionx(fabs(xDir));
		}

		// bounce off right wall
		if (x + radius > screen_x)
		{
			p->SetPositionx(screen_x - radius);
			p->SetDirectionx(-fabs(xDir));
		}

		// bounce off bottom wall
		if (y - radius < 0)
		{
			p->SetPositiony(radius);
			p->SetDirectiony(fabs(yDir));
		}

		// bounce off top wall
		if (y + radius > screen_y)
		{
			p->SetPositiony(screen_y - radius);
			p->SetDirectiony(-fabs(yDir));
		}
	}


	// Draw Spring Forces as edges
	for (i = 0; i < NF; i++)
	{
		Force* f = PS.GetForce(i);
		if (f->Type() == SPRING_FORCE)
		{
			SpringForce* sf = (SpringForce*)f;
			Particle* p1 = sf->GetParticle1();
			Particle* p2 = sf->GetParticle2();
			glColor3d(sf->r(), sf->g(), sf->b());
			DrawLine(p1->GetPositionx(), p1->GetPositiony(), p2->GetPositionx(), p2->GetPositiony());
		}
	}

	// Draw Particles
	for (i = 0; i < N; i++)
	{
		Particle* p = PS.GetParticle(i);
		double radius = p->GetRadius();

		double thePos[DIM];
		p->GetPosition(thePos);
		if (p->GetAnchored())
			glColor3dv(redMaterial);
			if (mousedown&&pindex==i){
				p->SetPositionx(mousex);
				p->SetPositiony(mousey);
			}
		else
			glColor3dv(whiteMaterial);
			if (mousedown&&pindex==i){
				p->SetDirectionx(p->GetDirectionx()+(mousex-thePos[0])*.2);
				p->SetDirectiony(p->GetDirectiony()+(mousey-thePos[1])*.2);
			}
		DrawCircle(thePos[0], thePos[1], radius);
		
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
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// go into 2D mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set the world coordinates.
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);

}
void motion(int x, int y)
{
	mousex=x;
	mousey=screen_y-y;
}
// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	mousex=x;
	mousey=screen_y-y;
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mousedown=true;
		int ld=10000000;
		pindex=-1;
		for (int i = 0; i < PS.GetNumParticles(); i++)
		{
			Particle* p = PS.GetParticle(i);

			double thePos[DIM];
			p->GetPosition(thePos);
			double d2=(mousex-thePos[0])*(mousex-thePos[0])+(mousey-thePos[1])*(mousey-thePos[1]);
			if (d2<ld){
				ld=d2;
				pindex=i;
			}
		}
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		mousedown=false;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
	}
	glutPostRedisplay();
}
class Parser {
public:
	Parser(istream* fin);
	Parser(string filename);
	double Value();
	void ParseBlock(ParticleSystem& ps);
private:
	istream* mFin;
	map<string, double> mLabels;
	map<string, string> mBlocks;
	map<string, vector<double>> mBundles;
	string mBundleIndex;
	int mBundleValueIndex;
	vector<Particle*> mParticles;

};
Parser::Parser(istream* fin) : mFin(fin) {
	mLabels["true"] = 1;
	mLabels["false"] = 0;
}
double Parser::Value() {
	if (mBundleIndex != "") {
		mBundleValueIndex += 1;
		if (mBundleValueIndex >= mBundles[mBundleIndex].size()) {
			mBundleIndex = "";
		}
		else {
			return mBundles[mBundleIndex][mBundleValueIndex];
		}
	}
	string value;
	*mFin >> value;
	if (value == "add" || value == "+") {
		return Value() + Value();
	}
	else if (value == "sub" || value == "-") {
		return Value() - Value();
	}
	else if (value == "mul" || value == "*") {
		return Value() * Value();
	}
	else if (value == "div" || value == "/") {
		return Value() / Value();
	}
	else if (value == "sin") {
		return sin(Value());
	}
	else if (value == "cos") {
		return cos(Value());
	}
	else if (value == "atan2") {
		return atan2(Value(), Value());
	}
	else if (value == "root") {
		return sqrt(Value());
	}
	else if (value == "rand") {
		return  (double) rand() / (RAND_MAX + 1.0);
	}
	else if (mLabels.count(value) > 0) {
		return mLabels[value];
	}
	else if (mBundles.count(value) > 0) {
		mBundleValueIndex = 0;
		return mBundles[value][mBundleValueIndex];
	}
	else {
		double v;
		stringstream(value) >> v;
		return v;
	}
}
void Parser::ParseBlock(ParticleSystem& ps) {
	while (*mFin) {
		string command;
		*mFin >> command;
		mLabels["point"] = mParticles.size();
		if (command == "p") {
			double x, y, dx, dy, r;
			bool fixed;
			x = Value();
			y = Value();
			dx = Value();
			dy = Value();
			r = Value();
			fixed = Value();
			Particle* p1 = new Particle(x,y,dx,dy,r,fixed);
			ps.AddParticle(p1);
			mParticles.push_back(p1);
		}
		else if (command == "s") {
			int p1, p2;
			double s, d, r, g, b, p;
			p1 = Value();
			p2 = Value();
			s = Value();
			d = Value();
			r = Value();
			g = Value();
			b = Value();
			p = Value();
			ps.AddForce(new SpringForce(mParticles[p1], mParticles[p2],s,d,r,g,b,p));
		}
		else if (command == "points") {
			double dx, dy, r;
			bool fixed;
			int points;
			dx = Value();
			dy = Value();
			r = Value();
			fixed = Value();
			points = Value();
			for (int i = 0;i < points;i++) {
				double x, y;
				x = Value();
				y = Value();
				Particle* p1 = new Particle(x, y, dx, dy, r, fixed);
				ps.AddParticle(p1);
				mParticles.push_back(p1);
			}
		}
		else if (command == "dpoints") {
			double dx, dy, r;
			bool fixed;
			int points;
			double x, y;
			x = Value();
			y = Value();
			dx = Value();
			dy = Value();
			r = Value();
			fixed = Value();
			points = Value();
			for (int i = 0;i < points;i++) {
				x += Value();
				y += Value();
				Particle* p1 = new Particle(x, y, dx, dy, r, fixed);
				ps.AddParticle(p1);
				mParticles.push_back(p1);
			}
		}
		else if (command == "spoints") {
			double dx, dy, r;
			bool fixed;
			int points;
			double sx, sy;
			sx = Value();
			sy = Value();
			dx = Value();
			dy = Value();
			r = Value();
			fixed = Value();
			points = Value();
			for (int i = 0;i < points;i++) {
				double x, y;
				x = Value();
				y = Value();
				Particle* p1 = new Particle(x+sx, y+sy, dx, dy, r, fixed);
				ps.AddParticle(p1);
				mParticles.push_back(p1);
			}
		}
		else if (command == "string") {
			double s, d, r, g, b, p;
			int start, points;
			s = Value();
			d = Value();
			r = Value();
			g = Value();
			b = Value();
			p = Value();
			start = Value();
			points = Value();
			for (int i = 1;i < points;i++) {
				ps.AddForce(new SpringForce(mParticles[start+i], mParticles[start+i-1], s, d, r, g, b, p));
			}
		}
		else if (command == "matrix") {
			double s, d, r, g, b, p;
			int start, points;
			s = Value();
			d = Value();
			r = Value();
			g = Value();
			b = Value();
			p = Value();
			start = Value();
			points = Value();
			for (int i = 0;i < points;i++) {
				for (int j = i+1;j < points;j++) {
					ps.AddForce(new SpringForce(mParticles[start + i], mParticles[start + j], s, d, r, g, b, p));
				}
			}
		}
		else if (command == "screen") {
			screen_x = Value();
			screen_y = Value();
		}
		else if (command == "speed") {
			speed_m = Value();
		}
		else if (command == "g") {
			double gravity[DIM] = { Value(), Value() };
			Force* gf = new GravityForce(gravity, &ps);
			ps.AddForce(gf);
		}
		else if (command == "set" || command == "=") {
			string label;
			*mFin >> label;
			mLabels[label] = Value();
		}
		else if (command == "{") {
			char v = 0;
			string label;
			*mFin >> label;
			stringstream ss;
			while (*mFin && v != '}') {
				if (v) {
					ss << v;
				}
				(*mFin).get(v);
			}
			mBlocks[label] = ss.str();
		}
		else if (command == "/*") {
			string v;
			while (*mFin && v != "*/") {
				*mFin >> v;
			}
		}
		else if (command == "//") {
			char v = 0;
			while (*mFin && v != '\n') {
				(*mFin).get(v);
			}
		}
		else if (command == "bundle") {
			string label;
			*mFin >> label;
			vector<double> b;
			int length = Value();
			for (int i = 0;i < length;i++) {
				b.push_back(Value());
			}
			mBundles[label] = b;
		}
		else if (command == "call") {

			string label;
			*mFin >> label;
			stringstream ss = stringstream(mBlocks[label]); // new stream
			istream* nFin = &ss; // Location new stream
			istream* t = mFin; // Location old stream
			mFin = nFin; // Change location
			ParseBlock(ps); // Call
			mFin = t; // Change back
		}else if (command=="rep"){
			istream* t = mFin; // Location old stream
			string label;
			int amount;
			*mFin >> label;
			amount = Value();
			
			
			
			for (int x=0;x<amount;x++){
				stringstream ss = stringstream(mBlocks[label]); // new stream
				istream* nFin = &ss; // Location new stream
				mFin = nFin; // Change location
				ParseBlock(ps); // Call
			}
			
			mFin = t; // Change back
		}
		else if (command == "say") {
			string label;
			*mFin >> label;
			if (mLabels.count(label)>0){
				cout << label << ":" << mLabels[label] << endl;
			}
			else if (mBundles.count(label)>0){
				cout << label << ":<";
				if (mBundles[label].size()>0){
					cout << mBundles[label][0];
					for (int i=1;i<mBundles[label].size();i++){
						cout << ", " << mBundles[label][i];
					}
				}
				cout << ">" << endl;
			}
			
		}
		else if (command == "d") {
			Force* df = new DragForce(Value(), &PS);
			PS.AddForce(df);
		}
		else {
			if (command.size()>0){
				cout << "No entiendo: " << command << endl;
			}
		}
	}
}

int main(int argc, char** argv)
{
	string filename;
	if (argc > 1) {
		filename = argv[1];
	}
	else {
		cout << "Please enter filename: ";
		cin >> filename;
	}
	ifstream fin;
	fin.open(filename);
	Parser p = Parser(&fin);
	p.ParseBlock(PS);
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
		glutCreateWindow("Shapes");
	}
	glutDisplayFunc(display);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glClearColor(.3, .3, .3, 0);

	glutMainLoop();

	return 0;
}

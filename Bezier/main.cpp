#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include "../pglut.h"
#include "slider.h"


// Global Variables
double screen_x = 700;
double screen_y = 500;
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
class Point{
	public:
		Point(double x, double y);
		Point();
		double x,y;
};
std::map<int,Point> gPoints;
class Bezier{
	public:
		Bezier(int a, int b, int c, int d);
		Bezier();
		Point Evaluate(double t);
		void DrawCurve();
		void DrawControlPoints();
		int i0,i1,i2,i3;
		double r,g,b;
};
Point::Point(double _x,double _y):x(_x),y(_y){}
Bezier::Bezier(int _a, int _b, int _c, int _d):i0(_a),i1(_b),i2(_c),i3(_d){}
Point::Point(){}
Bezier::Bezier(){}
Point Bezier::Evaluate(double t){
	Point p0=gPoints[i0];
	Point p1=gPoints[i1];
	Point p2=gPoints[i2];
	Point p3=gPoints[i3];
	double x = p0.x*(1-t)*(1-t)*(1-t) + 3*p1.x*(1-t)*(1-t)*t + 3*p2.x*(1-t)*t*t + p3.x*t*t*t;
	double y = p0.y*(1-t)*(1-t)*(1-t) + 3*p1.y*(1-t)*(1-t)*t + 3*p2.y*(1-t)*t*t + p3.y*t*t*t;
	return Point(x,y);
}
void Bezier::DrawCurve(){
	glColor3d(r,g,b);
	glBegin(GL_LINE_STRIP);
	for (int i=0;i<21;i++){
		Point p = Evaluate(i/20.0);
		glVertex2d(p.x,p.y);
	}
	glEnd();
}
void Bezier::DrawControlPoints(){
	double r=5;
	Point p0=gPoints[i0];
	Point p1=gPoints[i1];
	Point p2=gPoints[i2];
	Point p3=gPoints[i3];
	DrawCircle(p0.x,p0.y,r);
	DrawCircle(p1.x,p1.y,r);
	DrawCircle(p2.x,p2.y,r);
	DrawCircle(p3.x,p3.y,r);
}
std::map<int,Bezier> gCurves;
int gSelectedPoint=-1;
int gNextPoint=0;
int gNextCurve=0;
int gCurveFocus=0;
std::vector<int> gNextCurvePoints;
int gActiveSlider=-1;
std::vector<Slider> gSliders;
//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3d(1,1,1);
	for (auto it=gPoints.begin();it!=gPoints.end();it++){
		DrawCircle(it->second.x,it->second.y,5);
	}
	glLineWidth(2);
	for (auto it=gCurves.begin();it!=gCurves.end();it++){
		it->second.DrawCurve();
	}
	glLineWidth(5);
	gCurves[gCurveFocus].DrawCurve();
	
	
	for (int i=0;i<gNextCurvePoints.size();i++){
		glColor3d(1,0,0);
		DrawCircle(gPoints[gNextCurvePoints[i]].x,gPoints[gNextCurvePoints[i]].y,7);
	}
	if (gSelectedPoint!=-1){
		glColor3d(1,1,0);
		DrawCircle(gPoints[gSelectedPoint].x,gPoints[gSelectedPoint].y,6);
	}
	for (int i=0;i<gSliders.size();i++){
		if (i==gActiveSlider){
			gSliders[i].drawActive();
		}
		else{
			gSliders[i].draw();
		}
	}
	glutSwapBuffers();
}

void newCurve(){
	double x=rand()%(int)screen_x;
	double y=rand()%(int)screen_y;
	gPoints[gNextPoint]=Point(x,y);
	gNextPoint+=1;
	x=rand()%(int)screen_x;
	y=rand()%(int)screen_y;
	gPoints[gNextPoint]=Point(x,y);
	gNextPoint+=1;
	x=rand()%(int)screen_x;
	y=rand()%(int)screen_y;
	gPoints[gNextPoint]=Point(x,y);
	gNextPoint+=1;
	x=rand()%(int)screen_x;
	y=rand()%(int)screen_y;
	gPoints[gNextPoint]=Point(x,y);
	gNextPoint+=1;
	gCurves[gNextCurve]=Bezier(gNextPoint-4,gNextPoint-3,gNextPoint-2,gNextPoint-1);
	double r = (rand()%256)/255.0;
	double g = (rand()%256)/255.0;
	double b = (rand()%256)/255.0;
	gCurves[gNextCurve].r=r;
	gCurves[gNextCurve].g=g;
	gCurves[gNextCurve].b=b;
	gNextCurve+=1;
}
inline bool fexists (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
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
	case 'n':
		// do something when 'b' character is hit.
		newCurve();
		break;
	case 'e':{
		// Export
		int i=0;
		std::stringstream n;
		n<<"images/out0.svg";
		while (fexists(n.str())){
			i++;
			n.str("");
			n<<"images/out";
			n<<i;
			n<<".svg";
		}
		std::ofstream fout(n.str());
		if (!fout){
			std::cout<<"could not export to: "<<n.str()<<std::endl;
		}
		fout << "<svg width=\""<<screen_x<<"\" height=\""<<screen_y<<"\" xmlns=\"http://www.w3.org/2000/svg\">";
		for (auto it=gCurves.begin();it!=gCurves.end();it++){
			Bezier& b=it->second;
			Point p0=gPoints[b.i0];
			Point p1=gPoints[b.i1];
			Point p2=gPoints[b.i2];
			Point p3=gPoints[b.i3];
			p0.y=screen_y-p0.y;
			p1.y=screen_y-p1.y;
			p2.y=screen_y-p2.y;
			p3.y=screen_y-p3.y;
			fout<<"<path d=\"M "<<p0.x<<" "<<p0.y<<" C "<<p1.x<<" "<<p1.y<<", "<<p2.x<<" "<<p2.y<<", "<<p3.x<<" "<<p3.y<<"\" stroke-width=\"2\" stroke=\"#";
			const char* t = "0123456789abcdef";
			int r=(int)(b.r*255);
			char rh1 = t[r/16];
			char rh2 = t[r%16];
			int g=(int)(b.g*255);
			char gh1 = t[g/16];
			char gh2 = t[g%16];
			int bc=(int)(b.b*255);
			char bh1 = t[bc/16];
			char bh2 = t[bc%16];
			fout<<rh1<<rh2<<gh1<<gh2<<bh1<<bh2;
			fout<<"\" fill=\"transparent\"/>";
		}
		fout<<"</svg>";
		fout.close();
		break;
	}
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
	y=screen_y-y;
	if (gSelectedPoint!=-1){
		gPoints[gSelectedPoint].x=x;
		gPoints[gSelectedPoint].y=y;
	}
	if (gActiveSlider!=-1){
		gSliders[gActiveSlider].drag(x,y);
	}
	glutPostRedisplay();
}
int closestPoint(double x,double y){
	double closest=INFINITY;
	int index=-1;
	for (auto it=gPoints.begin();it!=gPoints.end();it++){
		double px=it->second.x;
		double py=it->second.y;
		double dx=px-x;
		double dy=py-y;
		double d2=dx*dx+dy*dy;
		if (d2<closest){
			index=it->first;
			closest=d2;
		}
	}
	return index;
}
// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	y=screen_y-y;
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		int closest=closestPoint(x,y);
		double px=gPoints[closest].x;
		double py=gPoints[closest].y;
		double dx=px-x;
		double dy=py-y;
		if (dx*dx+dy*dy<625){
			gSelectedPoint=closest;
		}
		for (auto it=gCurves.begin();it!=gCurves.end();it++){
			if (gSelectedPoint==it->second.i0||gSelectedPoint==it->second.i1||gSelectedPoint==it->second.i2||gSelectedPoint==it->second.i3){
				gCurveFocus=it->first;
				gSliders[0].setValue(&gCurves[gCurveFocus].r);
				gSliders[1].setValue(&gCurves[gCurveFocus].g);
				gSliders[2].setValue(&gCurves[gCurveFocus].b);
			}
		}
		gActiveSlider=-1;
		for (int i=0;i<gSliders.size();i++){
			if (gSliders[i].hovering(x,y)){
				gActiveSlider=i;
			}
		}
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		gSelectedPoint=-1;
		gActiveSlider=-1;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		gPoints[gNextPoint]=Point(x,y);
		gNextPoint+=1;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		int closest=closestPoint(x,y);
		double px=gPoints[closest].x;
		double py=gPoints[closest].y;
		double dx=px-x;
		double dy=py-y;
		if (dx*dx+dy*dy<625){
			gNextCurvePoints.push_back(closest);
			if (gNextCurvePoints.size()==4){
				gCurves[gNextCurve]=Bezier(gNextCurvePoints[0],gNextCurvePoints[1],gNextCurvePoints[2],gNextCurvePoints[3]);
				gNextCurvePoints.clear();
				gNextCurve+=1;
			}
		}
		else{
			gNextCurvePoints.clear();
		}
	}
	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	newCurve();
	gSliders.push_back(Slider(0,1,15,100,200,15,&gCurves[0].r));
	gSliders.push_back(Slider(0,1,15,60,200,15,&gCurves[0].g));
	gSliders.push_back(Slider(0,1,15,20,200,15,&gCurves[0].b));
	std::cout<<"Left click to select point&curve"<<std::endl;
	std::cout<<"Middle click to add point"<<std::endl;
	std::cout<<"Right click points to make curve"<<std::endl;
	std::cout<<"Sliders to control color of curve"<<std::endl;
	std::cout<<"Press n for new curve"<<std::endl;
	std::cout<<"Press e to export to a svg"<<std::endl;
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

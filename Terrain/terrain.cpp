#include "graphics.h"
#include "terrain.h"
#include <stdlib.h>
#include "../pglut.h"
Terrain::Terrain(std::function<double(double,double)> generator, std::function<double(double,double,double)> red, std::function<double(double,double,double)> green, std::function<double(double,double,double)> blue, std::function<double(double,double,double)> alpha){
	mGenerator=generator;
	mRed=red;
	mBlue=blue;
	mGreen=green;
	mAlpha=alpha;
}
double Terrain::GetZ(double x, double y){
	return mGenerator(x,y);
}
void Terrain::DrawUniform(double xmin, double ymin, double xmax, double ymax, int cols, int rows){
	double x=xmin;
	double y=ymin;
	double xc=(xmax-xmin)/cols;
	double yc=(ymax-ymin)/rows;
	glBegin(GL_TRIANGLES);
	for (int r=0;r<rows;r++){
		for (int c=0;c<cols;c++){
			//glColor3ub((int)(32895*x+34743*y)%256,(int)(78653*x+865845*y)%256,(int)(37547*x+23543*y)%256);
			double xy=GetZ(x,y);
			double xcy=GetZ(x+xc,y);
			double xyc=GetZ(x,y+yc);
			double xcyc=GetZ(x+xc,y+yc);

			double rxy=mRed(x,y,xy);
			double gxy=mGreen(x,y,xy);
			double bxy=mBlue(x,y,xy);
			double axy=mAlpha(x,y,xy);

			double rxcy=mRed(x+xc,y,xcy);
			double gxcy=mGreen(x+xc,y,xcy);
			double bxcy=mBlue(x+xc,y,xcy);
			double axcy=mAlpha(x+xc,y,xcy);

			double rxyc=mRed(x,y+yc,xyc);
			double gxyc=mGreen(x,y+yc,xyc);
			double bxyc=mBlue(x,y+yc,xyc);
			double axyc=mAlpha(x,y+yc,xyc);

			double rxcyc=mRed(x+xc,y+yc,xcyc);
			double gxcyc=mGreen(x+xc,y+yc,xcyc);
			double bxcyc=mBlue(x+xc,y+yc,xcyc);
			double axcyc=mAlpha(x+xc,y+yc,xcyc);

			glColor4d(rxy,gxy,bxy,axy);
			glVertex3d(x,y,xy);
			glColor4d(rxcyc,gxcyc,bxcyc,axcyc);
			glVertex3d(x+xc,y+yc,xcyc);
			glColor4d(rxcy,gxcy,bxcy,axcy);
			glVertex3d(x+xc,y,xcy);

			glColor4d(rxy,gxy,bxy,axy);
			glVertex3d(x,y,xy);
			glColor4d(rxcyc,gxcyc,bxcyc,axcyc);
			glVertex3d(x+xc,y+yc,xcyc);
			glColor4d(rxyc,gxyc,bxyc,axyc);
			glVertex3d(x,y+yc,xyc);
			GetZ(x,y);
			x+=xc;
		}
		x=xmin;
		y+=yc;
	}
	glEnd();
}
#include <iostream>
void Terrain::DrawDecending(double xmin, double ymin, double xmax, double ymax, int scols, int srows, int cols, int rows){
	double x=xmin;
	double y=ymin;
	double xc=(xmax-xmin)/cols;
	double yc=(ymax-ymin)/rows;
	int cc=cols/2;
	int rc=rows/2;
	for (int r=0;r<rows;r++){
		for (int c=0;c<cols;c++){
			int d = (abs(r-rc)+abs(c-cc)+1);
			int mx=scols/d;
			int my=srows/d;
			DrawUniform(x,y,x+xc,y+yc,mx,my);
			x+=xc;
		}
		x=xmin;
		y+=yc;
	}
}
	

#include "../pglut.h"
#include "slider.h"
Slider::Slider(double& v, double from, double to, double x, double y, double width, double height):mValue(v){
	mFrom=from;
	mTo=to;
	mX=x;
	mY=y;
	mWidth=width;
	mHeight=height;
}
bool Slider::hovering(double x, double y){
	return x>=mX&&y>=mY&&x<mX+mWidth&&y<mHeight+mY;
}
void Slider::drag(double x, double y){
	double r=(x-mX)/mWidth;
	if (r<0){
		r=0;
	}
	else if (r>1){
		r=1;
	}
	mValue=r*(mTo-mFrom)+mFrom;
}
void Slider::draw(){
	glBegin(GL_QUADS);
	glColor3d(.9,.9,.9);
	glVertex2d(mX,mY);
	glVertex2d(mX+mWidth,mY);
	glVertex2d(mX+mWidth,mY+mHeight);
	glVertex2d(mX,mY+mHeight);

	double x = (mValue-mFrom)/(mTo-mFrom)*mWidth;
	glColor3d(1,0,0);
	glVertex2d(mX,mY);
	glVertex2d(mX+x,mY);
	glVertex2d(mX+x,mY+mHeight);
	glVertex2d(mX,mY+mHeight);

	double m = 2;
	glColor3d(1,0,1);
	glVertex2d(mX+x-m,mY-m);
	glVertex2d(mX+x+m,mY-m);
	glVertex2d(mX+x+m,mY+mHeight+m);
	glVertex2d(mX+x-m,mY+mHeight+m);
	glEnd();
}
void Slider::drawActive(){
	glBegin(GL_QUADS);
	glColor3d(.8,.8,.8);
	glVertex2d(mX,mY);
	glVertex2d(mX+mWidth,mY);
	glVertex2d(mX+mWidth,mY+mHeight);
	glVertex2d(mX,mY+mHeight);

	double x = (mValue-mFrom)/(mTo-mFrom)*mWidth;
	glColor3d(1,0,0);
	glVertex2d(mX,mY);
	glVertex2d(mX+x,mY);
	glVertex2d(mX+x,mY+mHeight);
	glVertex2d(mX,mY+mHeight);

	double m = 2;
	glColor3d(1,0,1);
	glVertex2d(mX+x-m,mY-m);
	glVertex2d(mX+x+m,mY-m);
	glVertex2d(mX+x+m,mY+mHeight+m);
	glVertex2d(mX+x-m,mY+mHeight+m);
	glEnd();
}
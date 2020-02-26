#include <vector>
#include "graphics.h"
#include "rat.h"
#include <stdlib.h>
#include <math.h>
#include "../pglut.h"
const double PI = 3.141592653589793238;
Rat::Rat(double radius, double degrees, double speed, double rotspeed, Maze* maze)
: mRadius(radius), mDegrees(degrees), mSpeed(speed), mRotSpeed(rotspeed), mMaze(maze) {
	mX=maze->GetStartCell()+.5;
	mY=.5;


}
Rat::Rat(){

}
double Rat::GetX(){return mX;}
double Rat::GetY(){return mY;}
double Rat::GetDX(){return cos(mDegrees*PI/180);}
double Rat::GetDY(){return sin(mDegrees*PI/180);}

void Rat::ScurryForward(double dt){
	double dx=cos(mDegrees*PI/180)*mSpeed*dt;
	double dy=sin(mDegrees*PI/180)*mSpeed*dt;
	if (mMaze->IsSafe(mX+dx,mY+dy,mRadius)){
		mX+=dx;
		mY+=dy;
	}
	else if (mMaze->IsSafe(mX+dx,mY,mRadius)){
		mX+=dx;
	}
	else if (mMaze->IsSafe(mX,mY+dy,mRadius)){
		mY+=dy;
	}
	else if (!(mMaze->IsSafe(mX,mY,mRadius))){
		mX=(int)mX+.5;
		mY=(int)mY+.5;
	}
}
void Rat::TurnLeft(double dt){
	mDegrees+=mRotSpeed*dt;
}
void Rat::TurnRight(double dt){
	mDegrees-=mRotSpeed*dt;
}
void Rat::Draw(){
	if(current_view == rat_view)
	{
		return; // drawing yourself in rat view looks bad.
	}
	glPushMatrix();
	glTranslated(mX,mY,0);
	glRotated(mDegrees,0,0,1);
	DrawTriangle(-.3,.15,-.3,-.15,0,0);
	DrawCircle(.15,0,.2);
	glPopMatrix();
}
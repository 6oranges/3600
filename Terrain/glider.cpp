#include <vector>
#include "graphics.h"
#include "glider.h"
#include <stdlib.h>
#include <math.h>
#include "../pglut.h"
const double PI = 3.141592653589793238;
Glider::Glider(double radius, double degrees, double speed, double rotspeed)
: mRadius(radius), mDegrees(degrees), mSpeed(speed), mRotSpeed(rotspeed){
	mX=.5;
	mY=.5;


}
Glider::Glider(){

}
double Glider::GetX(){return mX;}
double Glider::GetY(){return mY;}
double Glider::GetDX(){return cos(mDegrees*PI/180);}
double Glider::GetDY(){return sin(mDegrees*PI/180);}

void Glider::ScurryForward(double dt){
	double dx=cos(mDegrees*PI/180)*mSpeed*dt;
	double dy=sin(mDegrees*PI/180)*mSpeed*dt;
	mX+=dx;
	mY+=dy;
}
void Glider::TurnLeft(double dt){
	mDegrees+=mRotSpeed*dt;
}
void Glider::TurnRight(double dt){
	mDegrees-=mRotSpeed*dt;
}
void Glider::Draw(){
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
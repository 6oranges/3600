#pragma once
#include "maze.h"
class Rat
{
public:
	Rat(double radius, double degrees, double speed, double rotspeed, Maze* maze);
	Rat();
	void ScurryForward(double dt);
	void TurnLeft(double dt);
	void TurnRight(double dt);
	void Draw();
	double GetX();
	double GetDX();
	double GetY();
	double GetDY();
	

private:
	double mX, mY, mRadius, mDegrees, mSpeed, mRotSpeed;
	Maze* mMaze;
};
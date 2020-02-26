#pragma once
class Glider
{
public:
	Glider(double radius, double degrees, double speed, double rotspeed);
	Glider();
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
};
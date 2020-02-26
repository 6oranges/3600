#pragma once
#include <functional>
class Terrain
{
public:
	Terrain(std::function<double(double,double)> generator, std::function<double(double,double,double)> red, std::function<double(double,double,double)> green, std::function<double(double,double,double)> blue, std::function<double(double,double,double)> alpha);
	double GetZ(double x, double y);
	void DrawUniform(double xmin, double ymin, double xmax, double ymax, int cols, int rows);
	void DrawDecending(double xmin, double ymin, double xmax, double ymax, int scols, int srows, int cols, int rows);
	

private:
	std::function<double(double,double)> mGenerator;
	std::function<double(double,double,double)> mRed;
	std::function<double(double,double,double)> mGreen;
	std::function<double(double,double,double)> mBlue;
	std::function<double(double,double,double)> mAlpha;
};
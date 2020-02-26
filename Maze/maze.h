#pragma once
#include <vector>
struct Cell
{
	Cell();
	void Draw(int x, int y);
	bool left, top, right, bottom;
	bool visited;
};

class Maze
{
public:
	Maze(int width, int height);
	void RemoveWalls();
	void ReplaceWalls();
	void RemoveWallsRB(int i, int j);
	void RemoveWallsHAK();
	void Draw();
	bool IsSafe(double x, double y, double r); // Assuming 1x1 cells and translated already
	int GetWidth(){return mWidth;}
	int GetHeight(){return mHeight;}
	int GetStartCell();
	

private:
	std::vector<std::vector<Cell>> mCells;
	int mWidth, mHeight, mStart, mEnd;
};
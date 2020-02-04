#pragma once

const int WIDTH = 16;
const int HEIGHT =15;

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
	Maze();
	void RemoveWalls();
	void RemoveWallsR(int i, int j);
	void Draw();

private:
	Cell cells[WIDTH][HEIGHT];
};
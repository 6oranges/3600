#include <vector>
#include "graphics.h"
#include "maze.h"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "../pglut.h"
Cell::Cell()
{
	left = top = right = bottom = true;
	visited = false;
}
void Cell::Draw(int x, int y)
{
	if(current_view == top_view) 
	{
		if (left)
			DrawLine(x, y, x, y + 1);
		if (top)
			DrawLine(x, y + 1, x + 1, y + 1);
		if (right)
			DrawLine(x + 1, y + 1, x + 1, y);
		if (bottom)
			DrawLine(x + 1, y, x, y);
	}
	else
	{
		// draw walls as GL_QUADS
		// figure out a way to draw each wall in a different color. (you don't have to save the color of the wall)
		// figure out a way to prevent two co-planar wall from 'bleeding' on top of each other when drawing.
		glColor3ub((x*234589+y*43986)%256,(x*346754+y*357331)%256,(x*435745+y*544523)%256);
		if (left){
			glBegin(GL_QUADS);
			glVertex3d(x,y,0);
			glVertex3d(x,y+1,0);
			glVertex3d(x,y+1,1);
			glVertex3d(x,y,1);
			glEnd();
		}
		glColor3ub((x*456434+y*34745)%256,(x*346347+y*24643)%256,(x*54745+y*246346)%256);
		if (top){
			glBegin(GL_QUADS);
			glVertex3d(x,y+1,0);
			glVertex3d(x+1,y+1,0);
			glVertex3d(x+1,y+1,1);
			glVertex3d(x,y+1,1);
			glEnd();
		}
		glColor3ub((x*457548+y*24634)%256,(x*2365346+y*34643)%256,(x*34636+y*432634)%256);
		if (right){
			glBegin(GL_QUADS);
			glVertex3d(x+1,y,1);
			glVertex3d(x+1,y+1,1);
			glVertex3d(x+1,y+1,0);
			glVertex3d(x+1,y,0);
			glEnd();
		}
		glColor3ub((x*453463+y*43754)%256,(x*65857+y*43634)%256,(x*436632+y*235252)%256);
		if (bottom){
			glBegin(GL_QUADS);
			glVertex3d(x,y,1);
			glVertex3d(x+1,y,1);
			glVertex3d(x+1,y,0);
			glVertex3d(x,y,0);
			glEnd();
		}
	}
	
}
int Maze::GetStartCell(){
	return mStart;
}


bool Maze::IsSafe(double x, double y, double r){
	int i=x;
	int j=y;
	if (i<0||j<0||i>=mWidth||j>=mHeight){
		return true;
	}
	double xo=x-i;
	double yo=y-j;
	Cell c=mCells[i][j];
	// Walls
	if (c.right&&xo+r>1){
		return false;
	}
	if (c.left&&xo-r<0){
		return false;
	}
	if (c.top&&yo+r>1){
		return false;
	}
	if (c.bottom&&yo-r<0){
		return false;
	}
	// Corners
	if (xo+r>1&&yo+r>1){
		return false;
	}
	if (xo-r<0&&yo+r>1){
		return false;
	}
	if (xo+r>1&&yo-r<0){
		return false;
	}
	if (xo-r<0&&yo-r<0){
		return false;
	}
	return true;
}
Maze::Maze(int width, int height)
: mWidth(width), mHeight(height), mCells(width)
{
	for (int i = 0; i < width; i++) { 
        mCells[i] = std::vector<Cell>(height); 
	}
}
void Maze::ReplaceWalls(){
	for (int i = 0;i<mWidth;i++){
		for (int j=0;j<mHeight;j++){
			mCells[i][j].left=true;
			mCells[i][j].right=true;
			mCells[i][j].top=true;
			mCells[i][j].bottom=true;
			mCells[i][j].visited=false;
		}
	}
}
void Maze::RemoveWalls()
{
	//RemoveWallsR(0, 0);
	RemoveWallsHAK();
	// Remove top and bottom wall
	mStart=rand()%mWidth;
	mEnd=rand()%mWidth;
	mCells[mStart][0].bottom=false;
	mCells[mEnd][mHeight-1].top=false;
}

void Maze::RemoveWallsRB(int i, int j)
{
	mCells[i][j].visited = true;

	while (true)
	{
		// record legal possibilitites between LEFT, UP, RIGHT, DOWN
		enum MOVES { LEFT, UP, RIGHT, DOWN };
		std::vector<MOVES> moves;

		// check for a legal LEFT move
		if (i - 1 >= 0 && !mCells[i - 1][j].visited)
		{
			moves.push_back(LEFT);
		}
		if (i + 1 < mWidth && !mCells[i + 1][j].visited)
		{
			moves.push_back(RIGHT);
		}
		if (j - 1 >= 0 && !mCells[i][j - 1].visited)
		{
			moves.push_back(DOWN);
		}
		if (j + 1 < mHeight && !mCells[i][j + 1].visited)
		{
			moves.push_back(UP);
		}
		// check other 3 directions

		if (moves.size() == 0)
		{
			return;
		}

		// pick which direction randomly
		int r = rand() % moves.size();

		if (moves[r] == LEFT)
		{
			mCells[i][j].left = false;
			mCells[i - 1][j].right = false;
			RemoveWallsRB(i - 1, j);
		}
		if (moves[r] == RIGHT)
		{
			mCells[i][j].right = false;
			mCells[i + 1][j].left = false;
			RemoveWallsRB(i + 1, j);
		}
		if (moves[r] == DOWN)
		{
			mCells[i][j].bottom = false;
			mCells[i][j - 1].top = false;
			RemoveWallsRB(i, j - 1);
		}
		if (moves[r] == UP)
		{
			mCells[i][j].top = false;
			mCells[i][j + 1].bottom = false;
			RemoveWallsRB(i, j + 1);
		}
		// Likewise for other 3 directions

	}

}

void Maze::RemoveWallsHAK()
{
	for (int x = 0;x<mWidth;x++){
		for (int y=0;y<mHeight;y++){
			int i=x,j=y;
			while (true)
			{
				mCells[i][j].visited = true;
				// record legal possibilitites between LEFT, UP, RIGHT, DOWN
				enum MOVES { LEFT, UP, RIGHT, DOWN };
				std::vector<MOVES> moves;

				// check for a legal LEFT move
				if (i - 1 >= 0 && !mCells[i - 1][j].visited)
				{
					moves.push_back(LEFT);
				}
				if (i + 1 < mWidth && !mCells[i + 1][j].visited)
				{
					moves.push_back(RIGHT);
				}
				if (j - 1 >= 0 && !mCells[i][j - 1].visited)
				{
					moves.push_back(DOWN);
				}
				if (j + 1 < mHeight && !mCells[i][j + 1].visited)
				{
					moves.push_back(UP);
				}

				if (moves.size() == 0)
				{
					break;
				}

				// pick which direction randomly
				int r = rand() % moves.size();

				if (moves[r] == LEFT)
				{
					mCells[i][j].left = false;
					mCells[i - 1][j].right = false;
					i-=1;
				}
				if (moves[r] == RIGHT)
				{
					mCells[i][j].right = false;
					mCells[i + 1][j].left = false;
					i+=1;
				}
				if (moves[r] == DOWN)
				{
					mCells[i][j].bottom = false;
					mCells[i][j - 1].top = false;
					j-=1;
				}
				if (moves[r] == UP)
				{
					mCells[i][j].top = false;
					mCells[i][j + 1].bottom = false;
					j+=1;
				}

			}
		}
	}
	

}
void Maze::Draw()
{
	glEnable(GL_CULL_FACE);
	for (int i = 0; i < mWidth; i++)
		for (int j = 0; j < mHeight; j++)
			mCells[i][j].Draw(i, j);
}

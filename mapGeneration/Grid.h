#pragma once

#include "Tile.h"
#include "Pathfinding.h"

#include <vector>
#include <string>
#include <tuple>
#include <map>

class Grid
{
public:
	Grid();
	~Grid();


	Grid::Grid(std::tuple<int,int> widthAndHeight, std::tuple<int,int> xYOffsets, int currentFloor, int lastFloor);

	void Grid::levelGeneration_Original();
	void Grid::levelGeneration_PathCreationWithBFS();

	void Grid::levelGeneration_byQuadrants();

	void Grid::draw();
	void Grid::blank();


	int width;
	int height;

	int floor;
	int endFloor;

	int drawOffset_x;
	int drawOffset_y;



	std::vector< std::vector<Tile> > tileList;




};


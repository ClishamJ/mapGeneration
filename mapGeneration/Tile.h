#pragma once
#include "BearLibTerminal.h"


class Tile
{
public:
	Tile();
	~Tile();


	Tile::Tile(int startX, int startY);

	void Tile::draw(int drawOffsetX, int drawOffsetY);
	void Tile::blank(int drawOffsetX, int drawOffsetY);

	int xCoord;
	int yCoord;

	int layer;

	int icon;
	color_t color;

	bool occupied;

	bool occupiedByCreature;
	bool occupiedByObject;


};


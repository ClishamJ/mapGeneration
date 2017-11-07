#include "Tile.h"



Tile::Tile()
{
}


Tile::~Tile()
{
}

Tile::Tile(int startX, int startY)
{

	xCoord = startX;
	yCoord = startY;

	icon = ' ';

	layer = 2;

	color = color_t(0xFFFFFFFF);

	occupied = true;
	occupiedByCreature = false;
	occupiedByObject = false;


}

void Tile::draw(int drawOffsetX, int drawOffsetY)
{

	terminal_layer(layer);
	terminal_color(color);

	terminal_put(xCoord+drawOffsetX, yCoord+drawOffsetY, icon);


}

void Tile::blank(int drawOffsetX, int drawOffsetY)
{

	terminal_layer(layer);
	terminal_clear_area(xCoord+drawOffsetX, yCoord+drawOffsetY, 1, 1);

}

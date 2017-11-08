
#include <cstdio>
#include <string>
#include <ctime>

#include "Grid.h"


// Hide the console window
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main()
{


	terminal_open();

	int screenWidth = 110;
	int screenHeight = 60;


	terminal_set("window: size=110x60, title='Map Generation'; font: terminal16x16_gs_ro.png, size=16x16; input: filter=[keyboard+]"); // for GameLoop: 100x60

	terminal_set("small font: terminal8x8_gs_ro.png, size=8x8");

	terminal_refresh();

	std::srand((unsigned)time(0));


	int gridWidth = 106;
	int gridHeight = 56;

	int grid_xOffset = 2;
	int grid_yOffset = 2;

	Grid newGrid = Grid(std::make_tuple(gridWidth, gridHeight), std::make_tuple(grid_xOffset, grid_yOffset), 1, 1);
	

	terminal_bkcolor(color_t(0xFF121212));
	terminal_clear_area(grid_xOffset, grid_yOffset, gridWidth, gridHeight);

	newGrid.draw();

	
	while (terminal_peek() != TK_CLOSE) {

		if (terminal_has_input()) {

			int currentInput = terminal_read();

			if (currentInput == TK_SPACE) {

				newGrid.blank();
				newGrid = Grid(std::make_tuple(gridWidth, gridHeight), std::make_tuple(grid_xOffset, grid_yOffset), 1, 1);
				newGrid.draw();

			}


		}


		terminal_refresh();

	}



	return 0;

}
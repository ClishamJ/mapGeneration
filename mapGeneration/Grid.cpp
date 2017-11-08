#include "Grid.h"



Grid::Grid()
{
}


Grid::~Grid()
{
}

Grid::Grid(std::tuple<int, int> widthAndHeight, std::tuple<int, int> xYOffsets, int currentFloor, int lastFloor)
{


	width = std::get<0>(widthAndHeight);
	height = std::get<1>(widthAndHeight);


	drawOffset_x = std::get<0>(xYOffsets);
	drawOffset_y = std::get<1>(xYOffsets);

	floor = currentFloor;
	endFloor = lastFloor;



	for (int eachY = 0; eachY < height; eachY++) {

		std::vector<Tile> currentYList = {};

		for (int eachX = 0; eachX < width; eachX++) {

			Tile newTile = Tile(eachX, eachY);
			currentYList.push_back(newTile);




		}

		tileList.push_back(currentYList);

	}




	// Level Generation

	//levelGeneration_Original();
	//levelGeneration_PathCreationWithBFS();

	levelGeneration_byQuadrants();



}

void Grid::levelGeneration_Original()
{

	//
	// Quick and dirty ANGBAND-style implementation; A better concept is to generate rooms smartly and then use A* to ensure connectivity. This is not that.
	//


	// Randomly place rooms all over the place, as long as they don't go off screen

	bool stillGenerating = true;
	while (stillGenerating == true) {



		bool enoughInitialRooms = false;

		std::vector < std::tuple<int, int, int, int> > initialRoomList = {};

		while (enoughInitialRooms == false) {


			int minRoomWidth = 5;
			int maxRoomWidth = 15;

			int minRoomHeight = 5;
			int maxRoomHeight = 15;


			int minNumOfRooms = 5;
			int maxAdditionalRooms = 20;


			int numOfRooms = minNumOfRooms + (int)(maxAdditionalRooms * (rand() / (RAND_MAX + 1.0)));



			for (int eachRoom = 0; eachRoom < numOfRooms; eachRoom++) {

				int roomWidth = (int)(maxRoomWidth * (rand() / (RAND_MAX + 1.0)));
				int roomHeight = (int)(maxRoomHeight * (rand() / (RAND_MAX + 1.0)));

				if (roomWidth < minRoomWidth) {
					roomWidth = minRoomWidth;
				}

				if (roomHeight < minRoomHeight) {
					roomHeight = minRoomHeight;
				}

				int roomAttemptCounter = 0;
				int roomAttemptAmount = 100;

				bool acceptablePlacement = false;
				while (acceptablePlacement == false) {

					if (roomAttemptCounter >= roomAttemptAmount) {
						roomAttemptCounter = 0;
						acceptablePlacement = true;
					}
					else {
						roomAttemptCounter++;

						int xToPlace = (int)(width * (rand() / (RAND_MAX + 1.0)));
						int yToPlace = (int)(height * (rand() / (RAND_MAX + 1.0)));

						bool acceptableSpot = true;

						for (int eachY = 0; eachY < roomHeight; eachY++) {

							for (int eachX = 0; eachX < roomWidth; eachX++) {

								if (eachX + xToPlace >= width || eachY + yToPlace >= height || eachX + xToPlace < 0 || eachY + yToPlace < 0) {
									acceptableSpot = false;
								}
								else if (tileList[yToPlace + eachY][xToPlace + eachX].occupied == false) {
									acceptableSpot = false;
								}

							}

						}

						if (acceptableSpot == true) {

							for (int eachY = 0; eachY < roomHeight; eachY++) {

								for (int eachX = 0; eachX < roomWidth; eachX++) {

									tileList[yToPlace + eachY][xToPlace + eachX].occupied = false;
									tileList[yToPlace + eachY][xToPlace + eachX].icon = 219; // 219 is a solid block character
									tileList[yToPlace + eachY][xToPlace + eachX].color = color_t(0x4400FF00);

								}

							}

							initialRoomList.push_back(std::make_tuple(xToPlace, yToPlace, roomWidth, roomHeight));

							acceptablePlacement = true;
						}

					}

				}





			}

			if (initialRoomList.size() >= 2) {
				enoughInitialRooms = true;
			}
			else {

				for (int eachY = 0; eachY < height; eachY++) {


					for (int eachX = 0; eachX < width; eachX++) {

						tileList[eachY][eachX].occupied = true;

					}

				}

				initialRoomList.clear();

			}



		}


		// Go through initial room list and make a ton of random connections to other rooms (sequentially). Save the rooms that connect, discard the rest.


		int roomIndex = rand() % initialRoomList.size();

		std::vector < std::tuple<int, int, int, int> > finalRoomList = {};

		finalRoomList.push_back(initialRoomList[roomIndex]);
		initialRoomList.erase(initialRoomList.begin() + roomIndex);


		roomIndex = 0;


		int stillConnectingRoomsCounter = 0;
		int stillConnectingRoomsTimeout = 10000;

		bool stillConnectingRooms = true;
		while (stillConnectingRooms == true) {

			if (stillConnectingRoomsCounter >= stillConnectingRoomsTimeout) {
				stillConnectingRooms = false;
			}


			bool madeAConnection = false;

			int currentStep = 1;

			//while (madeAConnection == false) {

			int roomX = std::get<0>(finalRoomList[roomIndex]);
			int roomY = std::get<1>(finalRoomList[roomIndex]);
			int roomWidth = std::get<2>(finalRoomList[roomIndex]);
			int roomHeight = std::get<3>(finalRoomList[roomIndex]);

			// decide whether to go N, S, E, W

			std::string directionToTry;

			int xToStartAt = 0;
			int yToStartAt = 0;

			int xCoeff = 0;
			int yCoeff = 0;

			int choiceToTry = rand() % 4;
			if (choiceToTry == 0) {
				directionToTry = "E";

				xToStartAt = roomX + roomWidth;
				yToStartAt = roomY + rand() % roomHeight;

				xCoeff = 1;
				yCoeff = 0;
				currentStep = 3;

			}
			else if (choiceToTry == 1) {
				directionToTry = "W";

				xToStartAt = roomX - 1;
				yToStartAt = roomY + rand() % roomHeight;

				xCoeff = -1;
				yCoeff = 0;
				currentStep = 3;

			}
			else if (choiceToTry == 2) {
				directionToTry = "N";

				xToStartAt = roomX + rand() % roomWidth;
				yToStartAt = roomY - 1;

				xCoeff = 0;
				yCoeff = -1;
				currentStep = 3;

			}
			else if (choiceToTry == 3) {
				directionToTry = "S";

				xToStartAt = roomX + rand() % roomWidth;
				yToStartAt = roomY + roomHeight;

				xCoeff = 0;
				yCoeff = 1;
				currentStep = 3;

			}

			bool maybe_hitARoom = false;
			bool tryingDirection = true;
			int directionTestMax = 6;

			while (tryingDirection == true) {

				if (currentStep >= directionTestMax || roomX + (xCoeff*currentStep) > width || roomY + (yCoeff*currentStep) > height || roomX + (xCoeff*currentStep) < 0 || roomY + (yCoeff*currentStep) < 0) {

					tryingDirection = false;

				}
				else {

					if (tileList[roomY + (yCoeff*currentStep)][roomX + (xCoeff*currentStep)].occupied == false) { // hit a room

						tryingDirection = false;
						maybe_hitARoom = true;

					}
					else {
						currentStep++;
					}

				}

			}


			if (maybe_hitARoom == true) {


				for (int eachInitialRoom = 0; eachInitialRoom < initialRoomList.size(); eachInitialRoom++) {

					int currentX = xToStartAt + (xCoeff*currentStep);
					int currentY = yToStartAt + (yCoeff*currentStep);

					if (currentX >= std::get<0>(initialRoomList[eachInitialRoom]) && currentY >= std::get<1>(initialRoomList[eachInitialRoom]) && currentX < std::get<0>(initialRoomList[eachInitialRoom]) + std::get<2>(initialRoomList[eachInitialRoom]) && currentY < std::get<1>(initialRoomList[eachInitialRoom]) + std::get<3>(initialRoomList[eachInitialRoom])) {

						finalRoomList.push_back(initialRoomList[eachInitialRoom]);
						initialRoomList.erase(initialRoomList.begin() + eachInitialRoom);
						roomIndex++;

						for (int eachStep = currentStep - 1; eachStep >= -1; eachStep--) {

							tileList[yToStartAt + (yCoeff*eachStep)][xToStartAt + (xCoeff*eachStep)].occupied = false;
							tileList[yToStartAt + (yCoeff*eachStep)][xToStartAt + (xCoeff*eachStep)].icon = 219; // 219 is a solid block character
							tileList[yToStartAt + (yCoeff*eachStep)][xToStartAt + (xCoeff*eachStep)].color = color_t(0x4400FF00);

						}

					}



				}


				maybe_hitARoom = false;

			}




			//}

			stillConnectingRoomsCounter++;

		}

		for (int eachInitialRoom = 0; eachInitialRoom < initialRoomList.size(); eachInitialRoom++) {

			int roomX = std::get<0>(initialRoomList[eachInitialRoom]);
			int roomY = std::get<1>(initialRoomList[eachInitialRoom]);
			int roomWidth = std::get<2>(initialRoomList[eachInitialRoom]);
			int roomHeight = std::get<3>(initialRoomList[eachInitialRoom]);

			for (int eachY = 0; eachY < roomHeight; eachY++) {

				for (int eachX = 0; eachX < roomWidth; eachX++) {

					tileList[roomY + eachY][roomX + eachX].occupied = true;
					tileList[roomY + eachY][roomX + eachX].icon = ' ';

				}

			}

		}

		if (finalRoomList.size() >= 5) {
			stillGenerating = false;
			printf("wooo");
		}

	}

}

void Grid::levelGeneration_PathCreationWithBFS()
{

	// Concept:
	//
	// 1. Pick a bunch of points.
	// 2. Connect them all with BFS
	// 3. Expand out each room with a given width/height (that doesn't go beyond grid borders)
	//
	//
	// More complication:
	//
	// 1. Designate quadrants (top half of screen, bottom half, right half, left half)
	// 2. Split quadrants into several pieces (top left of top half, middle of top half, right of top half, etc)
	// 3. Make each point be within a different piece
	//
	//    - This ensures using multiple parts of the map
	//    - Maybe limits number of rooms? Would have to see results/play with number of quadrants/pieces
	//


	std::vector<std::tuple<int, int>> spotCoeffs = {};
	spotCoeffs.push_back(std::make_tuple(1, 0));
	spotCoeffs.push_back(std::make_tuple(-1, 0));
	spotCoeffs.push_back(std::make_tuple(0, 1));
	spotCoeffs.push_back(std::make_tuple(0, -1));



	bool stillGenerating = true;
	while (stillGenerating == true) {




		int minNumOfRooms = 2;
		int maxAdditionalRooms = 4;


		int numOfRooms = minNumOfRooms + (int)(maxAdditionalRooms * (rand() / (RAND_MAX + 1.0)));

		std::vector<std::tuple<int, int>> spotList;


		for (int eachSpot = 0; eachSpot < numOfRooms; eachSpot++) {


			int xToPlace = (int)(width * (rand() / (RAND_MAX + 1.0)));
			int yToPlace = (int)(height * (rand() / (RAND_MAX + 1.0)));

			if (xToPlace > width) {
				printf(std::to_string(xToPlace).c_str());
			}

			if (yToPlace > height) {
				printf(std::to_string(yToPlace).c_str());
			}

			tileList[yToPlace][xToPlace].occupied = false;
			tileList[yToPlace][xToPlace].icon = 219; // 219 is a solid block character
			tileList[yToPlace][xToPlace].color = color_t(0x220000FF);

			spotList.push_back(std::make_tuple(xToPlace, yToPlace));

		}

		for (int eachSpot = 0; eachSpot < spotList.size()-1; eachSpot++) {


			std::vector<std::tuple<int, int>> pathToNextSpot = Pathfinding::GreedyBFS_findPath_noCollision(tileList, spotList[eachSpot], spotList[eachSpot + 1]);

			for (int eachStep = 0; eachStep < pathToNextSpot.size(); eachStep++) {

				int stepX = std::get<0>(pathToNextSpot[eachStep]);
				int stepY = std::get<1>(pathToNextSpot[eachStep]);

				if (tileList[stepY][stepX].occupied == true) {

					tileList[stepY][stepX].occupied = false;
					tileList[stepY][stepX].icon = 219; // 219 is a solid block character
					tileList[stepY][stepX].color = color_t(0x330000DD);

				}

			}



		}

		//int boxMaxWidth = 20;
		//int boxMinWidth = 3;
		//int boxMaxHeight = 20;
		//int boxMinHeight = 3;

		int boxMaxWidth = 30;
		int boxMinWidth = 15;
		int boxMaxHeight = 30;
		int boxMinHeight = 15;


		for (int eachSpot = 0; eachSpot < spotList.size(); eachSpot++) {


			int eachSpotX = std::get<0>(spotList[eachSpot]);
			int eachSpotY = std::get<1>(spotList[eachSpot]);

			int boxWidth = boxMinWidth + (int)((boxMaxWidth - boxMinWidth) * (rand() / (RAND_MAX + 1.0)));
			int boxHeight = boxMinHeight + (int)((boxMaxHeight - boxMinHeight) * (rand() / (RAND_MAX + 1.0)));

			if (boxWidth >= boxMaxWidth) {
				boxWidth = boxMaxWidth;
			}
			if (boxHeight >= boxMaxHeight) {
				boxHeight = boxMaxHeight;
			}

			int halfBackX = eachSpotX - int(boxWidth / 2);
			int halfBackY = eachSpotY - int(boxHeight / 2);

			if (halfBackX < 0) {
				halfBackX = 0;
			}
			if (halfBackY < 0) {
				halfBackY = 0;
			}



			int numOfAttempts = 100;
			int currentAttempt = 0;


			bool placingBox = true;
			while(placingBox == true){

				if (currentAttempt >= numOfAttempts) {
					placingBox = false;
					currentAttempt = 0;
				}

				//printf("\n");
				//printf("halfBackX: ");
				//printf(std::to_string(halfBackX).c_str());
				//printf("\n");

				//printf("\n");
				//printf("halfBackY: ");
				//printf(std::to_string(halfBackY).c_str());
				//printf("\n");

				//printf("\n");
				//printf("halfBackX+boxWidth: ");
				//printf(std::to_string(halfBackX + boxWidth).c_str());
				//printf("\n");

				//printf("\n");
				//printf("halfBackY+boxHeight: ");
				//printf(std::to_string(halfBackY + boxHeight).c_str());
				//printf("\n");


				// if the box fits in the screen size AND the box still contains the original point (box not floating outside somewhere)
				if ((halfBackX > 0 && halfBackX + boxWidth < width && halfBackY > 0 && halfBackY + boxHeight < height) && (eachSpotX >= halfBackX && eachSpotY >= halfBackY && eachSpotX < halfBackX + boxWidth && eachSpotY < halfBackY + boxHeight)) {

						for (int eachY = 0; eachY < boxHeight; eachY++) {

							for (int eachX = 0; eachX < boxWidth; eachX++) {


								if (tileList[eachY + halfBackY][eachX + halfBackX].color != color_t(0x220000FF)) {

									tileList[eachY + halfBackY][eachX + halfBackX].occupied = false;
									tileList[eachY + halfBackY][eachX + halfBackX].icon = 219; // 219 is a solid block character
									tileList[eachY + halfBackY][eachX + halfBackX].color = color_t(0x440000FF);


								}

							}

						}

						placingBox = false;

				}
				else {
					//printf("DOESN'T FIT");

					int changeToMake = (int)((6) * (rand() / (RAND_MAX + 1.0)));

					if (changeToMake == 0) {

						boxWidth -= 1;
						if (boxWidth < 2) {
							boxWidth = 2;
						}

					}
					else if (changeToMake == 1) {

						boxHeight -= 1;
						if (boxHeight < 2) {
							boxHeight = 2;
						}
					}
					else if (changeToMake == 2) {
						halfBackX++;
						if (halfBackX >= width) {
							halfBackX = width - 1;
						}
					}
					else if (changeToMake == 3) {
						halfBackY++;
						if (halfBackY >= height) {
							halfBackY = height - 1;
						}
					}
					else if (changeToMake == 4) {
						halfBackX--;
						if (halfBackX < 0) {
							halfBackX = 0;
						}
					}
					else if (changeToMake == 5) {
						halfBackY--;
						if (halfBackY < 0) {
							halfBackY = 0;
						}
					}

				}

				currentAttempt++;
			
			
			}
			


		}


		stillGenerating = false;

	}

}

void Grid::levelGeneration_byQuadrants()
{


	int numberOfHorizontalQuadrants = 4;
	int numberOfVerticalQuadrants = 2;


	int widthOfQuadrants = int(width / numberOfHorizontalQuadrants);
	int heightOfQuadrants = int(height / numberOfVerticalQuadrants);

	struct quadrant {

		int startX;
		int startY;

		int width;
		int height;

		int xIndex;
		int yIndex;

	};

	std::vector< std::vector<quadrant> > quadrantList;

	int currentStartX = 0;
	int currentStartY = 0;

	for (int eachY = 0; eachY < numberOfVerticalQuadrants; eachY++) {

		std::vector<quadrant> currentQuadList = {};

		for (int eachX = 0; eachX < numberOfHorizontalQuadrants; eachX++) {

			quadrant newQuad = quadrant();
			newQuad.startX = currentStartX;
			newQuad.startY = currentStartY;
			newQuad.width = widthOfQuadrants;
			newQuad.height = heightOfQuadrants;
			newQuad.xIndex = eachX;
			newQuad.yIndex = eachY;
			currentQuadList.push_back(newQuad);

			currentStartX += widthOfQuadrants;

		}

		quadrantList.push_back(currentQuadList);

		currentStartY += heightOfQuadrants;
		currentStartX = 0;

	}

	printf("wooo");

	int currentRed = 100;
	int currentBlue = 100;
	int currentGreen = 100;


	for (int eachY = 0; eachY < numberOfVerticalQuadrants; eachY++) {

		for (int eachX = 0; eachX < numberOfHorizontalQuadrants; eachX++) {


			for (int eachYCoord = 0; eachYCoord < quadrantList[eachY][eachX].height; eachYCoord++) {

				for (int eachXCoord = 0; eachXCoord < quadrantList[eachY][eachX].width; eachXCoord++) {

					terminal_layer(0);
					terminal_bkcolor(color_from_argb(255, currentRed, currentBlue, currentGreen));
					terminal_clear_area(quadrantList[eachY][eachX].startX+drawOffset_x, quadrantList[eachY][eachX].startY+ drawOffset_y, quadrantList[eachY][eachX].width, quadrantList[eachY][eachX].height);


				}

			}


			currentRed -= 10;
			currentBlue -= 10;
			currentGreen -= 10;


		}

	}




	std::vector<std::tuple<int, int>> spotCoeffs = {};
	spotCoeffs.push_back(std::make_tuple(1, 0));
	spotCoeffs.push_back(std::make_tuple(-1, 0));
	spotCoeffs.push_back(std::make_tuple(0, 1));
	spotCoeffs.push_back(std::make_tuple(0, -1));



	bool stillGenerating = true;
	while (stillGenerating == true) {




		//int minNumOfRooms = 2;
		//int maxAdditionalRooms = 4;


		//int numOfRooms = minNumOfRooms + (int)(maxAdditionalRooms * (rand() / (RAND_MAX + 1.0)));

		//int numOfRooms = 3;

		std::vector<std::tuple<int, int>> spotList;


		std::vector<quadrant> quadrantsToUse = {};


		// Add multiple DIFFERENT quadrants to the list

		int numOfQuadrantsToUse = 3;

		for (int eachQuad = 0; eachQuad < numOfQuadrantsToUse; eachQuad++) {

			int yToUse = (int)(numberOfVerticalQuadrants * (rand() / (RAND_MAX + 1.0)));
			int xToUse = (int)(numberOfHorizontalQuadrants * (rand() / (RAND_MAX + 1.0)));

			bool usableQuad = true;

			for (int eachOtherQuad = 0; eachOtherQuad < quadrantsToUse.size(); eachOtherQuad++) {

				if (quadrantsToUse[eachOtherQuad].xIndex == quadrantList[yToUse][xToUse].xIndex && quadrantsToUse[eachOtherQuad].yIndex == quadrantList[yToUse][xToUse].yIndex) {
					usableQuad = false;
				}

			}

			if (usableQuad == true) {
				quadrantsToUse.push_back(quadrantList[yToUse][xToUse]);
			}

		}
		

		//int nextQuadIndex = 0;
		//while(nextQuad )

		for (int eachSpot = 0; eachSpot < quadrantsToUse.size(); eachSpot++) {


			int xToPlace;
			int yToPlace;

			int quadrantYToUse;
			int quadrantToPick;


			xToPlace = quadrantsToUse[eachSpot].startX + (int)(quadrantsToUse[eachSpot].width * (rand() / (RAND_MAX + 1.0)));
			yToPlace = quadrantsToUse[eachSpot].startY + (int)(quadrantsToUse[eachSpot].height * (rand() / (RAND_MAX + 1.0)));


			//int xToPlace = (int)(width * (rand() / (RAND_MAX + 1.0)));
			//int yToPlace = (int)(height * (rand() / (RAND_MAX + 1.0)));

			if (xToPlace > width) {
				printf(std::to_string(xToPlace).c_str());
			}

			if (yToPlace > height) {
				printf(std::to_string(yToPlace).c_str());
			}

			tileList[yToPlace][xToPlace].occupied = false;
			tileList[yToPlace][xToPlace].icon = 219; // 219 is a solid block character
			tileList[yToPlace][xToPlace].color = color_t(0x220000FF);

			spotList.push_back(std::make_tuple(xToPlace, yToPlace));

		}

		for (int eachSpot = 0; eachSpot < spotList.size() - 1; eachSpot++) {


			std::vector<std::tuple<int, int>> pathToNextSpot = Pathfinding::GreedyBFS_findPath_noCollision(tileList, spotList[eachSpot], spotList[eachSpot + 1]);

			for (int eachStep = 0; eachStep < pathToNextSpot.size(); eachStep++) {

				int stepX = std::get<0>(pathToNextSpot[eachStep]);
				int stepY = std::get<1>(pathToNextSpot[eachStep]);

				if (tileList[stepY][stepX].occupied == true) {

					tileList[stepY][stepX].occupied = false;
					tileList[stepY][stepX].icon = 219; // 219 is a solid block character
					tileList[stepY][stepX].color = color_t(0x330000DD);

				}

			}



		}

		//int boxMaxWidth = 20;
		//int boxMinWidth = 3;
		//int boxMaxHeight = 20;
		//int boxMinHeight = 3;

		int boxMaxWidth = 30;
		int boxMinWidth = 15;
		int boxMaxHeight = 30;
		int boxMinHeight = 15;


		for (int eachSpot = 0; eachSpot < spotList.size(); eachSpot++) {


			int eachSpotX = std::get<0>(spotList[eachSpot]);
			int eachSpotY = std::get<1>(spotList[eachSpot]);

			int boxWidth = boxMinWidth + (int)((boxMaxWidth - boxMinWidth) * (rand() / (RAND_MAX + 1.0)));
			int boxHeight = boxMinHeight + (int)((boxMaxHeight - boxMinHeight) * (rand() / (RAND_MAX + 1.0)));

			if (boxWidth >= boxMaxWidth) {
				boxWidth = boxMaxWidth;
			}
			if (boxHeight >= boxMaxHeight) {
				boxHeight = boxMaxHeight;
			}

			int halfBackX = eachSpotX - int(boxWidth / 2);
			int halfBackY = eachSpotY - int(boxHeight / 2);

			if (halfBackX < 0) {
				halfBackX = 0;
			}
			if (halfBackY < 0) {
				halfBackY = 0;
			}



			int numOfAttempts = 100;
			int currentAttempt = 0;


			bool placingBox = true;
			while (placingBox == true) {

				if (currentAttempt >= numOfAttempts) {
					placingBox = false;
					currentAttempt = 0;
				}

				//printf("\n");
				//printf("halfBackX: ");
				//printf(std::to_string(halfBackX).c_str());
				//printf("\n");

				//printf("\n");
				//printf("halfBackY: ");
				//printf(std::to_string(halfBackY).c_str());
				//printf("\n");

				//printf("\n");
				//printf("halfBackX+boxWidth: ");
				//printf(std::to_string(halfBackX + boxWidth).c_str());
				//printf("\n");

				//printf("\n");
				//printf("halfBackY+boxHeight: ");
				//printf(std::to_string(halfBackY + boxHeight).c_str());
				//printf("\n");


				// if the box fits in the screen size AND the box still contains the original point (box not floating outside somewhere)
				if ((halfBackX > 0 && halfBackX + boxWidth < width && halfBackY > 0 && halfBackY + boxHeight < height) && (eachSpotX >= halfBackX && eachSpotY >= halfBackY && eachSpotX < halfBackX + boxWidth && eachSpotY < halfBackY + boxHeight)) {

					for (int eachY = 0; eachY < boxHeight; eachY++) {

						for (int eachX = 0; eachX < boxWidth; eachX++) {


							if (tileList[eachY + halfBackY][eachX + halfBackX].color != color_t(0x220000FF)) {

								tileList[eachY + halfBackY][eachX + halfBackX].occupied = false;
								tileList[eachY + halfBackY][eachX + halfBackX].icon = 219; // 219 is a solid block character
								tileList[eachY + halfBackY][eachX + halfBackX].color = color_t(0x440000FF);


							}

						}

					}

					placingBox = false;

				}
				else {
					//printf("DOESN'T FIT");

					int changeToMake = (int)((6) * (rand() / (RAND_MAX + 1.0)));

					if (changeToMake == 0) {

						boxWidth -= 1;
						if (boxWidth < 2) {
							boxWidth = 2;
						}

					}
					else if (changeToMake == 1) {

						boxHeight -= 1;
						if (boxHeight < 2) {
							boxHeight = 2;
						}
					}
					else if (changeToMake == 2) {
						halfBackX++;
						if (halfBackX >= width) {
							halfBackX = width - 1;
						}
					}
					else if (changeToMake == 3) {
						halfBackY++;
						if (halfBackY >= height) {
							halfBackY = height - 1;
						}
					}
					else if (changeToMake == 4) {
						halfBackX--;
						if (halfBackX < 0) {
							halfBackX = 0;
						}
					}
					else if (changeToMake == 5) {
						halfBackY--;
						if (halfBackY < 0) {
							halfBackY = 0;
						}
					}

				}

				currentAttempt++;


			}



		}


		stillGenerating = false;

	}










}

void Grid::draw()
{

	// Draw all tiles
	for (int eachY = 0; eachY < height; eachY++) {

		for (int eachX = 0; eachX < width; eachX++) {

			tileList[eachY][eachX].draw(drawOffset_x, drawOffset_y);

		}
		
	}



}

void Grid::blank()
{

	// Blank all tiles
	for (int eachY = 0; eachY < height; eachY++) {

		for (int eachX = 0; eachX < width; eachX++) {

			tileList[eachY][eachX].blank(drawOffset_x, drawOffset_y);

		}


	}

	

}


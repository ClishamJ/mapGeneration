#include "Pathfinding.h"



inline double heuristic(std::tuple<int,int> a, std::tuple<int, int> b) {


	//return abs(a.x - b.x) + abs(a.y - b.y)

	int a_x = std::get<0>(a);
	int a_y = std::get<1>(a);

	int b_x = std::get<0>(b);
	int b_y = std::get<1>(b);

	return abs(a_x - b_x) + abs(a_y - b_y);
}


Pathfinding::Pathfinding()
{
}


Pathfinding::~Pathfinding()
{
}

std::vector<std::tuple<int, int>> Pathfinding::BFS_findPath(std::vector<std::vector<Tile>>& tileList, std::tuple<int, int> startPosition, std::tuple<int, int> goalPosition)
{

	bool foundGoal = false;

	std::vector< std::tuple<int, int> > frontier;
	frontier.push_back(startPosition);

	std::map< std::tuple<int, int>, std::tuple<int,int> > cameFrom;


	int mapWidth = tileList[0].size();
	int mapHeight = tileList.size();

	std::vector< std::tuple<int,int> > directionCoeffs;
	directionCoeffs.push_back(std::make_tuple(1, 0));
	directionCoeffs.push_back(std::make_tuple(-1, 0));
	directionCoeffs.push_back(std::make_tuple(0, 1));
	directionCoeffs.push_back(std::make_tuple(0, -1));

	while (frontier.size() > 0) {


		std::tuple<int, int> current = frontier.back();
		frontier.pop_back();

		if (current == goalPosition) {

			std::string currentXString = std::to_string(std::get<0>(current));
			std::string currentYString = std::to_string(std::get<0>(current));

			foundGoal = true;

			break;
		}

		int currentX = std::get<0>(current);
		int currentY = std::get<1>(current);

		// Add all unfound neighbors to the frontier
		for (int eachNeighbor = 0; eachNeighbor < directionCoeffs.size(); eachNeighbor++) {

			int currentXCoeff = std::get<0>(directionCoeffs[eachNeighbor]);
			int currentYCoeff = std::get<1>(directionCoeffs[eachNeighbor]);

			std::tuple<int,int> neighborTuple = std::make_tuple(currentX + currentXCoeff, currentY + currentYCoeff);

			if (currentY + currentYCoeff < mapHeight && currentY + currentYCoeff >= 0 && currentX + currentXCoeff < mapWidth && currentX + currentXCoeff >= 0) { // make sure neighbor is a real point

				std::map< std::tuple<int, int>, std::tuple<int, int> >::const_iterator it = cameFrom.find(neighborTuple); // check to see if neighbor is already in cameFrom map

				if (it == cameFrom.end() && tileList[currentY + currentYCoeff][currentX+currentXCoeff].occupied == false) { // if neighbor is NOT FOUND in cameFrom map (e.g. if iterator is at the end of the list having not found it)

					frontier.push_back(neighborTuple);
					cameFrom[neighborTuple] = current;

				}

			}

		}

	}

	std::vector< std::tuple<int, int> > pathToGoal = {};

	if (foundGoal == true) {

		

		std::tuple<int, int> currentTrace = goalPosition;
		pathToGoal.push_back(currentTrace);

		while (currentTrace != startPosition) {

			currentTrace = cameFrom[currentTrace];
			if (currentTrace != startPosition) {
				pathToGoal.insert(pathToGoal.begin(), currentTrace);
			}

		}

	}
	else {

		pathToGoal.push_back(std::make_tuple(-1, -1));

	}

	//for (int eachTrace = 0; eachTrace < pathToGoal.size(); eachTrace++) {

	//	int currentX = std::get<0>(pathToGoal[eachTrace]);
	//	int currentY = std::get<1>(pathToGoal[eachTrace]);

	//	printf("\n");
	//	printf(std::to_string(currentX).c_str());
	//	printf(",");
	//	printf(std::to_string(currentY).c_str());
	//	printf("\n");

	//}


	//std::string currentString = std::to_string(std::get<0>(current));
	//printf(currentString.c_str());



	return pathToGoal;
}

std::vector<std::tuple<int, int>> Pathfinding::BFS_findAllPositions(std::vector<std::vector<Tile>>& tileList, std::tuple<int, int> startPosition)
{
	bool foundGoal = false;

	std::vector< std::tuple<int, int> > frontier;
	frontier.push_back(startPosition);

	std::map< std::tuple<int, int>, std::tuple<int, int> > cameFrom;


	int mapWidth = tileList[0].size();
	int mapHeight = tileList.size();

	std::vector< std::tuple<int, int> > directionCoeffs;
	directionCoeffs.push_back(std::make_tuple(1, 0));
	directionCoeffs.push_back(std::make_tuple(-1, 0));
	directionCoeffs.push_back(std::make_tuple(0, 1));
	directionCoeffs.push_back(std::make_tuple(0, -1));

	while (frontier.size() > 0) {


		std::tuple<int, int> current = frontier.back();
		frontier.pop_back();

		int currentX = std::get<0>(current);
		int currentY = std::get<1>(current);

		// Add all unfound neighbors to the frontier
		for (int eachNeighbor = 0; eachNeighbor < directionCoeffs.size(); eachNeighbor++) {

			int currentXCoeff = std::get<0>(directionCoeffs[eachNeighbor]);
			int currentYCoeff = std::get<1>(directionCoeffs[eachNeighbor]);

			std::tuple<int, int> neighborTuple = std::make_tuple(currentX + currentXCoeff, currentY + currentYCoeff);

			if (currentY + currentYCoeff < mapHeight && currentY + currentYCoeff >= 0 && currentX + currentXCoeff < mapWidth && currentX + currentXCoeff >= 0) { // make sure neighbor is a real point

				std::map< std::tuple<int, int>, std::tuple<int, int> >::const_iterator it = cameFrom.find(neighborTuple); // check to see if neighbor is already in cameFrom map

				if (it == cameFrom.end() && tileList[currentY + currentYCoeff][currentX + currentXCoeff].occupied == false) { // if neighbor is NOT FOUND in cameFrom map (e.g. if iterator is at the end of the list having not found it)

					frontier.push_back(neighborTuple);
					cameFrom[neighborTuple] = current;

				}

			}

		}

	}

	std::vector< std::tuple<int, int> > allPaths = {};

	for (std::map< std::tuple<int, int>, std::tuple<int, int> >::const_iterator it = cameFrom.begin(); it != cameFrom.end(); ++it) {
		allPaths.push_back(it->second);
	}

	return allPaths;

}

std::vector<std::tuple<int, int>> Pathfinding::BFS_expansion(std::vector<std::vector<Tile>>& tileList, std::tuple<int, int> startPosition, int roundsToExpand)
{
	bool foundGoal = false;

	std::vector< std::tuple<int, int> > frontier;
	frontier.push_back(startPosition);

	std::map< std::tuple<int, int>, std::tuple<int, int> > cameFrom;


	int mapWidth = tileList[0].size();
	int mapHeight = tileList.size();

	std::vector< std::tuple<int, int> > directionCoeffs;
	directionCoeffs.push_back(std::make_tuple(1, 0));
	directionCoeffs.push_back(std::make_tuple(-1, 0));
	directionCoeffs.push_back(std::make_tuple(0, 1));
	directionCoeffs.push_back(std::make_tuple(0, -1));

	while (roundsToExpand > 0) {


		std::tuple<int, int> current = frontier.back();
		frontier.pop_back();

		int currentX = std::get<0>(current);
		int currentY = std::get<1>(current);

		// Add all unfound neighbors to the frontier
		for (int eachNeighbor = 0; eachNeighbor < directionCoeffs.size(); eachNeighbor++) {

			int currentXCoeff = std::get<0>(directionCoeffs[eachNeighbor]);
			int currentYCoeff = std::get<1>(directionCoeffs[eachNeighbor]);

			std::tuple<int, int> neighborTuple = std::make_tuple(currentX + currentXCoeff, currentY + currentYCoeff);

			if (currentY + currentYCoeff < mapHeight && currentY + currentYCoeff >= 0 && currentX + currentXCoeff < mapWidth && currentX + currentXCoeff >= 0) { // make sure neighbor is a real point

				std::map< std::tuple<int, int>, std::tuple<int, int> >::const_iterator it = cameFrom.find(neighborTuple); // check to see if neighbor is already in cameFrom map

				if (it == cameFrom.end()) { // if neighbor is NOT FOUND in cameFrom map (e.g. if iterator is at the end of the list having not found it)

					frontier.push_back(neighborTuple);
					cameFrom[neighborTuple] = current;

				}

			}

		}

		roundsToExpand--;

	}

	std::vector< std::tuple<int, int> > allPaths = {};

	for (std::map< std::tuple<int, int>, std::tuple<int, int> >::const_iterator it = cameFrom.begin(); it != cameFrom.end(); ++it) {
		allPaths.push_back(it->second);
	}

	return allPaths;
}



std::vector<std::tuple<int, int>> Pathfinding::GreedyBFS_findPath(std::vector<std::vector<Tile>>& tileList, std::tuple<int, int> startPosition, std::tuple<int, int> goalPosition)
{
	bool foundGoal = false;



	PriorityQueue<std::tuple<int,int>, double> frontier;

	frontier.put(startPosition, 0);
	

	std::map< std::tuple<int, int>, std::tuple<int, int> > cameFrom;


	int mapWidth = tileList[0].size();
	int mapHeight = tileList.size();

	std::vector< std::tuple<int, int> > directionCoeffs;
	directionCoeffs.push_back(std::make_tuple(1, 0));
	directionCoeffs.push_back(std::make_tuple(-1, 0));
	directionCoeffs.push_back(std::make_tuple(0, 1));
	directionCoeffs.push_back(std::make_tuple(0, -1));

	while (frontier.empty() == false) {


		std::tuple<int, int> current = frontier.get();

		if (current == goalPosition) {

			std::string currentXString = std::to_string(std::get<0>(current));
			std::string currentYString = std::to_string(std::get<0>(current));

			foundGoal = true;

			break;
		}

		int currentX = std::get<0>(current);
		int currentY = std::get<1>(current);

		// Add all unfound neighbors to the frontier
		for (int eachNeighbor = 0; eachNeighbor < directionCoeffs.size(); eachNeighbor++) {

			int currentXCoeff = std::get<0>(directionCoeffs[eachNeighbor]);
			int currentYCoeff = std::get<1>(directionCoeffs[eachNeighbor]);

			std::tuple<int, int> neighborTuple = std::make_tuple(currentX + currentXCoeff, currentY + currentYCoeff);

			if (currentY + currentYCoeff < mapHeight && currentY + currentYCoeff >= 0 && currentX + currentXCoeff < mapWidth && currentX + currentXCoeff >= 0) { // make sure neighbor is a real point

				std::map< std::tuple<int, int>, std::tuple<int, int> >::const_iterator it = cameFrom.find(neighborTuple); // check to see if neighbor is already in cameFrom map

				if (it == cameFrom.end() && tileList[currentY + currentYCoeff][currentX + currentXCoeff].occupied == false) { // if neighbor is NOT FOUND in cameFrom map (e.g. if iterator is at the end of the list having not found it)


					double priority = heuristic(goalPosition, neighborTuple);
					frontier.put(neighborTuple, priority);
					cameFrom[neighborTuple] = current;

				}

			}

		}

	}

	std::vector< std::tuple<int, int> > pathToGoal = {};

	if (foundGoal == true) {



		std::tuple<int, int> currentTrace = goalPosition;
		pathToGoal.push_back(currentTrace);

		while (currentTrace != startPosition) {

			currentTrace = cameFrom[currentTrace];
			if (currentTrace != startPosition) {
				pathToGoal.insert(pathToGoal.begin(), currentTrace);
			}

		}

	}
	else {

		pathToGoal.push_back(std::make_tuple(-1, -1));

	}

	//for (int eachTrace = 0; eachTrace < pathToGoal.size(); eachTrace++) {

	//	int currentX = std::get<0>(pathToGoal[eachTrace]);
	//	int currentY = std::get<1>(pathToGoal[eachTrace]);

	//	printf("\n");
	//	printf(std::to_string(currentX).c_str());
	//	printf(",");
	//	printf(std::to_string(currentY).c_str());
	//	printf("\n");

	//}


	//std::string currentString = std::to_string(std::get<0>(current));
	//printf(currentString.c_str());



	return pathToGoal;
}

std::vector<std::tuple<int, int>> Pathfinding::GreedyBFS_findPath_noCollision(std::vector<std::vector<Tile>>& tileList, std::tuple<int, int> startPosition, std::tuple<int, int> goalPosition)
{
	bool foundGoal = false;



	PriorityQueue<std::tuple<int, int>, double> frontier;

	frontier.put(startPosition, 0);


	std::map< std::tuple<int, int>, std::tuple<int, int> > cameFrom;


	int mapWidth = tileList[0].size();
	int mapHeight = tileList.size();

	std::vector< std::tuple<int, int> > directionCoeffs;
	directionCoeffs.push_back(std::make_tuple(1, 0));
	directionCoeffs.push_back(std::make_tuple(-1, 0));
	directionCoeffs.push_back(std::make_tuple(0, 1));
	directionCoeffs.push_back(std::make_tuple(0, -1));

	while (frontier.empty() == false) {


		std::tuple<int, int> current = frontier.get();

		if (current == goalPosition) {

			std::string currentXString = std::to_string(std::get<0>(current));
			std::string currentYString = std::to_string(std::get<0>(current));

			foundGoal = true;

			break;
		}

		int currentX = std::get<0>(current);
		int currentY = std::get<1>(current);

		// Add all unfound neighbors to the frontier
		for (int eachNeighbor = 0; eachNeighbor < directionCoeffs.size(); eachNeighbor++) {

			int currentXCoeff = std::get<0>(directionCoeffs[eachNeighbor]);
			int currentYCoeff = std::get<1>(directionCoeffs[eachNeighbor]);

			std::tuple<int, int> neighborTuple = std::make_tuple(currentX + currentXCoeff, currentY + currentYCoeff);

			if (currentY + currentYCoeff < mapHeight && currentY + currentYCoeff >= 0 && currentX + currentXCoeff < mapWidth && currentX + currentXCoeff >= 0) { // make sure neighbor is a real point

				std::map< std::tuple<int, int>, std::tuple<int, int> >::const_iterator it = cameFrom.find(neighborTuple); // check to see if neighbor is already in cameFrom map

				if (it == cameFrom.end()) { // if neighbor is NOT FOUND in cameFrom map (e.g. if iterator is at the end of the list having not found it)


					double priority = heuristic(goalPosition, neighborTuple);
					frontier.put(neighborTuple, priority);
					cameFrom[neighborTuple] = current;

				}

			}

		}

	}

	std::vector< std::tuple<int, int> > pathToGoal = {};

	if (foundGoal == true) {



		std::tuple<int, int> currentTrace = goalPosition;
		pathToGoal.push_back(currentTrace);

		while (currentTrace != startPosition) {

			currentTrace = cameFrom[currentTrace];
			if (currentTrace != startPosition) {
				pathToGoal.insert(pathToGoal.begin(), currentTrace);
			}

		}

	}
	else {

		pathToGoal.push_back(std::make_tuple(-1, -1));

	}

	//for (int eachTrace = 0; eachTrace < pathToGoal.size(); eachTrace++) {

	//	int currentX = std::get<0>(pathToGoal[eachTrace]);
	//	int currentY = std::get<1>(pathToGoal[eachTrace]);

	//	printf("\n");
	//	printf(std::to_string(currentX).c_str());
	//	printf(",");
	//	printf(std::to_string(currentY).c_str());
	//	printf("\n");

	//}


	//std::string currentString = std::to_string(std::get<0>(current));
	//printf(currentString.c_str());



	return pathToGoal;
}



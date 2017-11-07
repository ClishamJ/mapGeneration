#pragma once

#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <queue>
#include <functional>

#include "Tile.h"

class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();


	static std::vector< std::tuple<int, int> > Pathfinding::BFS_findPath(std::vector<std::vector<Tile>>& tileList, std::tuple<int, int> startPosition, std::tuple<int, int> goalPosition);
	static std::vector< std::tuple<int, int> > Pathfinding::BFS_findAllPositions(std::vector<std::vector<Tile>>& tileList, std::tuple<int, int> startPosition);

	static std::vector< std::tuple<int, int> > Pathfinding::BFS_expansion(std::vector<std::vector<Tile>>& tileList, std::tuple<int, int> startPosition, int roundsToExpand);

	static std::vector< std::tuple<int, int> > Pathfinding::GreedyBFS_findPath(std::vector<std::vector<Tile>>& tileList, std::tuple<int, int> startPosition, std::tuple<int, int> goalPosition);

	static std::vector< std::tuple<int, int> > Pathfinding::GreedyBFS_findPath_noCollision(std::vector<std::vector<Tile>>& tileList, std::tuple<int, int> startPosition, std::tuple<int, int> goalPosition);


	//struct PriorityQueue {

	//	typedef pair<std::tuple<int, int>, int> PQElement;
	//	std::priority_queue<PQElement, vector<PQElement>, std::isgreater<PQElement>> elements;

	//	
	//	inline bool empty() const { return elements.empty(); }

	//	inline void put(T item, priority_t priority) {
	//		elements.emplace(priority, item);
	//	}

	//	inline T get() {
	//		T best_item = elements.top().second;
	//		elements.pop();
	//		return best_item;
	//	}



	//};



};

template<typename T, typename priority_t>
struct PriorityQueue {
	typedef std::pair<priority_t, T> PQElement;
	std::priority_queue<PQElement, std::vector<PQElement>,
		std::greater<PQElement>> elements;


	inline bool empty() const { return elements.empty(); }

	inline void put(T item, priority_t priority) {
		elements.emplace(priority, item);
	}

	inline T get() {
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};
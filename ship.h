// roberhaiek 205962657

#include <vector>
#include <unordered_map>
#include <map>
#include <utility>
#include <functional>
#include <iostream>
using namespace std;

// using namespace shipping;
namespace shipping {

	//	Exception definition
	struct BadShipOperationException {
	public:
		std::string error;	// the error to be printed
		BadShipOperationException(std::string msg) :error(msg) {}
		const std::string errorMsg() { return error; }
		void printMsg() { std::cout << error << std::endl; }
	};

	// Named types
	template<typename T>
	class Named {
		T value;
	public:
		explicit Named(T value) : value{ value } {}
		operator T() const { return value; }
	};

	class X : public Named<int> {
		using Named<int>::Named;
	};

	class Y : public Named<int> {
		using Named<int>::Named;
	};

	class Height : public Named<int> {
		using Named<int>::Named;
	};

	// a grouping unordered map, key is a string representation of the grouping, 
	// value is a function that receives a const container and returns a string representing the group name: 
	// <group, f(group) = container's_group> (grouping can be according to destPort, owner, weight, etc...)
	template<typename Container>
	using Grouping = std::unordered_map<std::string, std::function<std::string(const Container&)>>;

	template<typename Container>
	class Ship {
	public:
		typedef typename std::vector<Container>::const_iterator const_iterator;	// the iterator over all the containers
		int x;
		int y;
		int c;	// iteration counter
		Container*** containers;	// contains all the containers as 3D array [row][column][floor]
		std::vector<Container> containerIter;	// contains all the containers unordered (iterator)
		std::map<std::pair<X, Y>, std::pair<int, int>> rowColumn_maxheightSize;	// <(X,Y),(max_height,size)>
		std::map<std::pair<X, Y>, std::vector<Container>>	xyContainers;	// <(X,Y),<containers in (X,Y)>>
		Grouping<Container> groupingFunctions;

		//Ship operator++() { ++c; return containerIter[c]; }

		inline const_iterator begin() const { return containerIter.begin(); }

		inline const_iterator end() const { return containerIter.end(); }

		// fills the rowColumn_maxheightSize map with the relevant max heights
		void handleRestrictions(std::vector<std::tuple<X, Y, Height>> restrictions) {
			for (int i = 0; i < restrictions.size(); i++) {
				rowColumn_maxheightSize.insert_or_assign(std::pair<X, Y>(std::get<0>(restrictions[i]), std::get<1>(restrictions[i])), std::pair<Height, int>(std::get<2>(restrictions[i]), 0));
			}
		}

		// Constructor with ship's dimentions, max height's restrictions, and grouping
		Ship(X x, Y y, Height max_height,
			std::vector<std::tuple<X, Y, Height>> restrictions,
			Grouping<Container> groupingFunctions) noexcept(false) {
			Ship(x, y, max_height, restrictions);
			this->groupingFunctions = groupingFunctions;
		}

		// Constructor with ship's dimentions, max height's restrictions
		Ship(X x, Y y, Height max_height,
			std::vector<std::tuple<X, Y, Height>> restrictions) noexcept(false) {
			Ship(x, y, max_height);
			handleRestrictions(restrictions);
		}

		// Constructor with ship's dimentions
		Ship(X x, Y y, Height max_height) noexcept {
			this->x = x;
			this->y = y;
			c = 0;
			containers = new Container * *[x];
			for (int i = 0; i < x; i++) {
				containers[i] = new Container * [y];
				for (int j = 0; j < y; j++) {
					rowColumn_maxheightSize.insert_or_assign(std::pair<X, Y>(i, j), std::pair<int, int>((int)max_height, 0));
					containers[i][j] = new Container[max_height];
					std::vector<Container> temp;
					xyContainers.insert_or_assign(std::pair<X, Y>(i, j), temp);
				}
			}
		}

		// load/unload/move operations for containers on the ship
		// these 3 methods may return BadShipOperationException
		void load(X x, Y y, Container c) noexcept(false) {
			if (x >= this->x || y >= this->y) {
				throw BadShipOperationException("WRONG LOADING");
			}
			std::pair<X, Y> b(x, y);
			if (rowColumn_maxheightSize.find(b) != rowColumn_maxheightSize.end()) {
				std::pair<int, int> a = (rowColumn_maxheightSize.find(b))->second;
				int size = a.second + 1;
				int max_height = (rowColumn_maxheightSize.find(std::pair<X, Y>(x, y)))->second.first;
				containers[x][y][size - 1] = c;
				rowColumn_maxheightSize.insert_or_assign(std::pair<X, Y>(x, y), std::pair<Height, int>(max_height, size));
				containerIter.push_back(c);
				(xyContainers.find(std::pair<X, Y>(x, y)))->second.push_back(c);
			}
		}

		Container unload(X x, Y y) noexcept(false) {
			if (x >= this->x || y >= this->y) {
				throw BadShipOperationException("WRONG UNLOADING");
			}
			std::pair<X, Y> b(x, y);
			int size = 0;
			if (rowColumn_maxheightSize.find(b) != rowColumn_maxheightSize.end()) {
				std::pair<int, int> a = (rowColumn_maxheightSize.find(b))->second;
				size = a.second;
				int max_height = (rowColumn_maxheightSize.find(std::pair<X, Y>(x, y)))->second.first;
				rowColumn_maxheightSize.insert_or_assign(std::pair<X, Y>(x, y), std::pair<Height, int>(max_height, size));
				for (int i = 0; i < size; i++) {
					if (containerIter[i] == containers[x][y][size - 1]) {
						containerIter.erase(containerIter.begin() + i);
					}
					if ((xyContainers.find(std::pair<X, Y>(x, y)))->second[i] == containers[x][y][size - 1]) {
						(xyContainers.find(std::pair<X, Y>(x, y)))->second.erase((xyContainers.find(std::pair<X, Y>(x, y)))->second.begin() + i);
					}
				}
			}
			return containers[x][y][size];
		}

		void move(X from_x, Y from_y, X to_x, Y to_y) noexcept(false) {
			if (from_x >= this->x || from_y >= this->y || to_x >= this->x || to_y >= this->y) {
				throw BadShipOperationException("WRONG MOVING");
			}
			load(to_x, to_y, unload(from_x, from_y));
		}

		std::vector<Container> getContainersViewByPosition(X x, Y y) const {
			return xyContainers.find(std::pair<X, Y>(x, y))->second;
		}

		auto getContainersViewByGroup(const std::string& groupingName, const std::string& groupName) const {
			if (groupingFunctions.find(groupingName) != groupingFunctions.end()) {
				std::map<std::string,std::string> groups;
				for (int i = 0; i < (int)containerIter.size();i++) {
					groups[groupingFunctions.find(groupingName)->second(containerIter[i])].push_back(containerIter[i]);
				}
				return groups;
			}
		}
	};
}

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
	using Position = std::tuple<shipping::X, shipping::Y, shipping::Height>;
}
	namespace std
	{
		template<> struct hash<shipping::Position>
		{
			std::size_t operator()(const shipping::Position& pos) const noexcept
			{
				return std::get<0>(pos) ^ (std::get<1>(pos) << 1) ^ (std::get<2>(pos) << 2);
			}
		};
	}
	namespace shipping{
	// a grouping unordered map, key is a string representation of the grouping, 
	// value is a function that receives a const container and returns a string representing the group name: 
	// <group, f(group) = container's_group> (grouping can be according to destPort, owner, weight, etc...)
	template<typename Container>
	using Grouping = std::unordered_map<std::string, std::function<std::string(const Container&)>>;

	template<typename Container>
	class Ship {
	public:
		class GroupView {
		const std::unordered_map<shipping::Position, const Container&>* p_group = nullptr;
		using iterator_type = typename std::unordered_map<shipping::Position, const Container&>::const_iterator;
	public:
		GroupView(const std::unordered_map<shipping::Position, const Container&>& group) : p_group(&group) {}
		GroupView(int) {}
		auto begin() const {
			return p_group ? p_group->begin() : iterator_type{};
		}
		auto end() const {
			return p_group ? p_group->end() : iterator_type{};
		}
	};

		typedef typename std::vector<Container>::const_iterator const_iterator;	// the iterator over all the containers
		int x;
		int y;
		int max_height;
		int c;	// iteration counter
		Container*** containers;	// contains all the containers as 3D array [row][column][floor]
		std::vector<Container> containerIter;	// contains all the containers unordered (iterator)
		std::map<std::pair<X, Y>, std::pair<int, int>> rowColumn_maxheightSize;	// <(X,Y),(max_height,size)>
		std::map<std::pair<X, Y>, std::vector<Container>> xyContainers;	// <(X,Y),<containers in (X,Y)>>
		Grouping<Container> groupingFunctions;
		using Pos2Container = std::unordered_map<shipping::Position, const Container&>;
		using Group = std::unordered_map<std::string, Pos2Container>;
		mutable std::unordered_map<std::string, Group> groups;


		void addContainerToGroups(X x, Y y, Height height) {
			Container& c = containers[x][y][height];
			for (auto& group_pair : groupingFunctions) {
				groups[group_pair.first][group_pair.second(c)].insert({ std::tuple{x, y, height}, c });
			}
		}

		void removeContainerFromGroups(X x, Y y, Height height) {
			Container& c = containers[x][y][height];
			for (auto& group_pair : groupingFunctions) {
				groups[group_pair.first][group_pair.second(c)].erase(std::tuple{ x, y, height });
			}
		}

		//Ship operator++() { ++c; return containerIter[c]; }

		inline const_iterator begin() const { return containerIter.begin(); }

		inline const_iterator end() const { return containerIter.end(); }

		// fills the rowColumn_maxheightSize map with the relevant max heights
		void handleRestrictions(std::vector<std::tuple<X, Y, Height>> restrictions) {
			for (int i = 0; i < (int)restrictions.size(); i++) {
				rowColumn_maxheightSize.insert_or_assign(std::pair<X, Y>(std::get<0>(restrictions[i]), std::get<1>(restrictions[i])), std::pair<Height, int>(std::get<2>(restrictions[i]), 0));
			}
		}
		void init(X x, Y y, Height max_height) {
			this->x = (int)x;
			this->y = (int)y;
			this->max_height = (int)max_height;
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
		// Constructor with ship's dimentions, max height's restrictions, and grouping
		Ship(X x, Y y, Height max_height,
			std::vector<std::tuple<X, Y, Height>> restrictions,
			Grouping<Container> groupingFunctions) noexcept(false) {
			init(x, y, max_height);
			handleRestrictions(restrictions);
			this->groupingFunctions = groupingFunctions;
		}

		// Constructor with ship's dimentions, max height's restrictions
		Ship(X x, Y y, Height max_height,
			std::vector<std::tuple<X, Y, Height>> restrictions) noexcept(false) {
			init(x, y, max_height);
			handleRestrictions(restrictions);
		}

		// Constructor with ship's dimentions
		Ship(X x, Y y, Height max_height) noexcept {
			init(x, y, max_height);
		}

		// load/unload/move operations for containers on the ship
		// these 3 methods may return BadShipOperationException
		void load(X x, Y y, Container c) noexcept(false) {
			try {
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
					addContainerToGroups(x, y, (Height)(size - 1));
				}
			}
			catch (...) {
				throw BadShipOperationException("WRONG LOADING");
			}
		}

		Container unload(X x, Y y) noexcept(false) {
			try {
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
					removeContainerFromGroups(x, y, size - 1);
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
			catch (...) {
				throw BadShipOperationException("WRONG LOADING");
			}
		}

		void move(X from_x, Y from_y, X to_x, Y to_y) noexcept(false) {
			try {
				if (from_x >= this->x || from_y >= this->y || to_x >= this->x || to_y >= this->y) {
					throw BadShipOperationException("WRONG MOVING");
				}
				load(to_x, to_y, unload(from_x, from_y));
			}
			catch (...) {
				throw BadShipOperationException("WRONG LOADING");
			}

		}

		std::vector<Container> getContainersViewByPosition(X x, Y y) const {
			return xyContainers.find(std::pair<X, Y>(x, y))->second;
		}
		auto findContainer(Container c) {
			for (int i = 0; i < this->x; i++) {
				for (int j = 0; j < this->y; j++) {
					for (int k = 0; k < this->max_height; k++) {
						if (containers[i][j][k] == c) {
							return std::tuple<X, Y, Height>{i, j, k};
						}
					}
				}
			}
		}
		GroupView getContainersViewByGroup(const std::string& groupingName, const std::string& groupName) const {
			auto itr = groups.find(groupingName);
			if (itr == groups.end() && groupingFunctions.find(groupingName) != groupingFunctions.end()) {
				std::tie(itr, std::ignore) = groups.insert({ groupingName, Group{} });
			}
			if (itr != groups.end()) {
				const auto& grouping = itr->second;
				auto itr2 = grouping.find(groupName);
				if (itr2 == grouping.end()) {
					std::tie(itr2, std::ignore) = itr->second.insert({ groupName, Pos2Container{} });
				}
				return GroupView{ itr2->second };
			}
			return GroupView{ 0 };
		}
	};
}

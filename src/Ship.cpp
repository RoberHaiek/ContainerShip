//============================================================================
// Name        : ContainerShip.cpp
// Author      : Rober Haiek
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Ship.h"
#include "Container.cpp"
#include "Port.cpp"
#include <stack>

#include <iostream>
using namespace std;

class Ship {
public:
	stack<int> ***plan;
	Port *route;
	int weight;
	int shipWidth;
	int shipHeight;

    /*shipWidth and shipHeight added + fixed the way we initialize the plan*/
	Ship(Port *givenRoute, int shipWidth, int shipHeight) {
		weight = 0;
		route = givenRoute;
		this->shipWidth = shipWidth;
		this->shipHeight = shipHeight;
		plan = new Container**[shipWidth];
		for (int i=0; i < shipWidth; i++) {
			plan[i] = new Container*[shipHeight];
			for (int j=0; j < shipHeight; j++) {
				plan[i][j] = new Container;
			}
		}
	}
	Ship(){
		std::cout << "Ship --> default constructor" << std::endl;
		plan = NULL;
		route = NULL;
		weight = 0;
		shipWidth = 0;
		shipHeight = 0;
	}

	Container*** getShipPlan() {
		return plan;
	}

	void optimize() {

	}

	bool weightBalance(Ship ship) {
		return true;
	}


	void load(Container container) {

	}

	void unload(Container container) {

	}

	void stowage(Ship ship, Port currentPort) {
		Container container;
		container = new Container();
		optimize();
		weightBalance(ship);
		container.load(ship);
		container.unload(ship);

	}

};

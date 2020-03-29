//============================================================================
// Name        : ContainerShip.cpp
// Author      : Rober Haiek
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ContainerShip.h"
using namespace std;

class Port {
public:
	string port="emptyString";

	string toString(){
	return port;
}
};

class Container {
public:
	int weight;
	Port destPort;
	string uniqueId;
};

class Ship {
public:
	Container ***plan;
	Port *route;
	int weight;
    /*shipWidth and shipHeight added + fixed the way we init the plan*/
	Ship(Port *givenRoute, int shipWidth, int shipHeight) {
		weight = 0;
		route = givenRoute;
		plan = new Container**[shipWidth];
		for (int i=0; i < shipWidth; i++) {
			plan[i] = new Container*[shipHeight];
			for (int j=0; j < shipHeight; j++) {
				plan[i][j] = new Container;
			}
		}
	}
	Ship(){
		std::cout << "Ship --> default constructer" << std::endl;
		plan =NULL;
		route=NULL;
		weight=0;
	}

	Container*** getShipPlan() {
		return plan;
	}



void optimize() {

}

void weightBalance(Ship ship) {

}

void load(Ship ship, Container container) {

}

void unload(Ship ship, Container container) {

}

void stowage(Ship ship, Port currentPort) {
	Container container;
	optimize();
	weightBalance(ship);
	load(ship, container);
	unload(ship, container);

}

};

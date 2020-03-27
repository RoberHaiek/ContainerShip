//============================================================================
// Name        : ContainerShip.cpp
// Author      : Rober Haiek
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

class Port {
public:
	string port;
};

class Container {
public:
	int weight;
	Port destPort;
	string uniqueId;
};

class Ship {
public:
	Container* plan = NULL;
	Port* route;
	int weight;

	Ship(Port* givenRoute){
		weight = 0;
		route = givenRoute;
		plan = new Container[0][0][0];
	}

	Container* getShipPlan(){
		return NULL;
	}


};

void optimize(){

}

void weightBalance(Ship ship){

}

void load(Ship ship, Container container){

}

void unload(Ship ship, Container container){

}

void stowage(Ship ship, Port currentPort){
	Container container;
	optimize();
	weightBalance(ship);
	load(ship, container);
	unload(ship, container);

}

void

int main() {
	Port* route;
	Ship ship;
	int currentPort = 0;
	cout << "Enter route" << endl; // prints !!!Hello World!!!
	cin >> route;
	ship = new Ship(route);
	while(route[currentPort]!=NULL){
		stowage(ship,route[currentPort]);
	}
	return 0;
}

//============================================================================
// Name        : ContainerShip.cpp
// Author      : Rober Haiek
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Ship.h"
#include "Container.cpp"
#include "Port.cpp"
#include <stack>
#include <map>
#include <iostream>
#include <list>
using namespace std;

class Ship {
public:
	int weight;
	int shipWidth;
	int shipLength;
	int shipHeight;
	Container **instructions;		// each cell (representing each port in the route) has 2 arrays of containers, 1st one is for loading, 2nd for unloading
	Port *route;					// array of ports
	stack<Container> **planStack;	// stack of containers for each cell
	map<string,int[3]> planMap;		// key = container, value = [width,length,height] of the container
	list<Container> tempContainers;

	Ship(Port *route, int shipWidth, int shipLength, int shipHeight, Container **instructions) {
		this->weight = 0;
		this->shipWidth = shipWidth;
		this->shipLength = shipLength;
		this->shipHeight = shipHeight;
		this->instructions=instructions;
		this->route = route;
		// Initializing the plan = a 2D array of stacks of containers
		this->planStack = new stack<Container>*[shipWidth];
		for (int i=0; i < shipWidth; i++) {
			this->planStack[i] = new stack<Container>[shipHeight];
			for (int j=0; j < shipLength; j++) {
				this->planStack[i][j] = new stack<Container>;
			}
		}
		this->planMap = NULL;
		this->tempContainers = NULL;
	}

	Ship(){
		std::cout << "Ship --> default constructor" << std::endl;
		weight = 0;
		shipWidth = 0;
		shipLength = 0;
		shipHeight = 0;
		instructions=NULL;
		route = NULL;
		planStack = NULL;
		planMap = NULL;
		tempContainers = NULL;
	}

	// needs to be done
	stack<Container>** getShipPlan() {
		return plan;
	}

	// Calculates the optimal way to load/unload the containers for the current port - returns exact instructions (uniqueId,load/unload,width,length,isTemp?)!
	string** optimize() {
		weightBalance();
		return NULL;
	}

	bool weightBalance() {
		return true;
	}

	void load(Container container,int width, int length) {
		this.planStack[width][length].push(container);
	}

	void unload(Container container, int width, int length, bool isTemp) {
		if(isTemp){
			this->tempContainers.insert(this.planStack[width][length].pop(container));
			return;
		}
		this.planStack[width][length].pop(container);
	}

	void stowage(Port currentPort) {
		string** currentInstructions;
		int width, length;
		bool isTemp;
		Container currentContainer;
		currentInstructions = optimize();
		for(int i=0;i<currentInstructions;i++){
			width = currentInstructions[i][2];
			length = currentInstructions[i][3];
			isTemp = currentInstructions[i][4];
			if(currentInstructions[i][1].equals("load")){
				if(isTemp){
					currentContainer = this->tempContainers[currentInstructions[i][0]];	// should be modified!!!
					load(currentContainer,width,length);
				}
				else{
					currentContainer = planMap[currentInstructions[i][0]];
					load(currentContainer,width,length);
				}
			}
			else{
				unload(currentContainer,width,length,isTemp);
			}
		}
	}

	// Captain: "Buckle up, we're starting the trip!"
	void start(){
		for(int i=0;i<this->route;i++){
			stowage(this->route[i]);
		}
	}

};

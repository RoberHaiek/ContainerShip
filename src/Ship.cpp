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
#include <queue>
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
	map<string,int[2]> planMap;		// key = container, value = [row,column] of the container
	queue<Container> tempContainers;

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
		return planStack;
	}

	int sizeOfArray(Container* array){
		int c=0;
		while(true){
			if(array[c]==NULL)
				return c;
			c++;
		}
		return 0;
	}

	bool weightBalance() {
		return true;	// we have a magical ship
	}

	void load(Container container,int row, int column) {
		this->planMap.insert(container.uniqueId,{row,column});
		this->planStack[row][column].push(container);
	}

	void unload(Container container, int row, int column, bool isTemp) {
		this->planMap.erase(container.uniqueId);
		if(isTemp){
			this->tempContainers.push(this->planStack[row][column].pop());	// need to check what to pass as an argument
			return;
		}
		this->planStack[row][column].pop();
	}

	void stowage(int i) {
		bool bottomContainer, // true if we're popping a container from a stack - need to pop all above it
			breakIt=false;	  // move to the next container from the port instructions list
		int instNum = 0,row,column;
		Container currentContainer;
		Container* PortInstructions;
		string** currentInstructions;
		PortInstructions = this->instructions[i];
		// unloading containers from SHIP to PORT
		for(row=0;row<this->shipWidth;row++){
			for(column=0;column<this->shipLength;column++){
				bottomContainer=false;
				while(this->planStack[row][column].hasNext()){		// start from the bottom !!!
					currentContainer=this->planStack[row][column].top();
					if(currentContainer.destPort == this->route[i].toString()){	// does this container belong to this port?
						unload(currentContainer,row,column,false);
						currentInstructions[instNum] = {currentContainer.uniqueId,"unload",row,column,"false"};
						instNum++;
						bottomContainer=true;
					}
					else{	// this container does NOT belong to this port
						if(bottomContainer){	// but should I put it in temp?
							unload(currentContainer,row,column,true);
							currentInstructions[instNum] = {currentContainer.uniqueId,"unload",row,column,"true"};
							instNum++;
						}
					}
				}
				// loading containers from temp back to ship
				while(this->tempContainers.hasNext()){
					currentContainer=this->tempContainers.pop();
					load(currentContainer,row,column);
					currentInstructions[instNum]={currentContainer.uniqueId,"load",row,column,"true"};
					instNum++;
				}
			}
		}

		// loading containers from port to ship
		for(int p=0;p<sizeOfArray(PortInstructions);p++){
			currentContainer=PortInstructions[p];
			for(row=0;row<this->shipWidth;row++){
				for(column=0;column<this->shipLength;column++){
					load(currentContainer,row,column);
					if(planStack[row][column].size()<=this->shipHeight && weightBalance()){		// check if we exceeded stack limit, used weight balance :D!
						currentInstructions[instNum] = {currentContainer.uniqueId,"load",row,column,"false"};
						breakIt = true;
						break;
					}
				// else:
					this->planStack[row][column].pop();
					this->planMap.erase(currentContainer.uniqueId);
				}
				if(breakIt){
					breakIt = false;
					break;
				}
			}
		}
	}

	//
	// Captain: "This is you captain speaking - Buckle up, we're starting the trip!"
	//
	void start(){
		for(int i=0;i<this->route;i++){
			stowage(i);
		}
	}

};

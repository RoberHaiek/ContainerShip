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
	struct node
	{
	    Container container;
	    struct node *next;
	};
	int weight;
	int shipWidth;
	int shipLength;
	int shipHeight;
	Container **instructions;		// each cell (representing each port in the route) has 2 arrays of containers, 1st one is for loading, 2nd for unloading
	Port *route;					// array of ports
	stack<Container> **planStack;	// stack of containers for each cell
	map<string,int[2]> planMap;		// key = container, value = [row,column] of the container
	queue<Container> tempContainers;
	node **planLinkedList;


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
		this->planLinkedList = NULL;
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
		planLinkedList = NULL;
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
		this->planMap.insert(container.uniqueId,{row,column});	// Adding container to the map
		this->planStack[row][column].push(container);			// Adding container to stack
		node *temp,newNode;										// Adding container to linked list
		newNode = new node;
		newNode.container=container;
		newNode.next=NULL;
		temp=this->planLinkedList[row][column];
		while((*temp).next!=NULL){
			temp=(*temp).next;
		}
		(*temp).next=newNode;
	}

	void unload(Container container, int row, int column, bool isTemp) {
		node *temp,newNode;
		newNode = new node;
		newNode.container=container;
		newNode.next=NULL;
		temp=this->planLinkedList[row][column];
		while((*temp).next->container.uniqueId!=container.uniqueId && (*temp).next!=NULL){
			temp=(*temp).next;
		}
		(*temp).next=NULL;
		this->planMap.erase(container.uniqueId);
		if(isTemp){
			this->tempContainers.push(container);
		}
		this->planStack[row][column].pop();
	}

	void stowage(int i) {
		bool popAllAbove, // true if we're popping a container from a stack and we need to pop all above it
			breakIt=false;	  // move to the next container from the port instructions list
		int instNum = 0,row,column;	// instNum is the instruction number of the returned instructions
		node currentContainer;
		Container* PortInstructions;
		string** currentInstructions;
		PortInstructions = this->instructions[i];
		// unloading containers from SHIP to PORT
		for(row=0;row<this->shipWidth;row++){
			for(column=0;column<this->shipLength;column++){
				popAllAbove=false;
				currentContainer=this->planLinkedList[row][column];
				while(currentContainer!=NULL){		// starting from the bottom !!!
					if(currentContainer.container.destPort == this->route[i].toString()){	// does this container belong to this port?
						unload(currentContainer.container,row,column,false);
						currentInstructions[instNum] = {currentContainer.container.uniqueId,"unload",row,column,"false"};
						instNum++;
						popAllAbove=true;
					}
					else{	// this container does NOT belong to this port
						if(popAllAbove){	// but should I put it in temp?
							unload(currentContainer.container,row,column,true);
							currentInstructions[instNum] = {currentContainer.container.uniqueId,"unload",row,column,"true"};
							instNum++;
						}
					}
					currentContainer=currentContainer.next;
				}
				// loading containers from temp back to ship
				while(!this->tempContainers.empty()){
					currentContainer=this->tempContainers.pop();
					load(currentContainer.container,row,column);
					currentInstructions[instNum]={currentContainer.container.uniqueId,"load",row,column,"true"};
					instNum++;
				}
			}
		}
		currentContainer.container=NULL;
		currentContainer.next=NULL;
		// loading containers from PORT to SHIP
		for(int p=0;p<sizeOfArray(PortInstructions);p++){
			currentContainer.container=PortInstructions[p];
			for(row=0;row<this->shipWidth;row++){
				for(column=0;column<this->shipLength;column++){
					load(currentContainer.container,row,column);
					if(planStack[row][column].size()<=this->shipHeight && weightBalance()){		// check if we are below stack height limit, used weight balance :D!
						currentInstructions[instNum] = {currentContainer.container.uniqueId,"load",row,column,"false"};
						breakIt = true;
						break;
					}
				// else:
					unload(currentContainer.container,row,column,false);
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

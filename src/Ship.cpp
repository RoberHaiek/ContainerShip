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
	// each node in the linked list represents a container
	struct node
	{
	    Container container;
	    struct node *next;
	};
	// each cellLinkedList represents a cell (specific row and column) in which containers are represented as nodes in the linked list, and the size
	struct cellLinkedList
	{
		node *linkedList;
		int size;
		int maxHeight;
	};
	int weight;
	int shipWidth;
	int shipLength;
	Container **instructions;		// each cell (representing each port in the route) has 2 arrays of containers, 1st one is for loading, 2nd for unloading
	Port *route;					// array of ports
	map<string,int[2]> planMap;		// key = container, value = [row,column] of the container
	queue<Container> tempContainers;
	cellLinkedList **planLinkedList;

	Ship(Port *route, int shipWidth, int shipLength, int shipHeight, Container **instructions) {
		this->weight = 0;
		this->shipWidth = shipWidth;
		this->shipLength = shipLength;
		this->instructions=instructions;
		this->route = route;
		this->planMap = new map<string,int[2]>;
		this->tempContainers = new queue<Container>;
		this->planLinkedList = new cellLinkedList*[shipWidth];
		for(int i=0;i<shipLength;i++){
			this->planLinkedList[i] = new cellLinkedList[shipLength];
		}
		for(int row=0;row<this->shipWidth;row++){
			for(int column=0;column<this->shipLength;column++){
				this->planLinkedList[row][column] = new cellLinkedList;
				this->planLinkedList[row][column].linkedList = NULL;
				this->planLinkedList[row][column].maxHeight = shipHeight;
				this->planLinkedList[row][column].size = 0;
			}
		}
	}

};

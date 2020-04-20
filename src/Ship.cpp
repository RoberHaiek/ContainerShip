//============================================================================
// Name        : ContainerShip.cpp
// Author      : Rober Haiek
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Container.cpp"
#include "Ship.h"
#include <stack>
#include <map>
#include <iostream>

class Ship {
public:
	int weight;
	int shipWidth;
	int shipLength;
	map<string,int*> planMap;		// key = container, value = [row,column] of the container
	cellLinkedList **planLinkedList;

	Ship(int shipWidth, int shipLength, int shipHeight) {
		this->weight = 0;
		this->shipWidth = shipWidth;
		this->shipLength = shipLength;
		this->planMap = map<string,int*>();
		this->planLinkedList = new cellLinkedList*[shipWidth];
		for(int i=0;i<shipWidth;i++){
			this->planLinkedList[i] = new cellLinkedList[shipLength];
		}
		for(int row=0;row<this->shipWidth;row++){
			for(int column=0;column<this->shipLength;column++){
				struct cellLinkedList temp;
				this->planLinkedList[row][column] = temp;
				struct node newNode;
				newNode = this->planLinkedList[row][column].linkedList;
				this->planLinkedList[row][column].maxHeight = shipHeight;
				this->planLinkedList[row][column].size = 0;
			}
		}
	}

	Ship(){
		this->weight = 0;
		this->shipLength = 0;
		this->shipWidth = 0;
		this->planMap = map<string,int*>();
		this->planLinkedList = new cellLinkedList*[1];
	}

	void setHeight(int row, int column, int maxHeight){
		this->planLinkedList[row][column].maxHeight=maxHeight;
	}
	~Ship(){
		delete[] this->planLinkedList[0];
	}

};

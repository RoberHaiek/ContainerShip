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
	int shipHeight;
	map<string,int*> *planMap;		// key = container, value = [row,column] of the container
	cellLinkedList **planLinkedList;

	Ship(int shipWidth, int shipLength, int shipHeight) {
		this->weight = 0;
		this->shipWidth = shipWidth;
		this->shipLength = shipLength;
		this->shipHeight = shipHeight;
		this->planMap = new map<string,int*>();
		this->planLinkedList = new cellLinkedList*[shipWidth];
		for(int i=0;i<shipWidth;i++){
			this->planLinkedList[i] = new cellLinkedList[shipLength];
		}
		for(int row=0;row<this->shipWidth;row++){
			for(int column=0;column<this->shipLength;column++){
				this->planLinkedList[row][column] = cellLinkedList(shipHeight);
				
				//this->planLinkedList[row][column].linkedList = node();
				//this->planLinkedList[row][column].linkedList.next = NULL;
				//this->planLinkedList[row][column].linkedList.container = Container();
				//this->planLinkedList[row][column].maxHeight = shipHeight;
				//this->planLinkedList[row][column].size = 0;
			}
		}
	}
	
	Ship(){
		this->weight = 0;
		this->shipLength = 0;
		this->shipWidth = 0;
		this->shipHeight = 0;
		this->planMap =new map<string,int*>();
		this->planLinkedList = new cellLinkedList*[1];

	}

	void setHeight(int row, int column, int maxHeight){
		this->planLinkedList[row][column].maxHeight=maxHeight;
	}
	~Ship(){
		delete[] this->planLinkedList[0];
	}
	void printPlanMap() {
		cellLinkedList **linkedList = this->planLinkedList;
		node *tmp;
		std::cout
				<< "--------------------printing linked list content---------------------"
				<< endl;
		for (int row = 0; row < this->shipWidth; row++) {
			std::cout << "-size= " << linkedList[row]->size << " MAX_HIGHET= "
					<< linkedList[row]->maxHeight << endl;
			for (int col = 0; col < this->shipLength; col++) {
				std::cout << endl << "**" << row << "**" << col << endl;
				std::cout << "	size= " << linkedList[row][col].size
						<< " MAX_HIGHET= " << linkedList[row][col].maxHeight
						<< endl;
				tmp = linkedList[row][col].linkedList;
				bool isNULL = tmp->next == NULL;
				std::cout << "	-ContainerID: " << tmp->container->uniqueId
						<< "  IS_NEXT_NULL=" << isNULL << endl;

			}
		}
		std::cout << "--------------------END---------------------" << endl
				<< endl;
	}
};

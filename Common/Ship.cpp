#include "Container.cpp"
#include "Ship.h"
#include <stack>
#include <map>
#include <string>
#include <iostream>

class Ship {
public:
	int weight;
	int shipWidth;
	int shipLength;
	int shipHeight;
	std::map<std::string,int*> *planMap;		// key = container, value = [row,column,height] of the container
	std::map<std::string,std::string> *contMap;
	cellLinkedList **planLinkedList;

	Ship(int shipWidth, int shipLength, int shipHeight) {
		this->weight = 0;
		this->shipWidth = shipWidth;
		this->shipLength = shipLength;
		this->shipHeight = shipHeight;
		this->planMap = new std::map<std::string,int*>();
		this->contMap= new std::map<std::string,std::string>();

		this->planLinkedList = new cellLinkedList*[shipWidth];
		for(int i=0;i<shipWidth;i++){
			this->planLinkedList[i] = new cellLinkedList[shipLength];
		}
		for(int row=0;row<this->shipWidth;row++){
			for(int column=0;column<this->shipLength;column++){
				this->planLinkedList[row][column] = cellLinkedList(shipHeight);			}
		}
	}
	
	Ship(){
		this->weight = 0;
		this->shipLength = 0;
		this->shipWidth = 0;
		this->shipHeight = 0;
	}

	void setHeight(int row, int column, int maxHeight){
		this->planLinkedList[row][column].size=shipHeight-maxHeight;
	}
	/*~Ship(){
		for(int i=0;i<shipWidth;i++){
			delete[] this->planLinkedList[i];
		}
		if(this->planLinkedList!=NULL){
			delete this->planLinkedList;
		}
		if(this->planMap!=NULL){
			delete this->planMap;
		}
	}*/
};

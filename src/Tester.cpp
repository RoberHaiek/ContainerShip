/*
 * Tester.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: rober.haiek
 */
#include "Ship.cpp"

class Tester{
public:
	struct node
	{
	    Container container;
	    struct node *next;
	};
	int sizeOfArray(Port* array){
			int c=0;
			while(true){
				if(array[c]==NULL)
					return c;
				c++;
			}
			return 0;
	}

	// checks if a container's destination port is in route
	bool isInRoute(string destPort, Port* route){
		for(int i=0;i<sizeOfArray(route);i++){
			if(destPort==route[i]){
				return true;
			}
		}
		return false;
	}

	// checks if the ship is full
	bool isFull(Ship ship){
		for(int i=0;i<ship.shipLength;i++){
			for(int j=0;j<ship.shipWidth;j++){
				if(ship.planLinkedList[i][j].maxHeight>ship.planLinkedList[i][j].size){
					return true;
				}
			}
		}
		return false;
	}

	// checks if the container weight is legal
	bool isLegalWeight(int containerWeight){
		if(containerWeight>0)
			return true;
		return false;
	}

	// checks if the unique id of a container is valid
	bool isValidId(string UniqueId){
		if(UniqueId.length()!=11)
			return false;
		for(int i=0;i<11;i++){
			if(i<3){
				if(!(UniqueId[3]>=65 && UniqueId[3]<=90)||(UniqueId[3]>=97 && UniqueId[3]<=122)){
						return false;
				}
			}
			if(i==3){
				if(!(UniqueId[4]==74 || UniqueId[4]==85 || UniqueId[4]==90 || UniqueId[4]==106 || UniqueId[4]==117 || UniqueId[4]==122)){
					return false;
				}
			}
			else{
				int currChar = UniqueId[i] - '0';
				if(!(currChar>=0 && currChar<=9)){
					return false;
				}
			}
		}
		return true;
	}

};

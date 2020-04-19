/*
 * CraneTester.cpp
 *
 *  Created on: 19 Apr 2020
 *      Author: rober.haiek
 */

#include "Ship.cpp"
class CraneTester{
public:
// checks if the ship is full
	static bool isFull(Ship ship, string uniqueId){
		for(int i=0;i<ship.shipLength;i++){
			for(int j=0;j<ship.shipWidth;j++){
				if(ship.planLinkedList[i][j].maxHeight>ship.planLinkedList[i][j].size){
					std::cout << "Container "+uniqueId+" was rejected - reason: ship is full";
					return true;
				}
			}
		}
		return false;
	}

	// checks if the container weight is legal
	static bool isLegalWeight(int containerWeight, string uniqueId){
		if(containerWeight>0)
			return true;
		std::cout << "Container "+uniqueId+" was rejected - reason: container unique id is invalid";
		return false;
	}

	// checks if the unique id of a container is valid
	static bool isValidId(string uniqueId){
		if(uniqueId.length()!=11)
			return false;
		for(int i=0;i<11;i++){
			if(i<3){
				if(!(uniqueId[3]>=65 && uniqueId[3]<=90)||(uniqueId[3]>=97 && uniqueId[3]<=122)){
						std::cout << "Container "+uniqueId+" was rejected - reason: container weight is invalid";
						return false;
				}
			}
			if(i==3){
				if(!(uniqueId[4]==74 || uniqueId[4]==85 || uniqueId[4]==90 || uniqueId[4]==106 || uniqueId[4]==117 || uniqueId[4]==122)){
					std::cout << "Container "+uniqueId+" was rejected - reason: container weight is invalid";
					return false;
				}
			}
			else{
				int currChar = uniqueId[i] - '0';
				if(!(currChar>=0 && currChar<=9)){
					std::cout << "Container "+uniqueId+" was rejected - reason: container weight is invalid";
					return false;
				}
			}
		}
		return true;
	}

	static bool isValidLoad(int row, int column, int floor, int shipWidth, int shipLength, int cellHeight, string uniqueId){
		if(row>=shipWidth || row<0 || column>=shipLength || column<0 || floor>=cellHeight || floor<0){
			std::cout << "Container "+uniqueId+" was rejected - reason: loading container into an invalid ship indexes";
			return false;
		}
		return true;
	}

	static bool isValidUnload(int row, int column, int floor, int shipWidth, int shipLength, int cellHeight, string uniqueId){
		if(row != shipWidth || column!=shipLength || floor!=cellHeight){
			std::cout << "Container "+uniqueId+" was rejected - reason: unloading container from an invalid ship indexes";
			return false;
		}
		return true;
	}
};

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
	static bool isFull(Ship* ship, string uniqueId){
		cout<<"	isFull"<<endl;
		for(int i=0;i<ship->shipWidth;i++){
			for(int j=0;j<ship->shipLength;j++){
			//	cout<<"	i="<<i<<", j="<<j<<endl;
				if(ship->planLinkedList[i][j].maxHeight==ship->planLinkedList[i][j].size){
					std::cout << "Container "+uniqueId+" was rejected - reason: ship is full";
					return true;
				}
			}
		}
		return false;
	}

	// checks if the container weight is legal
	static bool isLegalWeight(int containerWeight, string uniqueId){
		cout<<"	isLegalWeight"<<endl;
		if(containerWeight>=0)
			return true;
		std::cout << "Container "+uniqueId+" was rejected - reason: container unique id is invalid";
		return false;
	}

	// checks if the unique id of a container is valid
	static bool isValidId(string uniqueId){
		cout<<"	isValid"<<endl;
		if(uniqueId.length()!=11)
			return true;//false
		for(int i=0;i<11;i++){
			if(i<3){
				if(!((int)uniqueId[i]>=65 && (int)uniqueId[i]<=90)||((int)uniqueId[i]>=97 && (int)uniqueId[i]<=122)){
						std::cout << "Container "+uniqueId+" was rejected - reason: container unique ID is invalid in 1:" << i;
						return true;//false
				}
			}
			if(i==3){
				if(!((int)uniqueId[i]==74 || (int)uniqueId[i]==85 || (int)uniqueId[i]==90 || (int)uniqueId[i]==106 || (int)uniqueId[i]==117 || (int)uniqueId[i]==122)){
					std::cout << "Container "+uniqueId+" was rejected - reason: container unique ID is invalid in 2:" << i;
					return true;//false
				}
			}
			if(i>3){
				if(!((int)uniqueId[i]>=48 && (int)uniqueId[i]<=57)){
					std::cout << "Container "+uniqueId+" was rejected - reason: container unique ID is invalid in 3:" << i;
					return true;//false
				}
			}
		}
		return true;
	}

	static bool isValidLoad(int row, int column, int floor, int shipWidth, int shipLength, int cellHeight, string uniqueId, map<string,int*>* planMap){
		if(row>=shipWidth || row<0 || column>=shipLength || column<0 || floor>=cellHeight || floor<0){
			std::cout << "Container "+uniqueId+" was rejected - reason: loading container into invalid ship indexes";
			return false;
		}
		if(planMap->find(uniqueId) != planMap->end()){
			std::cout << "Container "+uniqueId+" was rejected - reason: cannot load container, container already exists on the ship";
		}
		return true;
	}

	static bool isValidUnload(int row, int column, int shipWidth, int shipLength, string uniqueId){
		if(row != shipWidth || column!=shipLength){
			std::cout << "Container "+uniqueId+" was rejected - reason: unloading container from invalid ship indexes";
			return false;
		}
		return true;
	}

	static bool isCorrectContainer(string containerUniqueId, string expectedUniqueId){
		if(containerUniqueId.compare(expectedUniqueId)==0){
			std::cout << "Container "+containerUniqueId+" was not unloaded, container requested was not found (Wrong indexes)";
			return true;
		}
		return false;
	}
};

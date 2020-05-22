#include "Ship.cpp"

class CraneTester{
public:
// checks if the ship is full
	static int isFull(Ship* ship){
		for(int i=0;i<ship->shipWidth;i++){
			for(int j=0;j<ship->shipLength;j++){
				if(!(ship->planLinkedList[i][j].maxHeight==ship->planLinkedList[i][j].size)){
					return 0;
				}
			}
		}
		return 17;
	}

	// checks if the container weight is legal
	static int isLegalWeight(int containerWeight){
		if(containerWeight>=0)
			return 0;
		return 0;
	}

	// checks if the unique id of a container is valid
	static int isValidId(std::string uniqueId){
		if(uniqueId.length()!=11)
			return 15;
		for(int i=0;i<11;i++){
			if(i<3){
				if(!((int)uniqueId[i]>=65 && (int)uniqueId[i]<=90)||((int)uniqueId[i]>=97 && (int)uniqueId[i]<=122)){
					return 15;
				}
			}
			if(i==3){
				if(!((int)uniqueId[i]==74 || (int)uniqueId[i]==85 || (int)uniqueId[i]==90 || (int)uniqueId[i]==106 || (int)uniqueId[i]==117 || (int)uniqueId[i]==122)){
					return 15;
				}
			}
			if(i>3){
				if(!((int)uniqueId[i]>=48 && (int)uniqueId[i]<=57)){
					return 15;
				}
			}
		}
		return 0;
	}

	static int isValidLoad(int row, int column, int floor, int shipWidth, int shipLength, int cellHeight, std::map<std::string,int*>* planMap, std::string uniqueId){
		if(floor>=cellHeight || floor<0){
			return 0;
		}
		if(row>=shipWidth || row<0 || column>=shipLength || column<0){
			return 1;
		}
		if(planMap->find(uniqueId) != planMap->end()){
			return 11;
		}
		return 0;
	}

	static int isValidUnload(int row, int column, int shipWidth, int shipLength){
		if(row != shipWidth || column!=shipLength){
			return -1;
		}
		return 0;
	}

	static int isCorrectContainer(std::string containerUniqueId,std::string expectedUniqueId){
		if(containerUniqueId.compare(expectedUniqueId)==0){
			return -1;
		}
		return 0;
	}
	
	static int isDuplicateIdOnShip(std::map<std::string,int*>* planMap, std::string uniqueId){
		if(planMap->find(uniqueId) != planMap->end()){
			return 10;
		}
		return 0;
	}
	static int lastPortHasContainers(int size){
		if(size != 0 ){
			return 18;
		}
		return 0;
	}
	static int priority(Container *PortInstructions, int numOfInstructions, int shipMaxCapacity, int shipSize){
		int shipCapacityStatus = shipSize/shipMaxCapacity;
		if(numOfInstructions > shipCapacityStatus){
			for(int i=0;i<numOfInstructions;i++){
				/*
				binarySort( destPort's index );
				reject the rest;
				*/
			}
			//std::sort(arr, numOfInstructions);
		}
		return 0;
	}

};

#include "Ship.cpp"
#include <math.h>

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
		return pow(2,18);
	}

	// checks if the container weight is legal
	static int isLegalWeight(int containerWeight){
		if(containerWeight>=0)
			return 0;
		return pow(2,12);
	}
	
	static int charToIntISO(char c, int index){
		int erase =0;
		if(c=='a' || c=='A'){
			erase=55;
		}else if((int)c>=int('B') && (int)c<=int('K') ||(int)c>=int('b') && (int)c<=int('k')){
			erase=54;	
		}else if((int)c>=int('L') && (int)c<=int('U')||(int)c>=int('l') && (int)c<=int('u')){
			erase=53;
		}else if((int)c>=int('V') && (int)c<=int('Z')||(int)c>=int('v') && (int)c<=int('z')){
			erase=52;
		}

		if(index<4){
				if((int)c>=int('A') && (int)c<=int('Z')){
					return ((int)c-erase)*pow(2,index);
				}else{
					return ((int)c-erase-32)*pow(2,index);

				}
		
		}else{
			return ((int)c-48)*pow(2,index);
		}
	
	}
	// checks if the unique id of a container is valid
	static int isValidId(std::string uniqueId){
		if(uniqueId.length()!=11)
			return pow(2,15);
		for(int i=0;i<10;i++){
			if(i<3){
				if(!(((int)uniqueId[i]>=65 && (int)uniqueId[i]<=90)|| ((int)uniqueId[i]>=97 && (int)uniqueId[i]<=122))){
					//cout <<" and the char is "<< (int)uniqueId[10]-48<<endl;
					return pow(2,15);
				}
			}
			if(i==3){
				if(!((int)uniqueId[i]==74 || (int)uniqueId[i]==85 || (int)uniqueId[i]==90 || (int)uniqueId[i]==106 || (int)uniqueId[i]==117 || (int)uniqueId[i]==122)){
					//cout <<" and the char is "<< (int)uniqueId[10]-48<<endl;
					return pow(2,15);
				}
			}
			if(i>3){
				if(!((int)uniqueId[i]>=48 && (int)uniqueId[i]<=57)){
					cout <<" the char is "<< (int)uniqueId[10]-48<<endl;
					return pow(2,15);
				}
			}
		}
		int sum=0;
		for(int i=0;i<10;i++){
		sum+=charToIntISO(uniqueId[i], i);
		//cout <<" and the char is "<< (int)uniqueId[10]-48<<endl;
		}
		
		int checkNumber= sum - int(sum/11) * 11;
		if(checkNumber==(int)uniqueId[10]-48){
			return 0;
		}
		return pow(2,15);
	}

	static int isValidLoad(int row, int column, int floor, int shipWidth, int shipLength, int cellHeight, std::map<std::string,int*>* planMap, std::string uniqueId){
		if(floor>=cellHeight || floor<0){
			return pow(2,1);
		}
		if(row>=shipWidth || row<0 || column>=shipLength || column<0){
			return pow(2,1);
		}
		if(planMap->find(uniqueId) != planMap->end()){
			return pow(2,11);
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
			return pow(2,10);
		}
		return 0;
	}
	static int lastPortHasContainers(int size){
		if(size != 0 ){
			return pow(2,18);
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

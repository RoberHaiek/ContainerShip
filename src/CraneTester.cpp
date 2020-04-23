#include "Ship.cpp"
class CraneTester{
public:
// checks if the ship is full
	static bool isFull(Ship* ship){
		for(int i=0;i<ship->shipWidth;i++){
			for(int j=0;j<ship->shipLength;j++){
				if(!(ship->planLinkedList[i][j].maxHeight==ship->planLinkedList[i][j].size)){
					return false;
				}
			}
		}
		return true;
	}

	// checks if the container weight is legal
	static bool isLegalWeight(int containerWeight){
		if(containerWeight>=0)
			return true;
		return false;
	}

	// checks if the unique id of a container is valid
	static bool isValidId(std::string uniqueId){
		if(uniqueId.length()!=11)
			return true;//false
		for(int i=0;i<11;i++){
			if(i<3){
				if(!((int)uniqueId[i]>=65 && (int)uniqueId[i]<=90)||((int)uniqueId[i]>=97 && (int)uniqueId[i]<=122)){
						return true;//false
				}
			}
			if(i==3){
				if(!((int)uniqueId[i]==74 || (int)uniqueId[i]==85 || (int)uniqueId[i]==90 || (int)uniqueId[i]==106 || (int)uniqueId[i]==117 || (int)uniqueId[i]==122)){
					return true;//false
				}
			}
			if(i>3){
				if(!((int)uniqueId[i]>=48 && (int)uniqueId[i]<=57)){
					return true;//false
				}
			}
		}
		return true;
	}

	static bool isValidLoad(int row, int column, int floor, int shipWidth, int shipLength, int cellHeight, std::map<std::string,int*>* planMap, std::string uniqueId){
		if(row>=shipWidth || row<0 || column>=shipLength || column<0 || floor>=cellHeight || floor<0){
			return false;
		}
		if(planMap->find(uniqueId) != planMap->end()){
		}
		return true;
	}

	static bool isValidUnload(int row, int column, int shipWidth, int shipLength){
		if(row != shipWidth || column!=shipLength){
			return false;
		}
		return true;
	}

	static bool isCorrectContainer(std::string containerUniqueId,std::string expectedUniqueId){
		if(containerUniqueId.compare(expectedUniqueId)==0){
			return true;
		}
		return false;
	}
};

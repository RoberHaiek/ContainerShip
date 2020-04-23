#include "Crane.cpp"

class StowageTester{
public:
	static int sizeOfArray(Port* array){
			int c=0;
			while(array[c].toString()!="last")
				c++;
			return c;
	}

	// checks if a container's destination port is in route
	static bool isInRoute(std::string destPort, Port* route,int routeIndex){
		int sizeArray=sizeOfArray(route);
		if(routeIndex==sizeArray-1){
			return false;
		}
		if(destPort==route[routeIndex].toString()){
				return false;
			}
		
		for(int i=routeIndex+1;i<sizeArray;i++){
			if(destPort==route[i].toString()){
				return true;
			}
		}
		return false;
	}

};

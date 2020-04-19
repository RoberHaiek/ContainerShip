/*
 * Tester.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: rober.haiek
 */
#include "Crane.cpp"

class StowageTester{
public:
	static int sizeOfArray(Port* array){
			int c=0;
			while(array[c].toString()!="last")
				c++;
			return 0;
	}

	// checks if a container's destination port is in route
	static bool isInRoute(string destPort, Port* route, string uniqueId){
		for(int i=0;i<sizeOfArray(route);i++){
			if(destPort==route[i].toString()){
				return true;
			}
		}
		std::cout << "Container "+uniqueId+" was rejected - reason: destination port is not in route";
		return false;
	}

};

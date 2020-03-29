/*
 * Container.cpp
 *
 *  Created on: 29 Mar 2020
 *      Author: rober.haiek
 */

#include "Ship.cpp"
#include "Port.cpp"

class Container {
public:
	int weight;
	Port destPort;
	string uniqueId;

	Container(int weight, Port destPort, string uniqueId){
		this->weight=weight;
		this->destPort=destPort;
		this->uniqueId=uniqueId;
	}

	Container(){
		this->weight=0;
		this->destPort=NULL;
		this->uniqueId=0;
	}

	void load(Ship ship) {

	}

	void unload(Ship ship) {

	}
};

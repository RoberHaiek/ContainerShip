#include "Port.cpp"
#include <iostream>

class Container {
public:
	int weight;
	Port destPort;
	std::string uniqueId;	// 11 digits

	Container(int weight, Port destPort, std::string uniqueId):destPort(destPort){
		this->weight=weight;
		this->uniqueId=uniqueId;
	}

	Container(){
		this->weight = 0;
		this->uniqueId = "";
		this->destPort=Port();
	}

};

/*
 * Port.cpp
 *
 *  Created on: 29 Mar 2020
 *      Author: rober.haiek
 */

#include <iostream>
#include <list>
#include "Container.cpp"
using namespace std;

class Port {
public:
	string port="emptyString";	// 5 capital English letters
	list<Container> tempContainers;

	Port(string port){
		this->port=port;
		this->tempContainers=NULL;
	}

	string toString(){
		return port;
	}
};

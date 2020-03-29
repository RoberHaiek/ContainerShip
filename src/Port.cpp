/*
 * Port.cpp
 *
 *  Created on: 29 Mar 2020
 *      Author: rober.haiek
 */

#include <iostream>
#include "Container.cpp"
using namespace std;

class Port {
public:
	string port="emptyString";	// 5 capital English letters

	Port(string port){
		this->port=port;
	}

	string toString(){
		return port;
	}
};

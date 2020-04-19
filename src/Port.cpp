/*
 * Port.cpp
 *
 *  Created on: 29 Mar 2020
 *      Author: rober.haiek
 */

#include <iostream>
#include <list>
using namespace std;

class Port {
public:
	string port;	// 5 capital English letters

	Port(string port){
		this->port=port;
	}

	Port(){
		this->port="";
	}

	string toString(){
		return port;
	}
};

#include <iostream>
#include <list>
#include "ErrorParsing.cpp"
class Port {
public:
	std::string port;	// 5 capital English letters

	Port(std::string port){
		this->port=port;
	}

	Port(){
		this->port="";
	}

	std::string toString(){
		return port;
	}
};

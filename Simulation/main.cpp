#include <iostream>
#include <stdlib.h>
#include "AlgorithmRegistrar.h"

int main(){

    	auto& registrar = AlgorithmRegistrar::getInstance();
    	{
        	std::string error;
	    	if (!registrar.loadAlgorithmFromFile("../Algorithms/_205962657_a.so", error)) {
	        	std::cerr << error << '\n'; 
            		return EXIT_FAILURE;
        	}
	    	if (!registrar.loadAlgorithmFromFile("../Algorithms/_205962657_b.so", error)) {
	        	std::cerr << error << '\n'; 
            		return EXIT_FAILURE;
        	}

    	}
    
    	for (auto algo_iter = registrar.begin(); algo_iter != registrar.end(); ++algo_iter) {	
		auto algo = (*algo_iter)();
		std::cout << algo->readShipPlan("hhhh")<<" DONE!!!! \n";
	}	
    	return EXIT_SUCCESS;
/*
	// init route
	Port* route = new Port[4];
	Port port0, port1, port3;
	port0 = Port("TEST1");
	port1 = Port("TEST2");
	port3 = Port("last");
	route[0] = port0;
	route[1] = port1;
	route[2] = port0;
	route[3] = port3;
	// init instructions
	Container** instructions;
	instructions = new Container*[3];
	instructions[0] = new Container[3];
	instructions[1] = new Container[3];
	instructions[2] = new Container[2];
	instructions[0][0] = Container(0,port1,"ABCJ1111110");
	instructions[0][1] = Container(1,port1,"ABCJ1111111");
	instructions[0][2] = Container(0,port1,"last");
	instructions[1][0] = Container(2,port0,"ABCJ1111112");
	instructions[1][1] = Container(3,port0,"ABCJ1111113");
	instructions[1][2] = Container(0,port1,"last");
	instructions[2][0] = Container(4,port1,"test");
	instructions[2][1] = Container(0,port1,"last");

	// init ship
	Ship *ship  = new Ship(3,3,3);

	// running stowage a
	//std::cout << "Running a";
	//_205962657_a a = _205962657_a(0,ship,route,instructions[0]);

	// running stowage b
	std::cout << "Running b";
	_205962657_b b = _205962657_b(0, ship, route, instructions[0]);

	std::cout << "done";

	//for(int i=0;i<3;i++){
	//	a = Stowage(i,*ship,route,instructions[i]);
	//}
	return 0;
*/
}
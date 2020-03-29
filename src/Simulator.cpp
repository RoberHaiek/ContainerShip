/*
 * Simulator.cpp
 *
 *  Created on: Mar 29, 2020
 *      Author: n_b_a
 */

#include <iostream>
#include "Ship.cpp" // i'll check it later
#include "Ship.h"	// i'll check it later

int main() {
	Port *route;
	int numOfPorts;
	Ship ship;
	int currentPort = 0;
	/**TEST THE CIN AND COUT**/
	cout << "Enter routeNum" << endl; // prints !!!Hello World!!!
	cin >> numOfPorts;
	route =new Port[numOfPorts];

	for(int i=0; i<numOfPorts; i++){
		cin >> route[i].port;
	}
	for(int j=0; j<numOfPorts; j++){
			cout << route[j].port <<endl;
		}

/*	ship = new Ship(route);
	while (route[currentPort] != NULL) {
		stowage(ship, route[currentPort]);
	}*/
	return 0;
}

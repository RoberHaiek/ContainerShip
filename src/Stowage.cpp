/*
 * Sowage.cpp
 *
 *  Created on: 15 Apr 2020
 *      Author: rober.haiek
 */

#include "Tester.cpp"
#include <cctype>


class Stowage{
public:
	Ship ship;
	Port *route;					// array of ports
	string** currentInstructions;

	// TO DO:
	//	void getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name)

	bool weightBalance() {
		return true;	// we have a magical ship
	}

	int sizeOfArray(Container* array){
		int c=0;
		while(true){
			if(array[c].uniqueId=="last")
				return c;
			c++;
		}
		return 0;
	}

	void fillInstructions(int instNum, string uniqueId, string LUR, string row, string column, string isTemp){
		string currentInstructions[instNum][5];
		currentInstructions[instNum][0]=uniqueId;
		currentInstructions[instNum][1]=LUR;
		currentInstructions[instNum][2]=row;
		currentInstructions[instNum][3]=column;
		currentInstructions[instNum][4]=isTemp;
	}
	// the logic for unloading the containers to a port
	void unloadingAlgo(int instNum, int i){
		bool popAllAbove; // true if we're popping a container from a stack and we need to pop all above it
		struct node currentContainer;
		Crane crane = Crane(this->ship);
		for(int row=0;row<ship.shipWidth;row++){
			for(int column=0;column<ship.shipLength;column++){
				popAllAbove=false;
				currentContainer=ship.planLinkedList[row][column].linkedList;
				for(int c=0;c<ship.planLinkedList[row][column].size;c++){		// starting from the bottom !!!
					if(currentContainer.container.destPort.toString() == route[i].toString()){	// does ship container belong to ship port?
						crane.unload(currentContainer.container,row,column,false);
						fillInstructions(instNum,currentContainer.container.uniqueId,"unload",to_string(row),to_string(column),"false");
						instNum++;
						popAllAbove=true;
					}
					else{	// ship container does NOT belong to ship port
						if(popAllAbove){	// but should I put it in temp?
							crane.unload(currentContainer.container,row,column,true);
							fillInstructions(instNum,currentContainer.container.uniqueId,"unload",to_string(row),to_string(column),"true");
							instNum++;
						}
					}
					currentContainer=*currentContainer.next;
				}
				// loading containers from temp back to ship
				while(!ship.tempContainers.empty()){
					currentContainer.container=ship.tempContainers.front();
					ship.tempContainers.pop();
					crane.load(currentContainer.container,row,column);
					fillInstructions(instNum,currentContainer.container.uniqueId,"load",to_string(row),to_string(column),"true");
					instNum++;
				}
			}
		}
	}

	// the logic for loading the containers from a port
	void loadingAlgo(int instNum, Container* PortInstructions, bool (*weightBalance)()){
		bool breakIt=false;	  // move to the next container from the port instructions list
		struct node currentContainer;
		Crane crane = Crane(this->ship);
		for(int p=0;p<sizeOfArray(PortInstructions);p++){
			currentContainer.container=PortInstructions[p];
			if(!isRejected(instNum, currentContainer)){
				for(int row=0;row<ship.shipWidth;row++){
					for(int column=0;column<ship.shipLength;column++){
						if(ship.planLinkedList[row][column].size<=ship.planLinkedList[row][column].maxHeight && weightBalance()){		// check if we are below height limit and balanced
							crane.load(currentContainer.container,row,column);
							fillInstructions(instNum,currentContainer.container.uniqueId,"load",to_string(row),to_string(column),"false");
							breakIt = true;
							break;
						}
					}
					if(breakIt){
						breakIt = false;
						break;
					}
				}
			}
		}
	}

	// rejection test
	bool isRejected(int instNum, node currentContainer){
		if(!Tester::isInRoute(currentContainer.container.destPort.toString(),this->route)){
			fillInstructions(instNum,currentContainer.container.uniqueId,"reject","-1","-1","false");
			std::cout << "Container "+currentContainer.container.uniqueId+" was rejected - reason: destination not in route";
			return true;
		}
		if(Tester::isFull(this->ship)){
			fillInstructions(instNum,currentContainer.container.uniqueId,"reject","-1","-1","false");
			std::cout << "Container "+currentContainer.container.uniqueId+" was rejected - reason: ship is full";
			return true;
		}
		if(!Tester::isValidId(currentContainer.container.uniqueId)){
			fillInstructions(instNum,currentContainer.container.uniqueId,"reject","-1","-1","false");
			std::cout << "Container "+currentContainer.container.uniqueId+" was rejected - reason: container unique id is invalid";
			return true;
		}
		if(!Tester::isLegalWeight(currentContainer.container.weight)){
			fillInstructions(instNum,currentContainer.container.uniqueId,"reject","-1","-1","false");
			std::cout << "Container "+currentContainer.container.uniqueId+" was rejected - reason: container weight is invalid";
			return true;
		}
		return false;
	}

	bool isValidInput(int i, Ship ship, Port *route){
		return true;
	}

	/*/
	//	LETS SAIL!!
	//
	//	This function takes the index of the current port in the list of routes as the parameter i (first port: i=0)
	//	and the ship
	//	returns a list of instructions as following:
	//	{a container's unique id, "load/unload/reject", row, column, and a boolean representing whether it's loaded\unloaded to\from a temporary storage}
	/*/
	Stowage(int i, Ship ship, Port *route, bool (*weightBalance)(), Container** instructions) {
		if(isValidInput(i,ship,route)){
			this->currentInstructions = new string*[5];
			int instNum = 0;	// The instruction number of the returned instruction
			this->ship=ship;
			this->route=route;
			Container* PortInstructions;
			PortInstructions = instructions[i];
			unloadingAlgo(instNum,i);
			loadingAlgo(instNum,PortInstructions, weightBalance);
		}
	}

	int main(int argc, char *argv[]) {

		// init route
		Port* route = new Port[3];
		Port port0, port1;
		port0 = Port("TEST1");
		port1 = Port("TEST2");
		route[0] = port0;
		route[1] = port1;
		route[2] = port0;

		// init instructions
		Container** instructions;
		instructions = new Container*[3];
		instructions[0] = new Container[2];
		instructions[1] = new Container[2];
		instructions[2] = new Container[1];
		instructions[0][0] = Container(0,port1,"ABCJ1111110");
		instructions[0][1] = Container(1,port1,"ABCJ1111111");
		instructions[1][0] = Container(2,port0,"ABCJ1111112");
		instructions[1][1] = Container(3,port0,"ABCJ1111113");
		instructions[2][0] = Container(4,port1,"test");

		// init ship
		Ship ship;
		ship = Ship(10,10,10);

		// running stowage
		Stowage result = Stowage(0,ship,route,weightBalance,instructions);
	}
};

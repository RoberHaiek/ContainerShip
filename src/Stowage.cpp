	/*
 * Sowage.cpp
 *
 *  Created on: 15 Apr 2020
 *      Author: rober.haiek
 */

#include "StowageTester.cpp"
#include <cctype>
#include <queue>
#include<sstream>  

class Stowage{
public:
	int instNum;
	Ship ship;
	Port *route;					// array of ports
	string** currentInstructions;
	queue<Container> tempContainers;

	// TO DO:
	//	void getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name)

	static bool weightBalance() {
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

	void fillInstructions(string uniqueId, string LUR, string row, string column, string height){
		currentInstructions[instNum] = new string[5];
		currentInstructions[instNum][0]=uniqueId;
		currentInstructions[instNum][1]=LUR;
		currentInstructions[instNum][2]=row;
		currentInstructions[instNum][3]=column;
		currentInstructions[instNum][4]=height;
	}
	// the logic for unloading the containers to a port
	void unloadingAlgo(int i){
		bool popAllAbove; // true if we're popping a container from a stack and we need to pop all above it
		stringstream rowStream,columnStream,sizeStream;
		string rowStr,columnStr,sizeStr;
		struct node currentContainer;
		Crane crane = Crane(this->ship);
		for(int row=0;row<ship.shipWidth;row++){
			for(int column=0;column<ship.shipLength;column++){
				rowStream<<row;
				columnStream<<column;
				sizeStream<<(this->ship.planLinkedList[row][column].size+1);
				rowStream>>rowStr;
				columnStream>>columnStr;
				sizeStream>>sizeStr;
				popAllAbove=false;
				currentContainer=ship.planLinkedList[row][column].linkedList;
				for(int c=0;c<ship.planLinkedList[row][column].size;c++){		// starting from the bottom !!!
					if(currentContainer.container.destPort.toString() == route[i].toString()){	// does ship container belong to ship port?
						int* dimensions = ship.planMap.find(currentContainer.container.uniqueId)->second;
						if(CraneTester::isValidUnload(row,column,ship.planLinkedList[row][column].size,dimensions[0],dimensions[1],dimensions[2],currentContainer.container.uniqueId)){
							crane.unload(currentContainer.container,row,column,this->ship.planLinkedList[row][column].size);
							fillInstructions(currentContainer.container.uniqueId,"unload",rowStr,columnStr,sizeStr);
							instNum++;
							popAllAbove=true;
						}
					}
					else{	// ship container does NOT belong to ship port
						if(popAllAbove){	// but should I put it in temp?
							int* dimensions = ship.planMap.find(currentContainer.container.uniqueId)->second;
							if(CraneTester::isValidUnload(row,column,ship.planLinkedList[row][column].size,dimensions[0],dimensions[1],dimensions[2],currentContainer.container.uniqueId)){
								crane.unload(currentContainer.container,row,column,this->ship.planLinkedList[row][column].size);
								tempContainers.push(currentContainer.container);
								fillInstructions(currentContainer.container.uniqueId,"unload",rowStr,columnStr,sizeStr);
								instNum++;
							}
						}
					}
					currentContainer=*currentContainer.next;
				}
				// loading containers from temp back to ship
				while(!tempContainers.empty()){
					currentContainer.container=tempContainers.front();
					tempContainers.pop();
					if(CraneTester::isValidUnload(row,column,this->ship.planLinkedList[row][column].size,ship.shipWidth,ship.shipLength,this->ship.planLinkedList[row][column].maxHeight,currentContainer.container.uniqueId)){
						crane.load(currentContainer.container,row,column,this->ship.planLinkedList[row][column].size);
						fillInstructions(currentContainer.container.uniqueId,"load",rowStr,columnStr,sizeStr);
						instNum++;
					}
				}
			}
		}
	}

	// the logic for loading the containers from a port
	void loadingAlgo(Container* PortInstructions, bool (*weightBalance)()){
		bool breakIt=false;	  // move to the next container from the port instructions list
		struct node currentContainer;
		stringstream rowStream,columnStream,sizeStream;
		string rowStr,columnStr,sizeStr;
		Crane crane = Crane(this->ship);
		for(int p=0;p<sizeOfArray(PortInstructions);p++){
			currentContainer.container=PortInstructions[p];
			if(!isRejected(currentContainer)){
				for(int row=0;row<ship.shipWidth;row++){
					for(int column=0;column<ship.shipLength;column++){
						rowStream<<row;
						columnStream<<column;
						sizeStream<<(this->ship.planLinkedList[row][column].size+1);
						rowStream>>rowStr;
						sizeStream>>sizeStr;
						columnStream>>columnStr;
						if(ship.planLinkedList[row][column].size<=ship.planLinkedList[row][column].maxHeight && weightBalance()){		// check if we are below height limit and balanced
							if(CraneTester::isValidLoad(row,column,this->ship.planLinkedList[row][column].size,ship.shipWidth,ship.shipLength,this->ship.planLinkedList[row][column].maxHeight,currentContainer.container.uniqueId,ship.planMap)){
								crane.load(currentContainer.container,row,column,this->ship.planLinkedList[row][column].size);
								fillInstructions(currentContainer.container.uniqueId,"load",rowStr,columnStr,sizeStr);
								breakIt = true;
								break;
							}
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
	bool isRejected(node currentContainer){
		if(!StowageTester::isInRoute(currentContainer.container.destPort.toString(),this->route,currentContainer.container.uniqueId)
			|| CraneTester::isFull(this->ship,currentContainer.container.uniqueId)
				|| !CraneTester::isValidId(currentContainer.container.uniqueId)
					|| !CraneTester::isLegalWeight(currentContainer.container.weight,currentContainer.container.uniqueId)){
			fillInstructions(currentContainer.container.uniqueId,"reject","-1","-1","-1");
			return true;
		}
		return false;
	}

	/*/
	//	LETS SAIL!!
	//
	//	This function takes the index of the current port in the list of routes as the parameter i (first port: i=0)
	//	and the ship
	//	returns a list of instructions as following:
	//	{a container's unique id, "load/unload/reject", row, column, height, and a boolean representing whether it's loaded\unloaded to\from a temporary storage}
	/*/
	Stowage(int i, Ship ship, Port *route, Container* instructions) {
			this->instNum = 0;	// The instruction number of the returned instruction
			this->currentInstructions = new string*[50];
			this->ship=ship;
			this->route=route;
			this->tempContainers = queue<Container>();
			unloadingAlgo(i);
			loadingAlgo(instructions, weightBalance);
			fillInstructions("last","","","","");
	//	}
	}

	/* int main() {

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
		// Stowage result = Stowage(0,ship,route,weightBalance,instructions);
		return 0;
	} */
};

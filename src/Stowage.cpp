/*
 * Sowage.cpp
 *
 *  Created on: 15 Apr 2020
 *      Author: rober.haiek
 */

#include "Ship.cpp"
#include "Port.cpp"
#include "Tester.cpp"
#include <cctype>


class Stowage{
public:
	Ship ship;
	Port *route;					// array of ports
	string** currentInstructions;
	struct node
	{
	    Container container;
	    struct node *next;
	};

	// TO DO:
	//	void getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name)

	bool weightBalance() {
		return true;	// we have a magical ship
	}

	void setHeight(int row, int column, int maxHeight){
		this->ship.planLinkedList[row][column].maxHeight=maxHeight;
	}

	int sizeOfArray(Container* array){
		int c=0;
		while(true){
			if(array[c]==NULL)
				return c;
			c++;
		}
		return 0;
	}

	// unload a single container from a specific location
	void unload(Container container, int row, int column, bool isTemp) {
		node *temp,newNode;
		newNode = new node;
		newNode.container=container;
		newNode.next=NULL;
		temp=this->ship.planLinkedList[row][column].linkedList;
		while((*temp).next->container.uniqueId!=container.uniqueId && (*temp).next!=NULL){
			temp=(*temp).next;
		}
		(*temp).next=NULL;
		this->ship.planMap.erase(container.uniqueId);
		if(isTemp){
			this->ship.tempContainers.push(container);
		}
		this->ship.planLinkedList[row][column].size--;
	}

	// load a single container to a specific location
	void load(Container container,int row, int column) {
		this->ship.planMap.insert(container.uniqueId,{row,column});	// Adding container to the map
		node *temp,newNode;										// Adding container to linked list
		newNode = new node;
		newNode.container=container;
		newNode.next=NULL;
		temp=this->ship.planLinkedList[row][column].linkedList;
		while((*temp).next!=NULL){
			temp=(*temp).next;
		}
		(*temp).next=newNode;
		this->ship.planLinkedList[row][column].size++;
	}

	// the logic for unloading the containers to a port
	void unloadingAlgo(int instNum, int i){
		bool popAllAbove; // true if we're popping a container from a stack and we need to pop all above it
		node currentContainer;
		for(int row=0;row<ship.shipWidth;row++){
			for(int column=0;column<ship.shipLength;column++){
				popAllAbove=false;
				currentContainer=ship.planLinkedList[row][column].linkedList;
				while(currentContainer!=NULL){		// starting from the bottom !!!
					if(currentContainer.container.destPort == route[i].toString()){	// does ship container belong to ship port?
						unload(currentContainer.container,row,column,false);
						currentInstructions[instNum] = {currentContainer.container.uniqueId,"unload",row,column,"false"};
						instNum++;
						popAllAbove=true;
					}
					else{	// ship container does NOT belong to ship port
						if(popAllAbove){	// but should I put it in temp?
							unload(currentContainer.container,row,column,true);
							currentInstructions[instNum] = {currentContainer.container.uniqueId,"unload",row,column,"true"};
							instNum++;
						}
					}
					currentContainer=currentContainer.next;
				}
				// loading containers from temp back to ship
				while(!ship.tempContainers.empty()){
					currentContainer=ship.tempContainers.pop();
					load(currentContainer.container,row,column);
					currentInstructions[instNum]={currentContainer.container.uniqueId,"load",row,column,"true"};
					instNum++;
				}
			}
		}
	}

	// the logic for loading the containers from a port
	void loadingAlgo(int instNum, Container* PortInstructions, bool weightBalance()){
		bool breakIt=false;	  // move to the next container from the port instructions list
		node currentContainer;
		for(int p=0;p<sizeOfArray(PortInstructions);p++){
			currentContainer.container=PortInstructions[p];
			if(!isRejected(instNum, currentContainer)){
				for(int row=0;row<ship.shipWidth;row++){
					for(int column=0;column<ship.shipLength;column++){
						if(ship.planLinkedList[row][column].size<=ship.planLinkedList[row][column].maxHeight && weightBalance()){		// check if we are below height limit and balanced
							load(currentContainer.container,row,column);
							currentInstructions[instNum] = {currentContainer.container.uniqueId,"load",row,column,"false"};
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
		Tester test;
		if(!test.isInRoute(currentContainer.container.destPort.toString(),this->route)){
			currentInstructions[instNum] = {currentContainer.container.uniqueId,"reject",-1,-1,"false"};
			std::cout << "Container "+currentContainer.container.uniqueId+" was rejected - reason: destination not in route";
			return true;
		}
		if(test.isFull(this->ship)){
			currentInstructions[instNum] = {currentContainer.container.uniqueId,"reject",-1,-1,"false"};
			std::cout << "Container "+currentContainer.container.uniqueId+" was rejected - reason: ship is full";
			return true;
		}
		if(test.isValidId(currentContainer.container.uniqueId)){
			currentInstructions[instNum] = {currentContainer.container.uniqueId,"reject",-1,-1,"false"};
			std::cout << "Container "+currentContainer.container.uniqueId+" was rejected - reason: ship is full";
			return true;
		}
		return false;
	}

	//
	//	LETS SAIL!!
	//
	//	This function takes the index of the current port in the list of routes as the parameter i (first port: i=0)
	//	and the ship
	//	returns a list of instructions as following:
	//	{a container's unique id, "load/unload/reject", row, column, and a boolean representing whether it's loaded\unloaded to\from a temporary storage}
	//
	string** Stowage(int i, Ship ship, Port *route, bool weightBalance()) {
		int instNum = 0;	// The instruction number of the returned instruction
		this->ship=ship;
		this->route=route;
		Container* PortInstructions;
		PortInstructions = ship.instructions[i];
		unloadingAlgo(instNum,i);
		loadingAlgo(instNum,PortInstructions, weightBalance);
		return this->currentInstructions;
	}
};

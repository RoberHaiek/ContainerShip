/*
 * Sowage.cpp
 *
 *  Created on: 15 Apr 2020
 *      Author: rober.haiek
 */

#include "Ship.cpp"

class Stowage{
public:
	Ship ship;
	string** currentInstructions;
	struct node
	{
	    Container container;
	    struct node *next;
	};

	bool weightBalance() {
		return true;	// we have a magical ship
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

	// the logic for unloading the containers to a port
	void unloadingAlgo(int instNum, int i){
		bool popAllAbove; // true if we're popping a container from a stack and we need to pop all above it
		node currentContainer;
		for(int row=0;row<ship.shipWidth;row++){
			for(int column=0;column<ship.shipLength;column++){
				popAllAbove=false;
				currentContainer=ship.planLinkedList[row][column].linkedList;
				while(currentContainer!=NULL){		// starting from the bottom !!!
					if(currentContainer.container.destPort == ship.route[i].toString()){	// does ship container belong to ship port?
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
	void loadingAlgo(int instNum, Container* PortInstructions){
		bool breakIt=false;	  // move to the next container from the port instructions list
		node currentContainer;
		for(int p=0;p<sizeOfArray(PortInstructions);p++)
			currentContainer.container=PortInstructions[p];
			for(int row=0;row<ship.shipWidth;row++){
				for(int column=0;column<ship.shipLength;column++){
					if(ship.planLinkedList[row][column].size<=ship.planLinkedList[row][column].maxHeight && weightBalance()){		// check if we are below height limit
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

	// LETS SAIL!!
	void Stowage(int i, Ship ship) {
		this->ship=ship;
		int instNum = 0;	// The instruction number of the returned instructions
		Container* PortInstructions;
		PortInstructions = ship.instructions[i];
		unloadingAlgo(instNum,i);
		loadingAlgo(instNum,PortInstructions);
	}
};

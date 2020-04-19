/*
 * Crane.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: rober.haiek
 */

#include "Ship.cpp"

class Crane{
public:
	Ship ship;
	Crane(Ship ship){
		this->ship=ship;
	}
	// unload a single container from a specific location
	void unload(Container container, int row, int column, bool isTemp) {
		struct node *temp,newNode;
		newNode.container=container;
		newNode.next=NULL;
		temp=*ship.planLinkedList[row][column].linkedList;
		while((*temp).next->container.uniqueId!=container.uniqueId && (*temp).next!=NULL){
			temp=(*temp).next;
		}
		(*temp).next=NULL;
		ship.planMap.erase(container.uniqueId);
		if(isTemp){
			ship.tempContainers.push(container);
		}
		ship.planLinkedList[row][column].size--;
	}

	// load a single container to a specific location
	void load(Container container,int row, int column) {
		ship.planMap.insert({container.uniqueId,{row,column}});	// Adding container to the map
		struct node *temp,newNode;										// Adding container to linked list
		newNode.container=container;
		newNode.next=NULL;
		temp=ship.planLinkedList[row][column].linkedList;
		while((*temp).next!=NULL){
			temp=(*temp).next;
		}
		(*temp).next=&newNode;
		ship.planLinkedList[row][column].size++;
	}

};

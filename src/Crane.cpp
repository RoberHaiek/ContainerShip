/*
 * Crane.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: rober.haiek
 */

#include "Ship.cpp"
#include "Container.cpp"

class Crane{
public:
	Ship ship;
	Crane(Ship ship){
		this->ship=ship;
	}
	// each node in the linked list represents a container
	struct node
	{
	    Container container;
	    struct node *next;
	};
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

};

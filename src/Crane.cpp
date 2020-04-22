/*
 * Crane.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: rober.haiek
 */

#include "CraneTester.cpp"

class Crane{
public:
	Ship* ship;
	Crane(Ship* ship):ship(ship){
	}
	// unload a single container from a specific location
	void unload(Container container, int row, int column, int floor) {
		struct node *temp;
		temp =new node();
		temp=ship->planLinkedList[row][column].linkedList;
		for(int i=0;i<floor;i++){
			temp=temp->next;
		}
		if(CraneTester::isCorrectContainer(container.uniqueId,temp->next->container->uniqueId)){
			temp->next=NULL;
			ship->planMap->erase(container.uniqueId);
			ship->planLinkedList[row][column].size--;
		}
	}

	// load a single container to a specific location
	void load(Container* container,int row, int column, int floor) {
		int* rowColumn = new int[3];
		rowColumn[0] = row;
		rowColumn[1] = column;
		rowColumn[2] = floor;
		ship->planMap->insert(pair<string, int*>(container->uniqueId,rowColumn));	// Adding container to the map
		struct node *temp, *newNode;
		temp =new node();
		newNode =new node();
		Port port = Port(container->destPort.toString());
		newNode->container=new Container(container->weight,port,container->uniqueId);
		newNode->next=NULL;
		temp=ship->planLinkedList[row][column].linkedList;
		while(temp->next!=NULL){
			temp=temp->next;
		}
		if(temp->container->uniqueId.compare("TESTING")==0){
		temp->container=newNode->container;
		}else{
			temp->next=newNode;
		}
		ship->planLinkedList[row][column].size++;
	}

};

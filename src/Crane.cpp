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
	node * unload(Container container, int row, int column, int floor) {
		struct node *temp,*prev;
		temp=ship->planLinkedList[row][column].linkedList;
		prev=temp;
		while(temp!=NULL && temp->container->uniqueId.compare(container.uniqueId)!=0 ){
			prev=temp;
			temp=temp->next;
		}
		if(temp==prev){
			if(temp->next==NULL){
			node *newNode =new node();
			ship->planLinkedList[row][column].linkedList=newNode;
			}else{
				ship->planLinkedList[row][column].linkedList=temp->next;
			}
		}else{
			prev->next=temp->next;
		}
			ship->planMap->erase(container.uniqueId);
			ship->planLinkedList[row][column].size--;
		if(floor){}
		return temp;
	}

	// load a single container to a specific location
	void load(Container* container,int row, int column, int floor) {
		int* rowColumn = new int[3];
		rowColumn[0] = row;
		rowColumn[1] = column;
		rowColumn[2] = floor;
		ship->planMap->insert(pair<string, int*>(container->uniqueId,rowColumn));	// Adding container to the map
		struct node *temp, *newNode;
		newNode =new node();
		Port port = Port(container->destPort.toString());
		newNode->container=new Container(container->weight,port,container->uniqueId);
		newNode->next=NULL;
		temp=ship->planLinkedList[row][column].linkedList;
		while(temp!=NULL && temp->next!=NULL){
			temp=temp->next;
		}
		if(temp==NULL){
			ship->planLinkedList[row][column].linkedList=newNode;
		}
		else if(temp->container->uniqueId.compare("TESTING")==0){
	//	temp->container=newNode->container;
		ship->planLinkedList[row][column].linkedList=newNode;//
		}else{
			temp->next=newNode;
		}
		ship->planLinkedList[row][column].size++;
	}

};

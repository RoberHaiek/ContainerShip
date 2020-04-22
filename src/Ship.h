/*
 * ContainerShip.h
 *
 *  Created on: Mar 29, 2020
 *      Author: n_b_a
 */

#ifndef CONTAINERSHIP_H_
#define CONTAINERSHIP_H_

	// each node in the linked list represents a container
	struct node
	{
	    Container* container;
	    struct node *next;
		node(){
			Port p = Port("TEST");
			container =new Container(1,p,"TESTING");
			next = NULL;}
	};
	// each cellLinkedList represents a cell (specific row and column) in which containers are represented as nodes in the linked list, and the size
	struct cellLinkedList
	{
		node* linkedList;
		int size;
		int maxHeight;
		cellLinkedList(){
			linkedList =new node();
			size=0;
			maxHeight=10;
}
	};



#endif /* CONTAINERSHIP_H_ */

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
	    Container container;
	    struct node *next;
	};
	// each cellLinkedList represents a cell (specific row and column) in which containers are represented as nodes in the linked list, and the size
	struct cellLinkedList
	{
		node linkedList;
		int size;
		int maxHeight;
	};



#endif /* CONTAINERSHIP_H_ */

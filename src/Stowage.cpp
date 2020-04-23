/*
 * Sowage.cpp
 *
 *  Created on: 15 Apr 2020
 *      Author: rober.haiek
 */

#include "StowageTester.cpp"
#include <cctype>
#include <queue>
#include <sstream>

class Stowage {
public:
	int instNum;
	Ship *ship;
	Port *route;					// array of ports
	string **currentInstructions;
	deque<node*> tempContainers;
	deque<node*> loadBackContainers;
	int routeIndex;

	void printContainersZeroZero(){
		node* temp=ship->planLinkedList[0][0].linkedList;
		cout<< "--------------------[0][0]---------------------------------"<<endl;

		while(temp!=NULL){
			cout<< temp->container->uniqueId <<" --> ";
			temp=temp->next;
		}
		cout<<" NULL "<<endl;

		cout<< "--------------------[0][0] END---------------------------------"<<endl;
	}

	//parinting
	void printContainers(Container *array) {
		Container *p = array;
		int index = 0;
		std::cout
				<< "--------------------CONTAINERS---------------------------------"
				<< endl;
		while (p->uniqueId.compare("last") != 0) {
			std::cout << "on index " << index << " ID: " << p->uniqueId << endl;

		}
		std::cout
				<< "------------------------END CONTAINERS-----------------------------"
				<< endl;
	}

	void printPlanMap() {
		cellLinkedList **linkedList = ship->planLinkedList;
		node *tmp;
		std::cout
				<< "--------------------printing linked list content---------------------"
				<< endl;
		for (int row = 0; row < ship->shipWidth; row++) {
			std::cout << "-size= " << linkedList[row]->size << " MAX_HIGHET= "
					<< linkedList[row]->maxHeight << endl;
			for (int col = 0; col < ship->shipLength; col++) {
				std::cout << endl << "**" << row << "**" << col << endl;
				std::cout << "	size= " << linkedList[row][col].size
						<< " MAX_HIGHET= " << linkedList[row][col].maxHeight
						<< endl;
				tmp = linkedList[row][col].linkedList;
				bool isNULL = tmp->next == NULL;
				std::cout << "	-ContainerID: " << tmp->container->uniqueId
						<< "  IS_NEXT_NULL=" << isNULL << endl;

			}
		}
		std::cout << "--------------------END---------------------" << endl
				<< endl;
	}
	// TO DO:
	//	void getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name)

	static bool weightBalance() {
		return true;	// we have a magical ship
	}

	int sizeOfArray(Container *array) {
		int c = 0;
		while (true) {
			if (array[c].uniqueId == "last")
				return c;
			c++;
		}
		return 0;
	}

	void fillInstructions(string uniqueId, string LUR, string row,
			string column, string height) {
		std::cout << "filling " << uniqueId << endl;
		currentInstructions[instNum] = new string[5];
		currentInstructions[instNum][0] = uniqueId;
		currentInstructions[instNum][1] = LUR;
		currentInstructions[instNum][2] = row;
		currentInstructions[instNum][3] = column;
		currentInstructions[instNum][4] = height;
		instNum = instNum + 1;
		std::cout << "done filling" << endl;
	}
	// the logic for unloading the containers to a port
	void unloadingAlgo(int i) {
		bool popAllAbove; // true if we're popping a container from a stack and we need to pop all above it
		stringstream rowStream, columnStream, sizeStream;
		string rowStr, columnStr, sizeStr;
		struct node *currentContainer;
		Crane crane = Crane(this->ship);
		for (int row = 0; row < ship->shipWidth; row++) {
			for (int column = 0; column < ship->shipLength; column++) {
				if (this->ship->planLinkedList[row][column].size == 0) {
					continue;
				}
				rowStream << row;
				columnStream << column;
				sizeStream
						<< (this->ship->planLinkedList[row][column].size + 1);
				rowStream >> rowStr;
				columnStream >> columnStr;
				sizeStream >> sizeStr;
				popAllAbove = false;
				cellLinkedList list=ship->planLinkedList[row][column];
				currentContainer =
						list.linkedList;
				//node* c =list.linkedList;
				while(currentContainer !=NULL) {		// starting from the bottom !!!
					cout << " dest port "
							<< currentContainer->container->destPort.toString()
							<< " current port " << route[i].toString() << endl;
					printContainersZeroZero();
					if (currentContainer->container->destPort.toString()
							== route[i].toString()) {// does ship container belong to ship port?
						int *dimensions = ship->planMap->find(
								currentContainer->container->uniqueId)->second;
							if (CraneTester::isValidUnload(row, column,
								dimensions[0], dimensions[1],
								currentContainer->container->uniqueId)) {
									node *temp =
									crane.unload(*(currentContainer->container),
											row, column,
											this->ship->planLinkedList[row][column].size);
							printContainersZeroZero();
							currentContainer = temp->next;

							tempContainers.push_front(temp);
							if(tempContainers.empty()){
								cout<<"tempContainers is empty"<<endl;
							}else{
								cout<<"tempContainers is not empty"<<endl;

							}
							popAllAbove = true;
						}
					} else {	// ship container does NOT belong to ship port
						if (popAllAbove) {	// but should I put it in temp?
							int *dimensions =
									ship->planMap->find(
											currentContainer->container->uniqueId)->second;
							if (CraneTester::isValidUnload(row, column,
									dimensions[0], dimensions[1],
									currentContainer->container->uniqueId)) {
								node *temp =
										crane.unload(
												*(currentContainer->container),
												row, column,
												this->ship->planLinkedList[row][column].size);
								printContainersZeroZero();

								currentContainer = temp->next;
								tempContainers.push_front(temp);

							}
						}
					}
					if(!popAllAbove && currentContainer!=NULL){
					currentContainer =currentContainer->next;}
					

				}
				cout<<"after unloading  "<<endl;
				printContainersZeroZero();
				// loading containers from temp back to ship
				node *popedElem;
				while (!tempContainers.empty()) {
					popedElem = tempContainers.front();
					tempContainers.pop_front();
					string dstPort = popedElem->container->destPort.toString();
					fillInstructions(popedElem->container->uniqueId, "unload",
							rowStr, columnStr, sizeStr);
					if (dstPort.compare(route[i].toString()) == 0) {
						delete popedElem;
					} else {
						loadBackContainers.push_front(popedElem);
					}
				}
				cout<<"done with the tempContainers "<<endl;
				while (!loadBackContainers.empty()) {
					cout<<"in the loadBackContainers"<<endl;
					popedElem = loadBackContainers.back();
					loadBackContainers.pop_back();
					fillInstructions(popedElem->container->uniqueId, "load",
							rowStr, columnStr, sizeStr);
					crane.load(popedElem->container, row, column,
							this->ship->planLinkedList[row][column].size);

				}

			}
		}
	}

// the logic for loading the containers from a port
	void loadingAlgo(Container *PortInstructions, bool (*weightBalance)()) {
		bool breakIt = false;// move to the next container from the port instructions list
		struct node currentContainer;
		stringstream rowStream, columnStream, sizeStream;
		string rowStr, columnStr, sizeStr;
		Crane crane = Crane(this->ship);
		for (int p = 0; p < sizeOfArray(PortInstructions); p++) {
			currentContainer.container = &(PortInstructions[p]);
			if (!isRejected(currentContainer)) {
				for (int row = 0; row < ship->shipWidth; row++) {
					for (int column = 0; column < ship->shipLength; column++) {
						rowStream << row;
						columnStream << column;
						sizeStream
								<< (this->ship->planLinkedList[row][column].size
										+ 1);
						rowStream >> rowStr;
						sizeStream >> sizeStr;
						columnStream >> columnStr;
						if (ship->planLinkedList[row][column].size
								<= ship->planLinkedList[row][column].maxHeight
								&& weightBalance()) {// check if we are below height limit and balanced
							if (CraneTester::isValidLoad(row, column,
									this->ship->planLinkedList[row][column].size,
									ship->shipWidth, ship->shipLength,
									this->ship->planLinkedList[row][column].maxHeight,
									currentContainer.container->uniqueId,
									ship->planMap)) {
								std::cout << "current container ID: "
										<< currentContainer.container->uniqueId
										<< endl;
								crane.load(currentContainer.container, row,
										column,
										this->ship->planLinkedList[row][column].size);
								fillInstructions(
										currentContainer.container->uniqueId,
										"load", rowStr, columnStr, sizeStr);
								breakIt = true;
								break;
							}
						}
					}
					if (breakIt) {
						breakIt = false;
						break;
					}
				}
			}
		}
	}

// rejection test
	bool isRejected(node currentContainer) {
		std::cout << "isRejected" << endl;
		if (!StowageTester::isInRoute(
				currentContainer.container->destPort.toString(), this->route,
				currentContainer.container->uniqueId,routeIndex)
				|| CraneTester::isFull(this->ship,
						currentContainer.container->uniqueId)
				|| !CraneTester::isValidId(currentContainer.container->uniqueId)
				|| !CraneTester::isLegalWeight(
						currentContainer.container->weight,
						currentContainer.container->uniqueId)) {
			fillInstructions(currentContainer.container->uniqueId, "reject",
					"-1", "-1", "-1");
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
	 //	{a container's unique id, "load/unload/reject", row, column, height}
	 /*/
	Stowage(int i, Ship *ship, Port *route, Container *instructions) :
			ship(ship) {
		cout<<"************in port number "<<i<<endl;
		printContainersZeroZero();
		this->instNum = 0;// The instruction number of the returned instruction
		this->currentInstructions = new string*[100];
		routeIndex=i;
		//this->ship=ship;
		this->route = route;
		//this->tempContainers = dequeue<node*>();
		std::cout << "unloadingAlgo " << endl;
		unloadingAlgo(i);
		cout<<"************in port number after unload "<<i<<endl;
		printContainersZeroZero();

		std::cout << "loadingAlgo " << endl;
		loadingAlgo(instructions, weightBalance);
		cout<<"************in port number after load "<<i<<endl;
		printContainersZeroZero();

		std::cout << "filling last instructions" << endl;
		fillInstructions("last", "last", "last", "last", "last");
		
		std::cout << "FINITO!";
		//std::cout << currentInstructions[1][1];
	}
}
;

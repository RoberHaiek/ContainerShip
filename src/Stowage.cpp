#include "StowageTester.cpp"
#include <cctype>
#include <queue>
#include <sstream>

class Stowage {
public:
	int instNum;
	Ship *ship;
	Port *route;					// array of ports
	std::string **currentInstructions;
	std::deque<node*> tempContainers;
	std::deque<node*> loadBackContainers;
	std::deque<std::string*> indexies;
	int routeIndex;

	/*
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

	//printing
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
	}*/

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

	void fillInstructions(std::string uniqueId, std::string LUR, std::string row,
			std::string column, std::string height) {
		currentInstructions[instNum] = new std::string[5];
		currentInstructions[instNum][0] = uniqueId;
		currentInstructions[instNum][1] = LUR;
		currentInstructions[instNum][2] = row;
		currentInstructions[instNum][3] = column;
		currentInstructions[instNum][4] = height;
		instNum = instNum + 1;
	}
	// the logic for unloading the containers to a port
	void unloadingAlgo(int i) {
		bool popAllAbove; // true if we're popping a container from a stack and we need to pop all above it
		struct node *currentContainer;
		std::string* indxes;
		Crane crane = Crane(this->ship);
		for (int row = 0; row < ship->shipWidth; row++) {
			for (int column = 0; column < ship->shipLength; column++) {
				if (this->ship->planLinkedList[row][column].size == 0) {
					continue;
				}
				popAllAbove = false;
				cellLinkedList list=ship->planLinkedList[row][column];
				currentContainer =
						list.linkedList;
				while(currentContainer !=NULL) {		// starting from the bottom !!!
					if (currentContainer->container->destPort.toString()
							== route[i].toString()) {// does ship container belong to ship port?
						int *dimensions = ship->planMap->find(
								currentContainer->container->uniqueId)->second;
							if (CraneTester::isValidUnload(row, column,
								dimensions[0], dimensions[1])) {
									node *temp =
									crane.unload(*(currentContainer->container),
											row, column,
											this->ship->planLinkedList[row][column].size);
							currentContainer = temp->next;
							tempContainers.push_front(temp);
							indxes=new std::string[3];
							indxes[0]=std::to_string(row+1);indxes[1]=std::to_string(column+1);indxes[2]=std::to_string(this->ship->planLinkedList[row][column].size+1);
							indexies.push_front(indxes);
							popAllAbove = true;
						}
					} else {	// ship container does NOT belong to ship port
						if (popAllAbove) {	// but should I put it in temp?
							int *dimensions =
									ship->planMap->find(
											currentContainer->container->uniqueId)->second;
							if (CraneTester::isValidUnload(row, column,
									dimensions[0], dimensions[1])) {
								node *temp =
										crane.unload(
												*(currentContainer->container),
												row, column,
												this->ship->planLinkedList[row][column].size);
								currentContainer = temp->next;
								tempContainers.push_front(temp);
								indxes=new std::string[3];
								indxes[0]=std::to_string(row+1);indxes[1]=std::to_string(column+1);indxes[2]=std::to_string(this->ship->planLinkedList[row][column].size+1);
								indexies.push_front(indxes);


							}
						}
					}
					if(!popAllAbove && currentContainer!=NULL){
					currentContainer =currentContainer->next;}
				}
				
				// loading containers from temp back to ship
				node *popedElem;
				std::string *indx;
				while (!tempContainers.empty()) {
					popedElem = tempContainers.front();
					indx=indexies.front();
					indexies.pop_front();
					tempContainers.pop_front();
					std::string dstPort = popedElem->container->destPort.toString();
					fillInstructions(popedElem->container->uniqueId, "unload",
							indx[0], indx[1], indx[2]);
					if (dstPort.compare(route[i].toString()) == 0) {
						delete popedElem;
						delete[] indx;
					} else {
						loadBackContainers.push_front(popedElem);
						indexies.push_back(indx);
					}
				}
				while (!loadBackContainers.empty()) {
					popedElem = loadBackContainers.back();
					indx=indexies.back();
					indexies.pop_back();
					loadBackContainers.pop_back();
					fillInstructions(popedElem->container->uniqueId, "load",
							indx[0], indx[1], indx[2]);
					crane.load(popedElem->container, row, column,
							this->ship->planLinkedList[row][column].size);
					delete popedElem;
					delete indx;


				}

			}
		}
	}

// the logic for loading the containers from a port
	void loadingAlgo(Container *PortInstructions, bool (*weightBalance)()) {
		bool breakIt = false;// move to the next container from the port instructions list
		struct node currentContainer;
		Crane crane = Crane(this->ship);
		for (int p = 0; p < sizeOfArray(PortInstructions); p++) {
			currentContainer.container = &(PortInstructions[p]);
			if (!isRejected(currentContainer)) {
				for (int row = 0; row < ship->shipWidth; row++) {
					for (int column = 0; column < ship->shipLength; column++) {
						if (ship->planLinkedList[row][column].size
								<= ship->planLinkedList[row][column].maxHeight
								&& weightBalance()) {// check if we are below height limit and balanced
							if (CraneTester::isValidLoad(row, column,
									this->ship->planLinkedList[row][column].size,
									ship->shipWidth, ship->shipLength,
									this->ship->planLinkedList[row][column].maxHeight,
									ship->planMap,currentContainer.container->uniqueId)) {
								crane.load(currentContainer.container, row,
										column,
										this->ship->planLinkedList[row][column].size);
								fillInstructions(
										currentContainer.container->uniqueId,
										"load", std::to_string(row+1), std::to_string(column+1), std::to_string((this->ship->planLinkedList[row][column].size)));
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
		if (!StowageTester::isInRoute(
				currentContainer.container->destPort.toString(), this->route,routeIndex)
				|| CraneTester::isFull(this->ship)
				|| !CraneTester::isValidId(currentContainer.container->uniqueId)
				|| !CraneTester::isLegalWeight(
						currentContainer.container->weight)) {
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
		this->instNum = 0;// The instruction number of the returned instruction
		this->currentInstructions = new std::string*[100];
		routeIndex=i;
		this->route = route;
		unloadingAlgo(i);
		loadingAlgo(instructions, weightBalance);
		fillInstructions("last", "last", "last", "last", "last");
	}
}
;

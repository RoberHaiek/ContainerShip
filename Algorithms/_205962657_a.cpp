#include "../Interfaces/AbstractAlgorithm.h"
#include "../Interfaces/AlgorithmRegistration.h"
#include "../Interfaces/IOHandler.h"
#include <cctype>
#include <iostream>
#include <queue>
#include <sstream>

class _205962657_a: public AbstractAlgorithm{
public:
	int instNum;
	Ship *ship;
	Port *route;					// array of ports
	std::string **currentInstructions;
	std::deque<node*> tempContainers;
	std::deque<node*> loadBackContainers;
	std::deque<std::string*> indexies;
	int routeIndex;
	string name="in algo A";
	/*
	 *
	 *added
	 *
	 */
	//decleration
	//void unloadingAlgo(int i);

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
	
	// ask Aubaida:

	int readShipPlan(const std::string& full_path_and_file_name){
		// read the ship plan from a file
		cout << "*initShipPlan "<<name<<endl;
	  	initShipPlan(ship,full_path_and_file_name);

		return 0; // success
	}

	int readShipRoute(const std::string& full_path_and_file_name){
		// read ship route from file
		cout << "*initRoute from stowage"<<endl;
		char** routeArray;
	  	initRoute(routeArray,full_path_and_file_name);
		route= getPortsFromRoute(routeArray);
		cout<<"* finish route from stowage"<<endl;

		return 0; // success
	}

	int setWeightBalanceCalculator(WeightBalanceCalculator& calculator){
		// what???
		return 0;
	}

	int getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name){
		// read from input, write to output - PARSING
		string fileName=getTheFileName(input_full_path_and_file_name);
		cout<<"111111111111 "<<input_full_path_and_file_name <<"  and the filename is : "<<fileName<<endl;
		
		int error = 0;
		getRouteIndex(routeIndex,fileName);
		cout<<"222222222222"<<endl;
		cout<<"the index in get inst ="<<routeIndex<<endl;
		Container* containers=parseCargoFile(input_full_path_and_file_name);
		cout<<"the first container is :"<<containers[0].uniqueId<<endl;
		error = error | unloadingAlgo(routeIndex);
		error = error | loadingAlgo(containers, weightBalance);
		fillInstructions(Action::REJECT, "last", "last", "last", "last");
		instructionsOut(currentInstructions,output_full_path_and_file_name);
		this->instNum = 0;
		return error;
	}

	void fillInstructions(Action LUR, std::string uniqueId, std::string height, std::string row,
			std::string column) {
		currentInstructions[instNum] = new std::string[5];
		currentInstructions[instNum][0] =  uniqueId;
		currentInstructions[instNum][1] =std::string(1,(char)LUR);
		currentInstructions[instNum][2] = height;
		currentInstructions[instNum][3] = row;
		currentInstructions[instNum][4] = column;
		instNum = instNum + 1;
	}
	// the logic for unloading the containers to a port
	int unloadingAlgo(int i) {
		int error = 0;
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
				currentContainer = list.linkedList;
				while(currentContainer !=NULL) {		// starting from the bottom !!!
					if (currentContainer->container->destPort.toString() == route[i].toString()) {// does ship container belong to ship port?
						int *dimensions = ship->planMap->find(currentContainer->container->uniqueId)->second;
						error = error | CraneTester::isValidUnload(row, column,dimensions[0], dimensions[1]);
						if (error == 0) {
							node *temp = crane.unload(*(currentContainer->container),row, column,this->ship->planLinkedList[row][column].size);
							currentContainer = temp->next;
							tempContainers.push_front(temp);
							indxes=new std::string[3];
							indxes[0]=std::to_string(row);indxes[1]=std::to_string(column);indxes[2]=std::to_string(this->ship->planLinkedList[row][column].size);							indexies.push_front(indxes);
							popAllAbove = true;
						}
					} else {	// ship container does NOT belong to ship port
						if (popAllAbove) {	// but should I put it in temp?
							int *dimensions = ship->planMap->find(currentContainer->container->uniqueId)->second;
							error = error | CraneTester::isValidUnload(row, column, dimensions[0], dimensions[1]);
							if (error == 0) {
								node *temp = crane.unload(*(currentContainer->container),row,column,this->ship->planLinkedList[row][column].size);
								currentContainer = temp->next;
								tempContainers.push_front(temp);
								indxes=new std::string[3];
								indxes[0]=std::to_string(row);indxes[1]=std::to_string(column);indxes[2]=std::to_string(this->ship->planLinkedList[row][column].size);
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
					fillInstructions(Action::UNLOAD, popedElem->container->uniqueId, indx[2], indx[0], indx[1]);
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
					fillInstructions(Action::LOAD,popedElem->container->uniqueId, indx[2],indx[0], indx[1]);
					crane.load(popedElem->container, row, column,this->ship->planLinkedList[row][column].size);
					delete popedElem;
					delete indx;


				}

			}
		}
		return error;
	}

// the logic for loading the containers from a port
	int loadingAlgo(Container *PortInstructions, bool (*weightBalance)()) {
		int error = 0;
		bool breakIt = false;		// move to the next container from the port instructions list
		struct node currentContainer;
		Crane crane = Crane(this->ship);
		for (int p = 0; p < sizeOfArray(PortInstructions); p++) {	// for each container in the instructions
			currentContainer.container = &(PortInstructions[p]);
			error = error | isRejected(currentContainer);
			if (error == 0) {	// if its not rejected
				for (int row = 0; row < ship->shipWidth; row++) {	// for each row
					for (int column = 0; column < ship->shipLength; column++) {	// for each column
						if (ship->planLinkedList[row][column].size <= ship->planLinkedList[row][column].maxHeight-1 && weightBalance()) {		// check if we are below height limit and balanced
							error = error | CraneTester::isValidLoad(row, column, this->ship->planLinkedList[row][column].size, ship->shipWidth, ship->shipLength, this->ship->planLinkedList[row][column].maxHeight, ship->planMap,currentContainer.container->uniqueId);
							if (error == 0) {
								crane.load(currentContainer.container, row,column,this->ship->planLinkedList[row][column].size);	// load it
								fillInstructions(Action::LOAD, currentContainer.container->uniqueId, std::to_string((this->ship->planLinkedList[row][column].size-1)), std::to_string(row), std::to_string(column));	// edit instructions
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
		return error;
	}

/*test result print */
void printTestResults(node  currentContainer){
	std::cout<< "*************RESULTS : "<<currentContainer.container->uniqueId<<" *************"<<std::endl;
	std::cout<< "1)isInRoute : "<<StowageTester::isInRoute(currentContainer.container->destPort.toString(), this->route,routeIndex)<<std::endl;
	std::cout<< "2)isFull: "<<CraneTester::isFull(this->ship)<<std::endl;
	std::cout<< "3)isValidId: "<<CraneTester::isValidId(currentContainer.container->uniqueId)<<std::endl;
	std::cout<< "4)isLegalWeight: "<<CraneTester::isLegalWeight(currentContainer.container->weight)<<std::endl;
	std::cout<< "*************END*************"<<std::endl;

}


// rejection test
	int isRejected(node currentContainer) {
		printTestResults(currentContainer);
		int error = 0;
		error = error | StowageTester::isInRoute(currentContainer.container->destPort.toString(), this->route,routeIndex);
		error = error | CraneTester::isFull(this->ship);
		error = error | CraneTester::isValidId(currentContainer.container->uniqueId);
		error = error | CraneTester::isLegalWeight(currentContainer.container->weight);
		if(error != 0){
			fillInstructions(Action::REJECT, currentContainer.container->uniqueId,"-1", "-1", "-1");
			return error;
		}
		return 0;
	}

	/*/
	 //	LETS SAIL!!
	 //
	 //	This function takes the index of the current port in the list of routes as the parameter i (first port: i=0)
	 //	and the ship
	 //	returns a list of instructions as following:
	 //	{"load/unload/reject", a container's unique id, height, row, column}
	 /*/

	_205962657_a(int i, Ship *ship, Port *route, Container *instructions):ship(ship) {
		this->instNum = 0;	// The instruction number of the returned instruction
		this->currentInstructions = new std::string*[100];
		this->route = route;
		routeIndex=i;
		unloadingAlgo(i);
		loadingAlgo(instructions, weightBalance);
		fillInstructions(Action::REJECT, "last", "last", "last", "last");
	}
	_205962657_a(){
	this->instNum = 0;	// The instruction number of the returned instruction
	this->currentInstructions = new std::string*[150];

	}
}
;
REGISTER_ALGORITHM (_205962657_a)
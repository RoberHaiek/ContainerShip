#include "../Interfaces/AbstractAlgorithm.h"
#include "../Interfaces/AlgorithmRegistration.h"
#include "../Common/IOHandler.cpp"
#include <cctype>
#include <iostream>
#include <queue>
#include <sstream>

class _205962657_b: public AbstractAlgorithm{
public:
	int instNum;
	Ship *ship;
	Port *route;					// array of ports
	std::string **currentInstructions;
	std::deque<node*> tempContainers;
	std::deque<node*> loadBackContainers;
	std::deque<std::string*> indexies;
	vector<Container> rejectedNotInRoute;
	int routeIndex;
	int shipCapacity;
	char** routeArr;
	WeightBalanceCalculator* calc;


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
	

	int readShipPlan(const std::string& full_path_and_file_name){
		// read the ship plan from a file
		int error = 0;
		
		this->instNum = 0;	// The instruction number of the returned instruction
		this->routeIndex=0;

		  	error |=initShipPlan(ship,full_path_and_file_name);
		return error ; // success
	}


	int readShipRoute(const std::string& full_path_and_file_name){
		// read ship route from file
		int error = 0;
		//cout << "*initRoute from stowage"<<endl;
		char** routeArray;
	  	error |=initRoute(routeArray,full_path_and_file_name);
		status isFatalError=giveMeErrorStatus(error);{
			if(isFatalError!=status::Ignore){
				return error;
			}
		}
		this->routeArr=routeArray;
		route= getPortsFromRoute(routeArray);
		//cout<<"* finish route from stowage"<<endl;

		return error ; // success
	}

	int setWeightBalanceCalculator(WeightBalanceCalculator& calculator){
		calc=&(calculator);
		return 0;
	}

	int getInstructionsForCargo(const std::string& input_full_path_and_file_name, const std::string& output_full_path_and_file_name){
		// read from input, write to output - PARSING
		//string fileName=getTheFileName(input_full_path_and_file_name);
		//cout<<"111111111111 "<<input_full_path_and_file_name <<"  and the filename is : "<<fileName<<endl;
		
		int error = 0;
		//getRouteIndex(routeIndex,fileName);

		//cout<<"the index in get inst ="<<routeIndex<<endl;
		ifstream fd_info;
		fd_info.open(input_full_path_and_file_name,ios_base::in);//open the file
		//checking the access to the file
		int notBadPort=1;
		if(!fd_info){
		notBadPort=0;
		error|=(int)pow(2,16);
		}

		Container* containers=parseCargoFile(input_full_path_and_file_name);
		int containerIndex=0;

		while(true){
			if(containers[containerIndex].uniqueId=="last"){
				error|=containers[containerIndex].weight;			
				break;
			}
		containerIndex++;
		}
		
		//cout<<"the first container is :"<<containers[0].uniqueId<<endl;
		error = error | unloadingAlgo(routeIndex);

		int sizeArray=sizeOfArray(containers);
		//is it the last route
		if((routeIndex==routeSize-1) && notBadPort){
			//is it have cargo on it
			if(containers[0].uniqueId.compare("last")!=0){
			error|=(int)pow(2,17);}
			notBadPort=0;
		}
		if(notBadPort){
		shipCapacity=CraneTester::getCapacity(this->ship);
		Container * sortedInst=CraneTester::priority(containers,routeArr,routeIndex ,sizeArray,routeSize,rejectedNotInRoute);
		//cout<<"before loading error was :"<<error;
		error |= loadingAlgo(sortedInst, weightBalance);
		//cout<<"after loading error become :"<<error;

		rejectWhatLeft();
		}
		fillInstructions(Action::REJECT, "last", "last", "last", "last");
		instructionsOut(currentInstructions,output_full_path_and_file_name);
		this->instNum = 0;
		this->routeIndex++;
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
	void rejectWhatLeft(){
		for(auto it=rejectedNotInRoute.begin();it!=rejectedNotInRoute.end();it++){
			fillInstructions(Action::REJECT, it->uniqueId,"-1", "-1", "-1");
			
		}
	
	}
//loading for sure 
void loadAgain(node *temp){
	int error=0;
	Crane crane = Crane(this->ship);
	for (int column = 0; column < ship->shipLength; column++) {	// for each col
		for (int row = 0; row < ship->shipWidth; row++) {	// for each row
			if (ship->planLinkedList[row][column].size <= ship->planLinkedList[row][column].maxHeight-1 && weightBalance()) {		// check if we are below height limit and balanced
					error = error | CraneTester::isValidLoad(row, column, this->ship->planLinkedList[row][column].size, ship->shipWidth, ship->shipLength, this->ship->planLinkedList[row][column].maxHeight, ship->planMap,temp->container->uniqueId);
						if (error == 0) {
							crane.load(temp->container, row,column,this->ship->planLinkedList[row][column].size);	// load it
							fillInstructions(Action::LOAD, temp->container->uniqueId, std::to_string((this->ship->planLinkedList[row][column].size-1)), std::to_string(row), std::to_string(column));	// edit instructions
							}
					}
				}
			}
		}







	// the logic for unloading the containers to a port
	int unloadingAlgo(int i) {
		int error = 0;
		bool popAllAbove; // true if we're popping a container from a stack and we need to pop all above it
		struct node *currentContainer;
		std::string* indxes;
		int totalPoped=0;
		Crane crane = Crane(this->ship);
		for (int row = 0; row < ship->shipWidth; row++) {
			for (int column = 0; column < ship->shipLength; column++) {
			totalPoped=0;
				if (this->ship->planLinkedList[row][column].size == 0) {
					continue;
				}
				popAllAbove = false;
				cellLinkedList list=ship->planLinkedList[row][column];
				currentContainer = list.linkedList;
				while(currentContainer !=NULL) {		// starting from the bottom !!!
					if (currentContainer->container->destPort.toString() == route[i].toString()) {// does ship container belong to ship port?
						int *dimensions = ship->planMap->find(currentContainer->container->uniqueId)->second;
						int flag =  CraneTester::isValidUnload(row, column,dimensions[0], dimensions[1]);
						if (flag == 0) {
							node *temp = crane.unload(*(currentContainer->container),row, column,this->ship->planLinkedList[row][column].size);
							currentContainer = temp->next;
							tempContainers.push_front(temp);
							indxes=new std::string[3];
							indxes[0]=std::to_string(row);indxes[1]=std::to_string(column);indxes[2]=std::to_string(dimensions[2]);	
							indexies.push_front(indxes);
						//	cout<<"	*)"<<temp->container->uniqueId<< " // poped with indexes "<<  indxes[2]<<","<<  indxes[0]<<","<<  indxes[1]<<endl;
							popAllAbove = true;
							totalPoped++;
						}
					} else {	// ship container does NOT belong to ship port
						if (popAllAbove) {	// but should I put it in temp?
							int *dimensions = ship->planMap->find(currentContainer->container->uniqueId)->second;
							error = error | CraneTester::isValidUnload(row, column, dimensions[0], dimensions[1]);
							if (error == 0) {
								int *dimensions = ship->planMap->find(currentContainer->container->uniqueId)->second;

								node *temp = crane.unload(*(currentContainer->container),row,column,this->ship->planLinkedList[row][column].size);
								currentContainer = temp->next;
								tempContainers.push_front(temp);
								indxes=new std::string[3];
								indxes[0]=std::to_string(row);indxes[1]=std::to_string(column);indxes[2]=std::to_string(dimensions[2]);
								indexies.push_front(indxes);
								totalPoped++;
								//cout<<"	*)"<<temp->container->uniqueId<< " // poped with indexes "<<  indxes[2]<<","<<  indxes[0]<<","<<  indxes[1]<<endl;

							}
						}
					}
					if(!popAllAbove && currentContainer!=NULL){
					currentContainer =currentContainer->next;}
				}

				this->ship->planLinkedList[row][column].size-=totalPoped;
				// loading containers from temp back to ship
				node *popedElem;
				std::string *indx;
				while (!tempContainers.empty()) {
					popedElem = tempContainers.front();
					indx=indexies.front();
					indexies.pop_front();
					tempContainers.pop_front();
					std::string dstPort = popedElem->container->destPort.toString();
		//cout<<"		-*-*-*unloading "<<popedElem->container->uniqueId<<endl;
					fillInstructions(Action::UNLOAD, popedElem->container->uniqueId, indx[2], indx[0], indx[1]);
					if (dstPort.compare(route[i].toString()) == 0) {
						delete popedElem;
						delete[] indx;
					} else {
		//cout<<"		-*-*-*to load back  "<<popedElem->container->uniqueId<<endl;

						loadBackContainers.push_front(popedElem);
						indexies.push_back(indx);
					}
				}
}
}
				node *popedElem;//
				std::string *indx;
				while (!loadBackContainers.empty()) {
					popedElem = loadBackContainers.back();
					indx=indexies.back();
					indexies.pop_back();
					loadBackContainers.pop_back();
					//cout<<"		-*-*-*loading back ??????"<<endl;
					loadAgain(popedElem);
					/*fillInstructions(Action::LOAD,popedElem->container->uniqueId, indx[2],indx[0], indx[1]);
		//cout<<"		-*-*-*loading back "<<popedElem->container->uniqueId << ","<<indx[2]<<","<<indx[0]<<"," <<indx[1]<<endl;
					crane.load(popedElem->container, row, column,this->ship->planLinkedList[row][column].size);*/
					delete popedElem;
					delete[] indx;


				}

		//	}
	//	}

		return error;
	}


// the logic for loading the containers from a port
	int loadingAlgo(Container *PortInstructions, bool (*weightBalance)()) {
//cout<<"*********in loading BBBBBBBB"<<endl;
		int error = 0;
		int is_regected=0;
		bool breakIt = false;		// move to the next container from the port instructions list
		struct node currentContainer;
		Crane crane = Crane(this->ship);
		for (int p = 0; p < sizeOfArray(PortInstructions); p++) {	// for each container in the instructions
	
			currentContainer.container = &(PortInstructions[p]);
//new
			if(currentContainer.container->destPort.port=="reject"){
				fillInstructions(Action::REJECT, currentContainer.container->uniqueId,"-1", "-1", "-1");
				continue;
			}
			//cout<<endl<<endl<<currentContainer.container->uniqueId<<endl<<endl;
			is_regected=isRejected(currentContainer);
			if(is_regected!=0){
				if(is_regected==-1){
				}else{
				error|=is_regected;}
				//cout<<"***** rejected error="<<error<<endl;	
			}
			if (is_regected== 0) {	// if its not rejected
				//cout<<"***** NOT ! rejected"<<endl;
				for (int column = 0; column < ship->shipLength; column++) {	// for each col
					for (int row = 0; row < ship->shipWidth; row++) {	// for each row
			//cout<<"****row,col= "<<row<<","<<column<<endl;
			//cout <<"ship->planLinkedList[row][column].size <= ship->planLinkedList[row][column].maxHeight-1 == "<< (ship->planLinkedList[row][column].size <= ship->planLinkedList[row][column].maxHeight-1)<<endl;
						if (ship->planLinkedList[row][column].size <= ship->planLinkedList[row][column].maxHeight-1 && weightBalance()) {		// check if we are below height limit and balanced
							is_regected=CraneTester::isValidLoad(row, column, this->ship->planLinkedList[row][column].size, ship->shipWidth, ship->shipLength, this->ship->planLinkedList[row][column].maxHeight, ship->planMap,currentContainer.container->uniqueId);
							if (is_regected== 0) {
								crane.load(currentContainer.container, row,column,this->ship->planLinkedList[row][column].size);	// load it
								fillInstructions(Action::LOAD, currentContainer.container->uniqueId, std::to_string((this->ship->planLinkedList[row][column].size-1)), std::to_string(row), std::to_string(column));	// edit instructions
								shipCapacity--;
								breakIt = true;
								break;
							}else{
			
							fillInstructions(Action::REJECT, currentContainer.container->uniqueId,"-1", "-1", "-1");
							error|=is_regected;

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
//cout<<"*********end loading BBBBBBBB"<<endl;

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
	//	printTestResults(currentContainer);
		int error = 0;
		int rejectFlag=0;
		int tmpError=0;
		
		tmpError=StowageTester::isInRoute(currentContainer.container->destPort.toString(), this->route,routeIndex);
		if(tmpError!=0){
			rejectFlag=-1;
			error = -1;
		}
		tmpError=CraneTester::isValidId(currentContainer.container->uniqueId);
		if(tmpError!=0){
			error|=tmpError;
			rejectFlag=1;
		}
		tmpError=CraneTester::isLegalWeight(currentContainer.container->weight);
		if(tmpError!=0){
			error|=tmpError;
			rejectFlag=1;
		}
		tmpError=CraneTester::isFull(this->ship);
		if(tmpError!=0){
			error|=tmpError;
			rejectFlag=1;
		}
		if(shipCapacity==0){
			error|=(int)pow(2,18);
			rejectFlag=1;
		}
		if(rejectFlag!=0){
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
	_205962657_b(){
	this->instNum = 0;	// The instruction number of the returned instruction
	this->currentInstructions = new std::string*[200];
	this->routeIndex=0;

	}
}
;
REGISTER_ALGORITHM (_205962657_b)
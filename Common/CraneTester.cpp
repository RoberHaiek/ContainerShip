#include "Ship.cpp"
#include <math.h>
#include <set>

class CraneTester{
public:
// checks if the ship is full
	static int isFull(Ship* ship){
		for(int i=0;i<ship->shipWidth;i++){
			for(int j=0;j<ship->shipLength;j++){
				if(!(ship->planLinkedList[i][j].maxHeight==ship->planLinkedList[i][j].size)){
					return 0;
				}
			}
		}
		return ErrorID::ContainersTooManyContainers;
	}


	// checks if the container weight is legal
	static int isLegalWeight(int containerWeight){
		if(containerWeight>=0)
			return 0;
		return ErrorID::ContainersMissingOrBadWeight;
	}
	
	static int charToIntISO(char c, int index){
		int erase =0;
		if(c=='a' || c=='A'){
			erase=55;
		}else if(((int)c>=int('B') && (int)c<=int('K')) ||((int)c>=int('b') && (int)c<=int('k'))){
			erase=54;	
		}else if(((int)c>=int('L') && (int)c<=int('U'))||((int)c>=int('l') && (int)c<=int('u'))){
			erase=53;
		}else if(((int)c>=int('V') && (int)c<=int('Z'))||((int)c>=int('v') && (int)c<=int('z'))){
			erase=52;
		}

		if(index<4){
				if((int)c>=int('A') && (int)c<=int('Z')){
					return ((int)c-erase)*pow(2,index);
				}else{
					return ((int)c-erase-32)*pow(2,index);

				}
		
		}else{
			return ((int)c-48)*pow(2,index);
		}
	
	}
	// checks if the unique id of a container is valid
	static int isValidId(std::string uniqueId){
		if(uniqueId.length()!=11)
			return ErrorID::ContainersIllegalIDCheck;
		for(int i=0;i<10;i++){
			if(i<3){
				if(!(((int)uniqueId[i]>=65 && (int)uniqueId[i]<=90)|| ((int)uniqueId[i]>=97 && (int)uniqueId[i]<=122))){
					//cout <<" and the char is "<< (int)uniqueId[10]-48<<endl;
					return ErrorID::ContainersIllegalIDCheck;
				}
			}
			if(i==3){
				if(!((int)uniqueId[i]==74 || (int)uniqueId[i]==85 || (int)uniqueId[i]==90 || (int)uniqueId[i]==106 || (int)uniqueId[i]==117 || (int)uniqueId[i]==122)){
					//cout <<" and the char is "<< (int)uniqueId[10]-48<<endl;
					return ErrorID::ContainersIllegalIDCheck;
				}
			}
			if(i>3){
				if(!((int)uniqueId[i]>=48 && (int)uniqueId[i]<=57)){
					//cout <<" the char is "<< (int)uniqueId[10]-48<<endl;
					return ErrorID::ContainersIllegalIDCheck;
				}
			}
		}
		int sum=0;
		for(int i=0;i<10;i++){
		sum+=charToIntISO(uniqueId[i], i);
		//cout <<" and the char is "<< (int)uniqueId[10]-48<<endl;
		}
		
		int checkNumber= sum - int(sum/11) * 11;
		if(checkNumber==10){checkNumber=0;}
		if(checkNumber==(int)uniqueId[10]-48){
			return 0;
		}
		return ErrorID::ContainersIllegalIDCheck;
	}

	static int isValidLoad(int row, int column, int floor, int shipWidth, int shipLength, int cellHeight, std::map<std::string,int*>* planMap, std::string uniqueId){
		if(floor>=cellHeight || floor<0){
			return ErrorID::ShipPlanWrongXY;
		}
		if(row>=shipWidth || row<0 || column>=shipLength || column<0){
			return ErrorID::ShipPlanWrongXY;
		}
		if(planMap->find(uniqueId) != planMap->end()){
			return ErrorID::ContainersIDArleadyOnShip;
		}
		return 0;
	}

	static int isValidUnload(int row, int column, int shipWidth, int shipLength){
		if(row != shipWidth || column!=shipLength){
			return -1;
		}
		return 0;
	}


	static int isValidUnloadSimulation(int row, int column, int shipWidth, int shipLength,int containerFloor,Ship* ship,Container* container){
		if(row != shipWidth || column!=shipLength){

			return -1;
		}
		struct node *temp;
		struct node *prev;
		temp=ship->planLinkedList[row][column].linkedList;
		
		if(temp==NULL || containerFloor>=ship->planLinkedList[row][column].maxHeight || containerFloor<0){
			return -1;
		}
		if(ship->planMap->find(container->uniqueId)==ship->planMap->end()){
			return -1;
		}
		prev=temp;
		while(temp->container->uniqueId != container->uniqueId){

			prev=temp;
			temp=temp->next;
			if(temp==NULL){
				return -1;
			}
		}
		//is there somone above of him?
		if(temp->next!=NULL){
			return -1;	
		}
		temp=prev;
		return 0;
	}

	static int isCorrectContainer(std::string containerUniqueId,std::string expectedUniqueId){
		if(containerUniqueId.compare(expectedUniqueId)==0){
			return -1;
		}
		return 0;
	}
	
	static int isDuplicateIdOnShip(std::map<std::string,int*>* planMap, std::string uniqueId){
		if(planMap->find(uniqueId) != planMap->end()){
			return ErrorID::ContainersDuplicateID;		}
		return 0;
	}
	static int lastPortHasContainers(int size){
		if(size != 0 ){
			return ErrorID::ContainersTooManyContainers;	
		}
		return 0;
	}


	static int getCapacity(Ship* ship){
	int capacity=0;
	for(int i=0;i<ship->shipWidth;i++){
			for(int j=0;j<ship->shipLength;j++){
				capacity+=ship->planLinkedList[i][j].maxHeight-ship->planLinkedList[i][j].size;

				}
		}
//cout<< "ship capacity is :: "<<capacity<<endl;
	return capacity;
	
	}
 
	/*sort the cargo containers by how much the port is far from the current port */
	static Container* priority(Container *PortInstructions,char** routeArray,int routeIndx ,int numOfInstruction,int routeSize,vector<Container>& rejectedNotInRoute){
		set<string> ports;
		set<int> indexes;
cout << "priorety "<<endl;
		//adds the next ports just once 
		for(int i=routeIndx+1;i<routeSize;i++){
		cout << "init the set if ? "<<routeArray[i];
			if(!ports.count(string(routeArray[i]))){
				cout << "  accepted";
				ports.insert(string(routeArray[i]));
			}
		cout << endl;
		}
		//if its the last port then we reject all the containers
		if(routeIndx==routeSize-1){//last Port
		Container* sortedContainers=new Container[1];
		sortedContainers[0]=Container(0,Port("last"),"last");
		for(int inst=0;inst<=numOfInstruction;inst++){
			cout<< "in last port we must reject "<<PortInstructions[inst].uniqueId<<endl;
			rejectedNotInRoute.push_back(PortInstructions[inst]);

		}
		return sortedContainers;
		
		}
		Container* sortedContainers=new Container[numOfInstruction+1];
		int containerIndx=0;

		//for each port -- we find the relevant cargo and push it in the set one by one
for(int i=routeIndx+1;i<routeSize;i++){
		for(auto curr=ports.begin();curr!=ports.end(); curr++){
if(*curr!=routeArray[i]){continue;}
			for(int inst=0;inst<=numOfInstruction;inst++){
			cout<< "the port is :: "<<*curr <<" , and the container is ::"<<PortInstructions[inst].uniqueId<<endl;
				if(PortInstructions[inst].uniqueId =="last"){
					break;
				}else if(PortInstructions[inst].destPort.port=="reject" && !indexes.count(inst)){//this container have been rejected for bad weight/dst port format
			cout << "	*)'reject'"<<endl;
					indexes.insert(inst);
					rejectedNotInRoute.push_back(PortInstructions[inst]);	
				}
				else if(PortInstructions[inst].destPort.port==*curr && !indexes.count(inst)){ //the port name matches
			cout << "	*)taken"<<endl;
					indexes.insert(inst);
					sortedContainers[containerIndx]=Container(PortInstructions[inst].weight,PortInstructions[inst].destPort,PortInstructions[inst].uniqueId);
					containerIndx++;
				}else{
					
					cout << "	*)in the else zbbe with port = "<<PortInstructions[inst].destPort.port<<endl;
					cout << "PortInstructions[inst].destPort.port==*curr -- "<<(PortInstructions[inst].destPort.port==(*curr))<<endl;
					cout << "!indexes.count(inst) -- "<<(!indexes.count(inst))<<endl;


				}
			}
ports.erase(*curr);
		}
}
		//for each other instructions ... add them to the reject (these are the containers that its destinations not in the route)
		for(int inst=0;inst<=numOfInstruction;inst++){
			if(PortInstructions[inst].uniqueId =="last"){
					break;
				}
			if(!indexes.count(inst)){
			rejectedNotInRoute.push_back(PortInstructions[inst]);
			}
		}
		sortedContainers[containerIndx]=Container(0,Port("last"),"last");
		return sortedContainers;
	}

};
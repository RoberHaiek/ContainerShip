//============================================================================
// Name        : MiniSimulator.cpp
// Author      : Aubaida
// Version     :
// Copyright   : Your copyright notice
// Description : Simulator in C++, Ansi-style
//============================================================================
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <fstream>
#include <sys/stat.h>
#include <queue> 
//#include "../Algorithms/Stowage.cpp"
#include "../Simulation/WeightBalanceCalculator.cpp"
//#include "../Interfaces/IOHandler.h"//may changed
#include "../Common/IOHandler.cpp"
#include "AlgorithmRegistrar.h"
#include "../Interfaces/AbstractAlgorithm.h"
//#include "../Algorithms/StowageTester.cpp"
using namespace std;
const char* EXPECTED_OUTPUT="/expected_output";
string travelName;
string AlgoName="StowageAlgo";
//Stowage* curAlgo;
string travel_path="",algorithm_path="",output="";
queue<string> algoQueue;
/*---------------FUNC DEC-------------*/
void getFiveElementsIntoArray(string line,int& seek,string* fivedArray,int INDICATOR);
/*------------------DEBUGGING METHODS-------------------*/
void printFiles(DIR* fd){
	struct dirent *entry;
    	while ((entry = readdir(fd)))
		if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
			std::cout <<entry->d_name<<std::endl;
	}
}
void printContainerArray(Container* arr,char* fileName){
	int index=0;
	Container pt=arr[index];
	std::cout << "-list of containers in " <<fileName<<" :"<<std::endl;
	while(pt.uniqueId.compare("last")!=0){
		std::cout <<"	"<< pt.uniqueId <<" ,"<<pt.weight<<" ,"<<pt.destPort.toString()<<std::endl;
		pt=arr[++index];
	}
	
}

/*-------------------------------------------------------------------------------------------------------------*/

// rejection test
	int isRejected(node currentContainer, Ship *ship,Port* route, int routeIndex) {
		int error = 0;
		error+=StowageTester::isInRoute(currentContainer.container->destPort.toString(),route,routeIndex);	// is the container's destination NOT port in route?
		error+=CraneTester::isValidId(currentContainer.container->uniqueId);						// is the container's unique ID invalid?
		error+=CraneTester::isDuplicateIdOnShip(ship->planMap,currentContainer.container->uniqueId);				// is the container's unique ID already on the ship?
		error+=CraneTester::isLegalWeight(currentContainer.container->weight);						// is the container's weight illegal?
		return error;
	}

//[??]
int validateAlgorithm(std::string **currentInstructions, int numOfInstructions, Port port, Ship *ship,Port* route, int routeIndex ){
	int error = 0;
	struct node currentContainer;
	int row, column;
	Crane crane = Crane(ship);
	for(int i=0;i<numOfInstructions;i++){
		*currentContainer.container = Container(0,port,currentInstructions[i][1]);
		row = std::stoi(currentInstructions[i][3]);
		column = std::stoi(currentInstructions[i][4]);
		// Is the command load/reject?
		if(currentInstructions[i][0] == "L" || currentInstructions[i][0] == "R"){
			error+=isRejected(currentContainer,ship,route,routeIndex);
			// Should the container be rejected according to the algorithm?
			if(currentInstructions[i][0] == "R"){
				// Was it really rejected?
				if(error != 0){
					error = 0;
					continue;
				}
				// The algorithm rejected a container that should not have been rejected
				else{
					std::cout << "Illegal algorithm command: Container " << currentContainer.container->uniqueId << " should not be rejected \n";
					return -1;
				}
			}
			// Should the container be rejected but wasn't rejected by the algorithm?
			if(error != 0 ){
				std::cout << "Illegal algorithm command: Container " << currentContainer.container->uniqueId << " should have been rejected \n";
				return -1;
			}
			error+=CraneTester::isValidLoad(row, column, ship->planLinkedList[row][column].size, ship->shipWidth, ship->shipLength, ship->planLinkedList[row][column].maxHeight, ship->planMap,currentContainer.container->uniqueId);
			// Does the container exceed ship height limit?
			if (ship->planLinkedList[row][column].size <= ship->planLinkedList[row][column].maxHeight) {
				// Was the load valid?
				if(error == 0){
					crane.load(currentContainer.container, row,column,ship->planLinkedList[row][column].size);
				}
				else{
					std::cout << "Illegal algorithm command: Invalid load for container " << currentContainer.container->uniqueId << "\n";
					return -1;

				}
			}
			else{
				std::cout << "Illegal algorithm command: Exceeded ship height limit while loading container " << currentContainer.container->uniqueId << " to " << row << ", " << column << "\n";
				return -1;
			}	
		}
		// Is the command unload?
		if(currentInstructions[i][0] == "U"){
			int *dimensions = ship->planMap->find(currentContainer.container->uniqueId)->second;
			error+=CraneTester::isValidUnload(row, column,dimensions[0], dimensions[1]);
			// Can we unload legally?
			if (error == 0) {
				node *temp = crane.unload(*(currentContainer.container),row, column,ship->planLinkedList[row][column].size);
			}
			else{
				std::cout << "Illegal algorithm command: Could not unload container " << currentContainer.container->uniqueId << "\n";
				return -1;
			}
		}
	}
	return 0;
}

//[16]
int checkMapIfAsExpected(string** expectedInstructions,map<string,vector<string>>& InstructionsMap){
	int index=0;
	int count;
	string algoUid,algoOperation;
	while((expectedInstructions[index][0]).compare("last")!=0){
		count=InstructionsMap.count(expectedInstructions[index][0]);
		if(count==0){
			std::cout<<"ERROR[16][1] : the algorithim missing instruction Container : "<<expectedInstructions[index][0]<<", operation :"<<expectedInstructions[index][1]<<std::endl;
			return ERROR;
		}else{
			auto it = InstructionsMap.find(expectedInstructions[index][0]);
			auto v=it->second;
			algoOperation =v[0];
			cout<<"ID :" << expectedInstructions[index][0]<< ", algo op: "<< algoOperation <<endl;
			if(algoOperation.compare(expectedInstructions[index][1])!=0 ){
				std::cout<<"ERROR[16][2] : the algorithim wrong operation instruction Container : "<<expectedInstructions[index][0]<<", operation :"<<expectedInstructions[index][1]<<std::endl;
				return ERROR;
			}else{
				InstructionsMap.erase(it);
			}
		}
		index++;
	}

	//checking if the map EMPTY 
	if(auto it2=InstructionsMap.begin(); it2!=InstructionsMap.end()){
		std::cout<<"ERROR[16][3] : the algorithim adds more operations than expected"<<std::endl;
		return ERROR;
	}
	return SUCCESS;
	
}
//[15]
int insertInstructionToMap(string** algoInstructions,map<string,vector<string>>& InstructionsMap){
	int index=0;
	int count;
	while((algoInstructions[index][0]).compare("last")!=0){
		cout<<"**inserting to map :"<<algoInstructions[index][0] << " , "<<algoInstructions[index][1]<<endl;

		count=InstructionsMap.count(algoInstructions[index][0]);
		if(count==0){
			vector<string> v1=vector<string>();
			v1.push_back(algoInstructions[index][1]);
			InstructionsMap.insert ( std::pair<string,vector<string>>(algoInstructions[index][0],v1) );
		}else{
			auto it = InstructionsMap.find(algoInstructions[index][0]);
			auto v=it->second;
			string old =v[0];
			if((algoInstructions[index][1]).compare("reject")==0 ||old.compare("reject")==0){
				std::cout<<"ERROR[15][1] : two instruction to the same container while one or two of them is reject"<<std::endl;
				 return ERROR;
		}else if(old.compare("unload")==0 && (algoInstructions[index][1]).compare("load")==0){
				InstructionsMap.erase(it);
			}else if(old.compare("load")==0 && (algoInstructions[index][1]).compare("unload")==0){
				InstructionsMap.erase(it);
			}else{ //load-->load /unload -->unload
					cout<<"old op= "<<old<<" , "<<"curr op ="<<algoInstructions[index][1]<<endl;
				 std::cout<<"ERROR[15][2] : impossible to do two instructions for the same container as (load -->load /unload--> unload)"<<std::endl;
				 return ERROR;
			}
		}
		index++;
	}
	cout<<endl<<"done inserting algo instructions" <<endl;
	return SUCCESS;
}

//[12]
int checkInstructionPerPort(int portIndex,string** algoInstructions){
	string cargoFileName = getCargoFileName(portIndex,true);
	map<string,vector<string>> InstructionsMap;
	int fillMap=insertInstructionToMap(algoInstructions,InstructionsMap);
	if(fillMap!=SUCCESS){
		return ERROR;
	}
	string** expectedInstructions = ReadExpectedInstructions(cargoFileName);
	cout<<endl<<"done reading expected instructions" <<endl;
	cout<<endl<<"start checking the map content" <<endl;

	int isSuccess=checkMapIfAsExpected(expectedInstructions,InstructionsMap);
	cout<<endl<<"finished checking the map content" <<endl;

	return isSuccess;


}


//[3] called in [2]
void simulateTravel(){
	  //intiate the ship and get the route
	ErrorCode errCode;

	cout << "*initShipPlan"<<endl;
	string shipPlanName;
	string routeName;
	int err=getTheFileNameFromTheTravel(travelPath,"ship_plan",shipPlanName);
	if(err!=0 && err!=-1){
		handleError(output,"Simulator",err);
		return;
	}
	err=getTheFileNameFromTheTravel(travelPath,"route",routeName);
	/*if(err!=0 && err!=-1){
		handleError(output,"Simulator",err);
		return;
	}*/
	cout<<"********************the ship plane :"<<travelPath+"/"+shipPlanName<<"/n******************and the route : "<<travelPath+"/"+routeName<<endl;
	shipPlanName=travelPath+"/"+shipPlanName+".ship_plan";
	routeName=travelPath+"/"+routeName+".route";
	err=initShipPlan(ship,shipPlanName);
	if(err!=0 && err!=-1){
		handleError(output,"Simulator",err);
		return;
	}
	cout << "*initRoute"<<endl;
	err=initRoute(route,routeName);

	Port* ports = getPortsFromRoute(route);
	//free route
	for(int i=0;i<routeSize;i++){
		delete[] route[i];
	}
	if(route!=NULL){
		delete route;
	}

	parseResults (AlgoName ,travelName ,0 ,0);

//new implemntation using algo regestrar
	auto& registrar = AlgorithmRegistrar::getInstance();
    	{
        	std::string error;
		const char *cstr = algorithm_path.c_str();
		  DIR* fd_Algo=opendir(cstr);
		  if(fd_Algo==NULL){
			  std::cout << "ERROR[3][1]- can't open "<<algorithm_path<<std::endl; 
				return;
			}

		struct dirent *entry;
    		while ((entry = readdir(fd_Algo)))
		if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
			string algoName=getNameWithoutExtinsion(entry->d_name,'.',"so");
			if(algoName.compare("/")!=0){
				cout << "the algoritim is : "<< algoName <<endl;
				algoQueue.push(algoName);
				if (!registrar.loadAlgorithmFromFile((algorithm_path+"/"+string(entry->d_name)).c_str(), error)) {
	        			std::cerr << error << '\n'; 
            				return ;
        			}


			}
			
		}

	closedir(fd_Algo);
    	}
	
	cout<<"******starting the loop over the algos **********"<<endl;
    	for (auto algo_iter = registrar.begin();algo_iter != registrar.end(); ++algo_iter) {	
		string algoName=algoQueue.front();
		algoQueue.pop();
		auto algo = (*algo_iter)();
		int errShipPlanAlgo=algo->readShipPlan(shipPlanName);
		int errRouteAlgo=algo->readShipRoute(routeName);
		if(errShipPlanAlgo || errRouteAlgo){
			cout<< "??????????????error in shipPlan or in route??????????????" << endl;
			break;
		}
		//make directory
		string makeDir=algoName+"_"+travelName+"_crane_instructions";
		const char *cstr = makeDir.c_str();
		int err= mkdir (cstr,0777);
		if(err){	
		std::cout << "ERROR[3][1]- can't make dir with name : "<<makeDir<<std::endl; 
		}
		cout<<"******starting the loop over the ports **********"<<endl;

		for(int routeIndex = 0; routeIndex < routeSize ; routeIndex++ ){
		cout<<"******port number = "<<routeIndex<<" **********"<<endl;

		string FileNameCarge=getCargoFileName(routeIndex,true);
		string FileNameInstruction=getCargoFileName(routeIndex,false);
		cout<<"****** getting instructions **********"<<endl;
		algo->getInstructionsForCargo(travelPath+"/"+FileNameCarge, output+"/"+makeDir+"/"+FileNameInstruction);

		//validate 

		string **instructions=ReadExpectedInstructions( output+"/"+makeDir+"/"+FileNameInstruction);
		ifstream fd_info;
		fd_info.open( output+"/"+makeDir+"/"+FileNameInstruction,ios_base::in);//open the file
		//checking the access to the file
		if(!fd_info){
		std::cout << "ERROR[8][1]- can't open "<< output+"/"+makeDir+"/"+FileNameInstruction<< std::endl;
		}

		int numOfInstructions=getNumOfLines(fd_info);//get container size

		int validation = validateAlgorithm(instructions, numOfInstructions, ports[routeIndex], ship, ports, routeIndex);
		cout<<"VALIDATION==========================="<<validation<<endl;
		
		/*if(check==SUCCESS){
			parseResults (algoName,travelName,numInstructions,routeIndex+1);
		}else{
			parseResults (algoName,travelName,numInstructions,0-(routeIndex+1));
		}*/
	}


		
	}	
    	//return EXIT_SUCCESS;
	if(ship!=NULL){
		delete ship;
	}
	if(ports!=NULL){
		delete ports;
	}
	 	
}

//[2] called in [1]
void simulate(DIR* fd){
    struct dirent *entry;
    while ((entry = readdir(fd)))
      if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
		  //open the travel dir
		  cout<< "the work path is : "<<workPath<<endl;
		  travelPath=workPath+"/"+string(entry->d_name);
		  const char *cstr = travelPath.c_str();
		  cout<< "the travel path is : "<<travelPath<<endl;
		  DIR* fd_travel=opendir(cstr);
		  std::cout <<entry->d_name<<std::endl;
		  if(fd_travel==NULL){
			  std::cout << "ERROR[2][1]- can't open "<<entry->d_name<<std::endl; 
			  }else{
				travelName=string(entry->d_name);
				  simulateTravel();//simulate the travel
				  //free resources
				  closedir(fd_travel);
			  }
	  }
	if(fd_results.is_open()){
		fd_results.close();	
	}
}
	
int getFromCommandLine(char *argv[],int argc,string& travel_path,string& algorithm_path ,string& output){
	for(int i=1;i<argc;i+=2){
		if(string(argv[i]).compare("-travel_path")==0 && travel_path.compare("")==0){
			travel_path=string(argv[i+1]);
		}else if(string(argv[i]).compare("-algorithm_path")==0 && algorithm_path.compare("")==0){
			algorithm_path=string(argv[i+1]);
		}else if(string(argv[i]).compare("-output")==0 && output.compare("")==0){
			output=string(argv[i+1]);
		}else{
			return ERROR;
		}
	}
	if(travel_path.compare("")==0){
		return ERROR;
	}
	if(algorithm_path.compare("")==0){
		algorithm_path=".";
	}
	if(output.compare("")==0){
		output=".";
	}
	return SUCCESS;
}

/* argv[1] will be the path of the workspace(IO-Files)*/
//[1]
int main(int argc, char *argv[]) {

	if(argc!=3 && argc!=5 && argc!=7){
		std::cout << "ERROR[1][1]- Wrong Number of Parameters!" << std::endl;
		return ERROR;
	}
        int checkErr=getFromCommandLine(argv,argc,travel_path,algorithm_path ,output);
	if (checkErr==ERROR){
		std::cout << "ERROR[1][2]- Wrong Parameters foramt! or A missing -travel_path argument" << std::endl;
		std::cout << "did you mean : simulator [-travel_path <path>] [-algorithm_path <algorithm path>] [-output <output path>]" << std::endl;
		return ERROR;
	}
	//getting the path
	workPath=travel_path;
	errorOutputPath=output;
	//checking the access to the DIR
	const char *cstr = workPath.c_str();
	DIR *fd_path=opendir(cstr);
	if(fd_path==NULL){
		std::cout << "ERROR[1][2]- can't open "<<workPath<<std::endl;
		return ERROR;
	}
	//get the travels
	simulate(fd_path);
	if(fd_errors.is_open()){
		fd_errors.close();	
	}
	
	closedir(fd_path);	
	

	cout << "Done!"<<endl;
	return 0;
}

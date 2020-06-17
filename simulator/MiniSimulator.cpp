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
#include <vector>
#include<thread>
#include<mutex>
#include<atomic>
#include<optional>
#include<functional>
#include<chrono>
#include "WeightBalanceCalculator.cpp"
#include "../Common/IOHandler.cpp"
#include "AlgorithmRegistrar.h"
#include "../Interfaces/AbstractAlgorithm.h"
#include <algorithm>
#define LAST 1
#define REGULAR 0
using namespace std;
const char* EXPECTED_OUTPUT="/expected_output";

std::mutex mtx;

string travel_path="",algorithm_path="",output="", num_threads="";
//queue<string> algoQueue;
std::vector<string> algoVector;
queue<string> travelQueue;
vector<string> travelVector;
vector<std::pair<string,string>> travelAlgoPairs;
std::map<string,std::vector<int>> resMap;
int travelNum=0;
std::map<std::thread::id,std::vector<std::pair<string,string>>*> threadsCount;
std::map<string,int> regestrationMapIndexeis;
/*---------------FUNC DEC-------------*/
int getFiveElementsIntoArray(string line,int& seek,string* fivedArray,int INDICATOR);

/*------------------DEBUGGING METHODS-------------------*/
int GetRandom(int max){
    srand(time(NULL));
    return rand() % max;
}
 

void printFiles(DIR* fd){
	struct dirent *entry;
    	while ((entry = readdir(fd)))
		if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
	}
}

//aubaida 
/* printing to the output+"/"+"simulation.results*/
void parseResults (std::pair<string,std::vector<int>> const &pair, bool isFirstLine){
	//static int sum;
	if(!fd_results.is_open()&& isFirstLine){
		fd_results.open(output+"/"+"simulation.results");
		fd_results<<"RESULTS,	";
		while(!travelQueue.empty()){
			fd_results<<travelQueue.front()<<",	";
			travelQueue.pop();
		}
		fd_results<<"Sum,	"<<"NumErrors"<<endl;
	}
	if(!isFirstLine){
		fd_results<<pair.first<<",	";
		auto v=pair.second;
		for(int i=0;i<travelNum;i++){
			fd_results<<v[i+2]<<",		";
		}
		fd_results<<v[0]<<",		";
		fd_results<<v[1]<<endl;
	}	
}

/*we sort the results from the map and the n print them*/
void printResults(){
	bool isFirstLine=true;
	std::pair<string,std::vector<int>> x=pair<string,std::vector<int>>();

	parseResults (x,isFirstLine);

	std::vector<std::pair<string,std::vector<int>>> vec;
	std::copy(resMap.begin(),resMap.end(),std::back_inserter<std::vector<pair<string,std::vector<int>>>>(vec));

	//update error num and sum
	for (auto &pair: vec) {
		int errNum=0;
		int Sum=0;	
		auto vector1=pair.second;
		for(auto& n : vector1){
			Sum+=n;
			if(n<0){
				errNum+=1;
			}
		}
		pair.second[0]=Sum;
		pair.second[1]=errNum;
	}
	//sort
	std::sort(vec.begin(), vec.end(),
			[](pair<string,std::vector<int>>& l, pair<string,std::vector<int>>& r) {
				if(l.second[1]<r.second[1]){
					return l.second > r.second;
				}
				if(l.second[1]>r.second[1]){
					return l.second < r.second;
				}
				//equals
				if(l.second[0]<r.second[0]){
					return l.second > r.second;
	
				}
					return l.second < r.second;
	


			});

	for (auto pair: vec) {
		
		parseResults (pair,!isFirstLine);
	}	
}

/*-------------------------------------------------------------------------------------------------------------*/

int sizeOfArray(Container *array) {
		int c = 0;
		while (true) {
			if (array[c].uniqueId == "last")
				return c;
			c++;
		}
		return 0;
}

int getNumOfInstructions(string** instructions){
	int opreations=0;
	for(int i=0;instructions[i][0]!="last";i++){
		string * array=instructions[i];
		if(array[0]!="R"){
			opreations++;
		}
	}
	return opreations;
	
}

void fillShipMap(string input,Ship *ship){
	Container* containers=parseCargoFile(input);
	for(int i=0;containers[i].uniqueId!="last";i++){
		ship->contMap->erase(containers[i].uniqueId);
		ship->contMap->insert(std::pair<std::string,std::string>(containers[i].uniqueId,containers[i].destPort.toString()));
	}

}

/*this function checks if the instrucctions that we got from the algorithm is readable and that every Load done after Unload operation
and also checks that in each stop we rejected or loaded the cargo*/
int isItFineInstructions(string** instructions,string file_name , int routeIndex,int &routeSize){
	Container* containers=parseCargoFile(file_name);

	int sizeArray=sizeOfArray(containers);
	int flag_load=0;
	std::set<int> indexes;
	int last_port=0;


	if(routeSize-1==routeIndex){
		last_port=1;
	}
	for(int i=0;instructions[i][0]!="last";i++){
		string * array=instructions[i];
		if(array[0]=="L"){
			flag_load=1;
		}

		if(last_port){
			if(array[0]=="L" || array[0]=="M"){//no R/L actions in the last Port 
				handleError(output,"Simulator","ERROR : algorithm set LOAD instructions to the last port ");
					return ERROR;
			}
		}

		if(!last_port){
			if(array[0]=="U" && flag_load){
				handleError(output,"Simulator","ERROR : algorithm sets UNLOAD instruction after LOAD ");
				return ERROR;
			}
		}
				
	}
	//if it is the last port all the operation will be L/R	
	if(!last_port){ 
		for(int index=0;containers[index].uniqueId!="last";index++){
			for(int i=0;instructions[i][0]!="last";i++){
				string * array=instructions[i];

				if(array[1]==containers[index].uniqueId && (array[0]=="L" || array[0]=="R") && indexes.count(i)==0){
					indexes.insert(i);
					break;
					}
				}
		}
		if(sizeArray==(int)indexes.size()){
			return SUCCESS;	
		}
		handleError(output,"Simulator","ERROR : algorithm must contain a LOAD/REJECT instruction for each awaiting cargo at the port(not encluding the last port)");
		return ERROR;
	}
	return SUCCESS;
}

// rejection test
int isRejected(node currentContainer, Ship *ship,Port* route, int routeIndex) {
		int error = 0;
		error = error | StowageTester::isInRoute(currentContainer.container->destPort.toString(),route,routeIndex);	// is the container's destination NOT port in route?
		//cout<<"isInRoute="<<error <<"with :"<<currentContainer.container->destPort.toString()<<","<<routeIndex<<endl;
		error = error | CraneTester::isValidId(currentContainer.container->uniqueId);						// is the container's unique ID invalid?
		//cout<<"isValidId="<<error<<endl;
		error = error | CraneTester::isDuplicateIdOnShip(ship->planMap,currentContainer.container->uniqueId);				// is the container's unique ID already on the ship?
		//cout<<"isDuplicateIdOnShip="<<error<<endl;
		error= error | CraneTester::isFull(ship);
		//cout<<"isFull="<<error<<endl;
		error = error | CraneTester::isLegalWeight(currentContainer.container->weight);					// is the container's weight illegal?
		//cout<<"isLegalWeight="<<error<<endl;
		return error;
}

/*get the port of the container*/
string getContainerPort(string name,Ship* ship){
	if(ship->contMap->empty()){

		return "";
	}
	std::map<std::string,std::string>::iterator it=ship->contMap->find(name);
	if(it!=ship->contMap->end()){
		return it->second;
	}

	return "";
}

/*this is the validation algorithm .. after we got the instructions from the algorithm we know want to check if the 
instructions are valid otherwise we say that the algorithm fails*/
int validateAlgorithm(std::string **currentInstructions, Port port, Ship *ship,Port* route, int routeIndex ){
	int error = 0;
	struct node currentContainer;
	int row, column,floor;
	set<string *> rejectedElementsByAlgo; 
	Crane crane = Crane(ship);
	for(int i=0;currentInstructions[i][0]!="last";i++){
		//all the operations must be M/L/U/R
		if(currentInstructions[i][0] != "U" && currentInstructions[i][0] != "R" && currentInstructions[i][0] != "L"  && currentInstructions[i][0] != "M"){
			return -1;
		}	
		
		//if we see R we put it in map so we will check if really should be rejected after the Loads/unloads done
		if(currentInstructions[i][0] == "R"){
			rejectedElementsByAlgo.insert(currentInstructions[i]);
			continue;
		}
		
		string containerPort=port.toString();
		containerPort=getContainerPort(currentInstructions[i][1],ship);
		*currentContainer.container = Container(0,Port(containerPort),currentInstructions[i][1]);

		try{//is it a number?
			floor = std::stoi(currentInstructions[i][2]);
			row = std::stoi(currentInstructions[i][3]);
			column = std::stoi(currentInstructions[i][4]);
		}catch(...){
			return -1;
		}

		//if "M"
		if(currentInstructions[i][0]=="M"){
			//unload then load;
			if(ship->planMap->find(currentContainer.container->uniqueId)==ship->planMap->end()){
				return -1;//the unloaded container not on the ship
			}
			int *dimensions = ship->planMap->find(currentContainer.container->uniqueId)->second;
		
			error =CraneTester::isValidUnloadSimulation(row, column,dimensions[0], dimensions[1],dimensions[2],ship,currentContainer.container);
			// Can we unload legally?
			if (error == 0) {
				crane.unload(*(currentContainer.container),row, column,ship->planLinkedList[row][column].size);
				ship->planLinkedList[row][column].size--;

			}
			else{
				std::cout << "Illegal algorithm command: Could not unload container " << currentContainer.container->uniqueId << "\n";
				return -1;
			}
			try{//is it a number?
			floor = std::stoi(currentInstructions[i][5]);
			row = std::stoi(currentInstructions[i][6]);
			column = std::stoi(currentInstructions[i][7]);
			}catch(...){
				return -1;
			}
			if (ship->planLinkedList[row][column].size <floor && floor < ship->planLinkedList[row][column].maxHeight) {
				// Was the load valid?
				if(error == 0){
					crane.load(currentContainer.container, row,column,ship->planLinkedList[row][column].size);
				}
				else{
					std::cout << "Illegal algorithm command: Invalid load for container " << currentContainer.container->uniqueId << "\n";
					return -1;

				}
			}
		}


		//if "L"
		if(currentInstructions[i][0] == "L"){
			//"L"
			// Should the container be rejected but wasn't rejected by the algorithm?
			error=isRejected(currentContainer,ship,route,routeIndex);
			if(error != 0 ){
				std::cout << "Illegal algorithm command: Container " << currentContainer.container->uniqueId << " should have been rejected \n";
				return -1;
			}
			// Does the container exceed ship height limit?
			if (ship->planLinkedList[row][column].size < ship->planLinkedList[row][column].maxHeight) {
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
		
		// //if "U"?
		if(currentInstructions[i][0] == "U"){
			if(ship->planMap->find(currentContainer.container->uniqueId)==ship->planMap->end()){
				return -1;//the unloaded container not on the ship
			}
			int *dimensions = ship->planMap->find(currentContainer.container->uniqueId)->second;
		
			error =CraneTester::isValidUnloadSimulation(row, column,dimensions[0], dimensions[1],dimensions[2],ship,currentContainer.container);
			// Can we unload legally?
			if (error == 0) {
				crane.unload(*(currentContainer.container),row, column,ship->planLinkedList[row][column].size);
				ship->planLinkedList[row][column].size--;

			}
			else{
				std::cout << "Illegal algorithm command: Could not unload container " << currentContainer.container->uniqueId << "\n";
				return -1;
			}
		}
	}
	//and now after we done loading/unloading we check if really must reject these ccargo containers
	for(auto inst=rejectedElementsByAlgo.begin();inst!=rejectedElementsByAlgo.end();inst++){
		string containerPort=getContainerPort((*inst)[1],ship);
		*currentContainer.container = Container(0,Port(containerPort),(*inst)[1]);
		error=isRejected(currentContainer,ship,route,routeIndex);
		if(error==0){
			return -1;//no need to reject
		}
	}
	return 0;
}

//[14]
/*parsing the instruction line from the algorithm output file*/
int getFiveElementsIntoArray(string line,int& seek,string* fivedArray,int INDICATOR){
	switch(INDICATOR){
		case LAST :{fivedArray[0]="last";fivedArray[1]="";fivedArray[2]="";fivedArray[3]="";fivedArray[4]="";fivedArray[5]="";fivedArray[6]="";fivedArray[7]="";
				return 0;
				break;
				}
		case REGULAR:{
				try{
				int moveFlag=0;
				int err=0;
				getElem(line,seek,',');fivedArray[0]=string(parse_out);
				if(fivedArray[0].compare("M")==0){
					moveFlag=1;
				}
				if(err!=SUCCESS|| seek>=(int)line.length() || parse_out==""){
					return ERROR;
				}
				bool isRejectInstruction=(fivedArray[0]=="R");
				err=getElem(line,seek,',');fivedArray[1]=string(parse_out);
				if(err!=SUCCESS||seek>=(int)line.length() || parse_out==""){
				if(isRejectInstruction && parse_out!="" && seek>=(int)line.length()){
					fivedArray[2]="-1";fivedArray[3]="-1";fivedArray[4]="-1";
					return 0;
				}

					return ERROR;
				}
				err=getElem(line,seek,',');fivedArray[2]=string(parse_out);
				if(err!=SUCCESS|| seek==(int)line.length() || parse_out==""){

					return ERROR;
				}
				err=getElem(line,seek,',');fivedArray[3]=string(parse_out);
				if(err!=SUCCESS|| seek==(int)line.length() || parse_out==""){
					return ERROR;
				}
				if(moveFlag){
					err=getElem(line,seek,'[');fivedArray[4]=string(parse_out);
					if(err!=SUCCESS || seek==(int)line.length() || parse_out==""){
						return ERROR;
					}
					err=getElem(line,seek,',');
					if(seek==(int)line.length() || parse_out.compare("")!=0){
						return ERROR;
					}
					err=getElem(line,seek,',');fivedArray[5]=string(parse_out);
					if(err!=SUCCESS || seek==(int)line.length() || parse_out==""){
						return ERROR;
					}
					err=getElem(line,seek,',');fivedArray[6]=string(parse_out);
					if(err!=SUCCESS || seek==(int)line.length() || parse_out==""){
						return ERROR;
					}
					cout << "the parse out is :"<<parse_out << endl;
					err=getElem(line,seek,']');fivedArray[7]=string(parse_out);
					if(err!=SUCCESS || parse_out.compare("")==0 || (seek<=(int)line.length()&& line.at(seek-1)!=']')){
						return ERROR;
					}
					cout << "the parse out is :"<<parse_out << endl;
					err=getElem(line,seek,',');
					if(err!=ERROR || seek<=(int)line.length()){
						return ERROR;
					}
					
				}else{
					err =getElem(line,seek,',');fivedArray[4]=string(parse_out);
					if(err!=SUCCESS){
						return ERROR;
					}
					err=getElem(line,seek,',');
					if(err!=ERROR || seek<=(int)line.length()){
						return ERROR;
					}
				}
				return 0;
				}catch(...){
					cout << "catched!!!" << endl;
					return ERROR;
				}
				break;
				}	
		default : std::cout<<"ERROR[14][1] : Unknown indicator in getFiveElementsIntoArray"<<std::endl;
			}
	return ERROR;
}

//[13]

/*reading the instructions line after line*/
string** ReadExpectedInstructions(string cargoFileName){
	ifstream fd_info;
	string** expectedInstructions;
	fd_info.open(cargoFileName,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		std::cout << "ERROR[13][1]- can't open "<< cargoFileName<< std::endl;
		return NULL;
	} 
	int lineNum = getNumOfLines(fd_info);
	expectedInstructions = new string*[lineNum+1];//the +1 used as indicator of the last container
	for(int i = 0;i < lineNum+1;i++){
		expectedInstructions[i]=new string[8];
	
	}
	//start reading from file
	int instructionIndex=0;
	string line;
	int seek; 
	int err=0;

	while(getline(fd_info,line)){
		if(line=="" || line.at(0)=='#'){
			continue;
		}else{
			seek=0;
			err=getFiveElementsIntoArray(line,seek,expectedInstructions[instructionIndex],REGULAR);
			if(err==ERROR){
				handleError(output,"Simulator","ERROR : algorithm have a bad format cargo instructions.");
				return NULL;
			}
			instructionIndex++;
		}
	}
	//the last indicates the last of the array
	err=getFiveElementsIntoArray("",seek,expectedInstructions[instructionIndex],LAST);
	if(err==ERROR){
		return NULL;
	}
	
	int index=0;
	while((expectedInstructions[index][0]).compare("last")!=0){
		index++;
	}
	if(fd_info.is_open()){
		fd_info.close();
	}

	return expectedInstructions;
}
//[3] called in [2]
/*here we simulate the  travel ......due to lack of time i didn't seperate this function and document it well .. 
but if i had an one hour i'll do so xD
*/

int simulateTravel(std::pair<string,string> travelAlgoPair,string &travelPath){
	//bool locked = true;
	  //intiate the ship and get the route

/*--DEBUGING--*/
auto it=threadsCount.find(std::this_thread::get_id());
if(it==threadsCount.end()){
	vector<std::pair<string,string>>* v=new vector<std::pair<string,string>>();
	threadsCount.insert({std::this_thread::get_id(),v});
}
it=threadsCount.find(std::this_thread::get_id());
it->second->push_back(travelAlgoPair);

cout<< endl << "DECLEARING of THE EMPTY LIST" << endl<< endl<< endl;
//these are the empty ports that was in the last travel ..
std::set<string>emptyPorts;
char **route;
Ship* ship;
string travelName=travelAlgoPair.first;
//handleError(output,"=#=#=#Simulator running : <"+ travelName+"> travel=#=#",0);
	string travelPath1 = travelAlgoPair.first;
/**********************in this section we check and initiate the route/ship plan and prepare the simulation********************/
	string shipPlanName;
	string routeName;
	int routeSize;
	int err=getTheFileNameFromTheTravel(travelPath,"ship_plan",shipPlanName);
//	if(locked){
//		std::lock_guard<std::mutex> lck (mtx);
	if(err!=0){
		if(err==-1){
			return 1;
		}
		status isIgnore =handleError(output,"Simulator",err);
		if(isIgnore!=status::Ignore){
			return 1;
		}
	}
	err=getTheFileNameFromTheTravel(travelPath,"route",routeName);
	if(err!=0){
		if(err==-1){
			return 1;
		}
		status isIgnore =handleError(output,"Simulator",err);
		if(isIgnore!=status::Ignore){
			return 1;
		}
	}
	shipPlanName=travelPath+"/"+shipPlanName+".ship_plan";
	routeName=travelPath+"/"+routeName+".route";
	err=initShipPlan(ship,shipPlanName);
	if(err!=0){
		if(err==-1){
			return 1;
		}
		status isIgnore =handleError(output,"Simulator",err);
		if(isIgnore!=status::Ignore){
			return 1;
		}
	}
	err=initRoute(route,routeName,routeSize);
	if(err!=0){
		status isIgnore =handleError(output,"Simulator",err);
		if(isIgnore!=status::Ignore){
			return 1;}
	}

	
	err=checkCargoFiles(travelPath,emptyPorts,route,routeSize);
	if(err!=0){
		if(err==1){
			return 1;
		}
		handleError(output,"Simulator","missing cargo files / there is additional cargo files");
	}
//}

/******************END initiation SECTION****************************/

/******************START in this section we initiate the registrar and get the algorithms*********************/
//new implemntation using algo regestrar
	int flag=1;
	auto& registrar = AlgorithmRegistrar::getInstance();

    	/******************END initiate the registrar and get the algorithms*********************/

	if(flag){
	//adding the travel to the queue
	travelQueue.push(travelName);
	travelNum++;
	cout << endl<<endl<<"#==#==#==#==#==#==#==#NEW_TRAVEL = "<<travelName<< " with "<<travelAlgoPair.second<<"#==#==#==#==#==#==#==#"<<endl<<endl;
	//for each algorithm :
	int AlgoIndex;
	try{
	AlgoIndex =regestrationMapIndexeis.find(travelAlgoPair.second)->second;
	}catch(...){
		return -1;
	}

//std::this_thread::sleep_for (std::chrono::seconds(GetRandom(6)));

	cout<<"#### the AlgoIndex ="<<AlgoIndex <<endl;
    	for (auto algo_iter = registrar.begin();algo_iter != registrar.end(); ++algo_iter) {
	cout<<"#### itreate ="<<AlgoIndex <<endl;
		if(AlgoIndex!=0 || AlgoIndex<0){
			cout<<"	continue" <<endl;
			AlgoIndex--;
			continue;
		}	
		string algoName=travelAlgoPair.second;
		if(algoName == "")
			continue;
		algoVector.push_back(algoName);
		//algoQueue.pop();
		std::cout << "Thread running: " << std::this_thread::get_id() << endl;
		cout << endl<<endl<<"##################### Algo = "<<algoName<<"##################"<<endl<<endl;
		auto algo = (*algo_iter)();
		try{

		err=algo->readShipPlan(shipPlanName);

		}catch(...){
			handleError(output,"Simulator","ERROR : "+algoName+" throws an exception by calling readShipPlan (stop the simulation on this algorithm/travel pair)");
			cout<<"throws an exception by calling readShipPlan"<<endl;
			throw 1;
		}
		if(err!=0){
		status isIgnore =handleError(output,algoName+"/"+travelName,err);
		if(isIgnore!=status::Ignore){
			 continue;}
			}
//readShipRoute
		try{
		
		err=algo->readShipRoute(routeName);
		}catch(...){
			handleError(output,"Simulator","ERROR : "+algoName+" throws an exception by calling readShipRoute(stop the simulation on this algorithm/travel pair)");
			throw 1;
		}

		if(err!=0){
		status isIgnore =handleError(output,algoName+"/"+travelName,err);
		if(isIgnore!=status::Ignore){
			continue;}
			}
		//whightBalance
		WeightBalanceCalculator calculator;
//setWeightBalanceCalculator
		try{
		algo->setWeightBalanceCalculator( calculator);
		}catch(...){
			handleError(output,"Simulator","ERROR : "+algoName+" throws an exception by calling setWeightBalanceCalculator(stop the simulation on this algorithm/travel pair)");
			throw 1;		
		}

		//make directory
		string makeDir=output+"/"+algoName+"_"+travelName+"_crane_instructions";
		const char *cstr = makeDir.c_str();
		int err= mkdir (cstr,0777);
		if(err){	
		std::cout << "ERROR[3][1]- can't make dir with name : "<<makeDir<<std::endl; 
		}
		int firstTiem=1;
		for(int routeIndex = 0; routeIndex < routeSize ; routeIndex++ ){

		string FileNameCarge=getCargoFileName(routeIndex,true,route);
		string FileNameInstruction=getCargoFileName(routeIndex,false,route);
		std::set<string>::iterator it;
		string FileNameCargewithout=getNameWithoutExtinsion(FileNameCarge,'.',"cargo_data");
		it =emptyPorts.find(FileNameCargewithout);
		int isEmpty=0;	
		if(it!=emptyPorts.end()){
			for(it=emptyPorts.begin() ;it!=emptyPorts.end();it++){
			}
			string makeDirc=output+"/"+"emptyFiles";
			const char *cstr = makeDirc.c_str();
			mkdir (cstr,0777);
			ofstream fd;
			fd.open(output+"/"+"emptyFiles/"+FileNameCarge);
			isEmpty=1;
		}
		string input;
		if(isEmpty){
		input=output+"/"+"emptyFiles/"+FileNameCarge;
		handleError(output,"Simulator","missing cargo files :"+FileNameCarge +"sending empty file");
		}else{
		input=travelPath+"/"+FileNameCarge;
		}
		fillShipMap(input,ship);
		try{
		err=algo->getInstructionsForCargo(input,makeDir+"/"+FileNameInstruction);
		}catch(...){
			handleError(output,"Simulator","ERROR : "+algoName+" throws an exception by calling getInstructionsForCargo(stop the simulation on this algorithm/travel pair)");
			continue;		
		}

		
		if(err!=0){
		status isIgnore =handleError(output,algoName+"/"+travelName,err);
		if(isIgnore!=status::Ignore){
			continue;}
			}

		/********************Start in this section we validate the route*************/
		//validate 
		string **instructions=ReadExpectedInstructions( output+"/"+makeDir+"/"+FileNameInstruction);
		ifstream fd_info;
		fd_info.open( output+"/"+makeDir+"/"+FileNameInstruction,ios_base::in);//open the file
		//checking the access to the file
		int numOfInstructions=0;
	try{
			if(!fd_info){
			handleError(output,"Simulator","ERROR- can't open the algorithm instruction file :"+ output+"/"+makeDir+"/"+FileNameInstruction);
			throw 1;
			}
			
		if(instructions!=NULL){
			err=isItFineInstructions(instructions,input,routeIndex,routeSize);
			if(err!=ERROR){
				numOfInstructions=getNumOfLines(fd_info);//get container size
				err=0;
				Port* ports=getPortsFromRoute(route,routeSize);
				 err= validateAlgorithm(instructions, ports[routeIndex], ship, ports, routeIndex);
				if(err==0 && routeIndex ==routeSize-1 && !(ship->planMap->empty())){
					//ship must be empty
					err=-1;
				}
			}
		}else{
			err=ERROR;
		}
}catch(int e){
	err=e;
}
/********************END validaton the route*************/


/********************Start in this section we preapare the results*************/

			//results parsing 
			auto isThere=resMap.find(algoName);

			if(isThere==resMap.end()){
			//add algo to the map

			std::vector<int> v=vector<int>();
			v.push_back(0);
			v.push_back(0);
			resMap[algoName]=v;
		
		}
			numOfInstructions=getNumOfInstructions(instructions);

			if(firstTiem==1){
				isThere=resMap.find(algoName);
				(isThere->second).resize(travelNum+2);

				(isThere->second).push_back(0);
				firstTiem=0;
			}
			if(err!=0 || (isThere->second)[travelNum+1]<0){
				(isThere->second)[travelNum+1]=-1;
			}else{
					(isThere->second)[travelNum+1]+=numOfInstructions;
			}
		
			}
			
/********************END preaparing the results*************/
		
		}	
	}
	return SUCCESS;
	
}

int pairingTravelAlgo(DIR* fd){
	struct dirent *entry;
	string travelPath;
	string travelName;
    while ((entry = readdir(fd))){
      	if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
		  	travelPath=workPath+"/"+string(entry->d_name);
		  	const char *cstr = travelPath.c_str();
		  	DIR* fd_travel=opendir(cstr);
		  	if(fd_travel==NULL){
			  std::cout << "ERROR[2][1]- can't open "<<entry->d_name<<std::endl; 
			}else{
				travelName=string(entry->d_name);	// specific travel
				travelVector.push_back(travelName);
				closedir(fd_travel);
			}
		}
	}
	if(fd_results.is_open()){
		fd_results.close();	
	}
	int AlgoIndex=0;
	auto& registrar = AlgorithmRegistrar::getInstance();
    	{	
        	std::string error;
		const char *cstr = algorithm_path.c_str();
		DIR* fd_Algo=opendir(cstr);
		if(fd_Algo==NULL){
			handleError(output,"Simulator","ERROR[3][1]- can't open "+algorithm_path);
			return -1;
		}
		struct dirent *entry;
    		while ((entry = readdir(fd_Algo))){
			if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
				string algoName=getNameWithoutExtinsion(entry->d_name,'.',"so");
				if(algoName.compare("/")!=0){
					algoVector.push_back(algoName);
					if (!registrar.loadAlgorithmFromFile((algorithm_path+"/"+string(entry->d_name)).c_str(), error)) {
	        				std::cerr << error << '\n';
						handleError(output,"Simulator",algoName +" : bad with error : "+error);
            					continue;
					}else{

					regestrationMapIndexeis.insert({algoName,AlgoIndex++});
					}
        			}
			}
		}	
	}
	cout << "travel size " << (int)travelVector.size() << " algo size " << (int)algoVector.size() << endl;
	for(int i=0;i<(int)travelVector.size();i++){
		for(int j=0;j<(int)algoVector.size();j++){
			travelAlgoPairs.push_back(std::make_pair(travelVector[i],algoVector[j]));
		}
	}

	return 0;
}

//[2] called in [1]
void simulate(DIR* fd2, std::pair<string,string> travelAlgoPair){	// fd = ../TRAVELS
    struct dirent *entry;
 /*openning the fd travels*/
	DIR *fd=fd2;	
	string travelName;
	const char *cstr = workPath.c_str();
		fd=opendir(cstr);
		if(fd==NULL){
			handleError(output,"Simulator","ERROR :: can't open the travel path :"+travel_path);
			
		}

    while ((entry = readdir(fd))){
      	if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
		  	//open the travel dir
			string travelPath;
		  	travelPath=workPath+"/"+string(entry->d_name);
		  	const char *cstr = travelPath.c_str();
		  	DIR* fd_travel=opendir(cstr);
		  	if(fd_travel==NULL){
			  std::cout << "ERROR[2][1]- can't open "<<entry->d_name<<std::endl; 
			}else{
				travelName=string(entry->d_name);	// specific travel
				if(travelName == travelAlgoPair.first){
					simulateTravel(travelAlgoPair,travelPath);//simulate the travel
					//free resources
					closedir(fd_travel);
				}
			}
		}
	}
	if(fd_results.is_open()){
		fd_results.close();	
	}
}

/*parse the command line prameters*/
int getFromCommandLine(char *argv[],int argc,string& travel_path,string& algorithm_path ,string& output, string& num_threads){
	for(int i=1;i<argc;i+=2){
		if(string(argv[i]).compare("-travel_path")==0 && travel_path.compare("")==0){
			travel_path=string(argv[i+1]);
		}else if(string(argv[i]).compare("-algorithm_path")==0 && algorithm_path.compare("")==0){
			algorithm_path=string(argv[i+1]);
		}else if(string(argv[i]).compare("-output")==0 && output.compare("")==0){
			output=string(argv[i+1]);
		}else if(string(argv[i]).compare("-num_threads")==0 && num_threads.compare("")==0){
			num_threads=string(argv[i+1]);
			if(std::stoi(num_threads) <= 0){
				output=".";
				return ERROR;
			}
		}
		else{
			output=".";
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
	if(num_threads.compare("")==0){
		num_threads="1";
	}
	return SUCCESS;
}

/*   sample   */
void ExecuteThread(int id){
    // Get current time
    auto nowTime = std::chrono::system_clock::now(); 
    
    // Convert to a time we can output
    std::time_t sleepTime = 
            std::chrono::system_clock::to_time_t(nowTime);
    
    // Convert to current time zone
    tm myLocalTime = *localtime(&sleepTime);
    
    // Print full time information
    std::cout << "Thread " << id << 
            " Sleep Time : " <<
            std::ctime(&sleepTime) << "\n";
    
    // Get separate pieces
    std::cout << "Seconds : " <<
            myLocalTime.tm_sec << "\n\n";
    
    // Put the thread to sleep for up to 3 seconds
    std::this_thread::sleep_for (std::chrono::seconds(GetRandom(3)));
    nowTime = std::chrono::system_clock::now();
    sleepTime = 
            std::chrono::system_clock::to_time_t(nowTime);
    std::cout << "Thread " << id << 
            " Awake Time : " <<
            std::ctime(&sleepTime) << "\n";
    
}
/*--------------EXECUTER-------------*/


template<typename Producer>
class ThreadPoolExecuter {
    Producer producer;	// produces tasks
    const int numThreads = -1;	// number of threads
    std::vector<std::thread> workers;
    std::atomic_bool running = false;
    std::atomic_bool stopped = false;
    static thread_local int num_tasks_finished;	// number of tasks, current thread executed
    std::atomic_int total_num_tasks_finished { 0 };	// number of tasks executed by all threads
    
    // goes over AVAILABLE tasks and executes them for current thread
    void worker_function() {
        while(!stopped) {
            auto task = producer.getTask();
            if(!task) break;
            (*task)();
	   
            ++num_tasks_finished;
            ++total_num_tasks_finished;
        }
        if(stopped) {
            std::cout << std::this_thread::get_id() << " - stopped gracefully after processing " << num_tasks_finished << " task(s)" << std::endl;
        }
        else {
            std::cout << std::this_thread::get_id() << " - finished after processing " << num_tasks_finished << " task(s)" << std::endl;
        }
    }
public:
    ThreadPoolExecuter(Producer producer, int numThreads)
    : producer(std::move(producer)), numThreads(numThreads) {
        workers.reserve(numThreads);
    }
    // gives the tasks to the threads for them to execute
    bool start() {
        bool running_status = false;
        if(!running.compare_exchange_strong(running_status, true)) {
            return false;
        }
        for(int i=0; i<numThreads; ++i) {
            workers.push_back(std::thread([this]{
		
		/*cout<<"workers = ";
		for(auto it=workers.begin();it!=workers.end();it++){
			cout<< (*it).get_id()<<" ,";
		}
		cout << endl;*/
                worker_function();
            }));
        }
        return true;
    }
    void stop_gracefully() {
        stopped = true;
        wait_till_finish();
    }
    void wait_till_finish() {
        for(auto& t : workers) {
            t.join();
        }
        std::cout << "thread pool finished/stopped after processing " << total_num_tasks_finished << " task(s)" << std::endl;
    }
};

// ThreadPoolExecuter.h - being a template - but outside of the class:
template<typename Producer>
thread_local int ThreadPoolExecuter<Producer>::num_tasks_finished { 0 };


/*--------------PRODUCER-------------*/



class SimpleTasksProducer {
    const int numTasks = -1;
    vector<std::pair<string,string>> travelAlgoPairs1;
    DIR* fd;
    std::atomic_int task_counter = 0;
    std::mutex m;
    // 2 ways of getting next task index:


    std::optional<int> next_task_index() {
        for(int curr_counter = task_counter.load(); curr_counter < numTasks; ) {
            if(task_counter.compare_exchange_weak(curr_counter, curr_counter + 1)) {
                return {curr_counter}; // zero based
            }
        }
        return {};
    }
    
public:
    SimpleTasksProducer(int numTasks, vector<std::pair<string,string>> travelAlgoPairs1, DIR* fd): numTasks(numTasks), travelAlgoPairs1(travelAlgoPairs1), fd(fd) {}
    SimpleTasksProducer(SimpleTasksProducer&& other) : numTasks(other.numTasks),travelAlgoPairs1(other.travelAlgoPairs1), fd(other.fd), task_counter(other.task_counter.load()) {}
    std::optional<std::function<void(void)>> getTask() {
        auto task_index = next_task_index();
        if(task_index) {
            return [task_index, this]{
            	rewinddir(fd);
		try {
			std::lock_guard g{m};
			std::cout << "Thread running: " << std::this_thread::get_id() << endl;
        		int n = task_index.value();
std::this_thread::yield();

			simulate(fd,travelAlgoPairs1[n]);
//std::this_thread::yield();
			//ExecuteThread(n	);

                	

    		}catch(const std::bad_optional_access& e) {
        		std::cout << "thread error "<<e.what() << '\n';
    		}
            };
        }
        else return {};
    }
};


/* argv[1] will be the path of the workspace(IO-Files)*/
//[1]
int main(int argc, char *argv[]) {
	DIR *fd_path;
	int flag=0;
	try{
		if(argc!=3 && argc!=5 && argc!=7 && argc!=9){
			std::cout << "ERROR[1][1]- Wrong Number of Parameters!" << std::endl;
			return ERROR;
		}
    	int checkErr=getFromCommandLine(argv,argc,travel_path,algorithm_path ,output, num_threads);
		if (checkErr==ERROR){
			std::cout << "ERROR[1][2]- Wrong Parameters foramt! or A missing -travel_path argument" << std::endl;
			std::cout << "did you mean : simulator [-travel_path <path>] [-algorithm_path <algorithm path>] [-output <output path>] [-num_threads <num of threads>]" << std::endl;
			handleError(output,"Simulator","ERROR- Wrong Parameters foramt! or A missing -travel_path argument");
			cout<<output<<endl;
			throw 1;
			flag=1;
			return ERROR;
		}
		//getting the path
		workPath=travel_path;
		errorOutputPath=output;
		//checking the access to the DIR
		const char *cstr = workPath.c_str();
		fd_path=opendir(cstr);
		if(fd_path==NULL){
			handleError(output,"Simulator","ERROR :: can't open the travel path :"+travel_path);
			flag=1;
			throw 1;
		}

		cout << pairingTravelAlgo(fd_path);

		if(std::stoi(num_threads)>1){
			ThreadPoolExecuter executer1 {SimpleTasksProducer{(int)travelAlgoPairs.size(), travelAlgoPairs, fd_path},std::stoi(num_threads)};
    			executer1.start();
    			executer1.wait_till_finish();
		}
		else{
			rewinddir(fd_path);
			//start the simulation
			for(int i=0;i<(int)travelAlgoPairs.size();i++){
			simulate(fd_path,travelAlgoPairs[i]);
			}
		}
	}catch(...){	//there is an error with the command line prameters
	}
	if(fd_errors.is_open()){
		fd_errors.close();	
	}
	if(flag){
		return -1;
	}
	//printing the results
	cout<<"results prints"<<endl;
	printResults();
	
	for(auto it=threadsCount.begin();it!=threadsCount.end();it++){
		cout<<"*)in thread "<<(*it).first<< ":"<<endl;
		int index=1;
		for(auto it2=(*it).second->begin();it2!=(*it).second->end();it2++){
			cout <<"	"<<index++<<"- ("<<(*it2).first<<","<<(*it2).second<<")"<<endl;
		}
	
	}
	
	cout << "Done! - ";
	return 0;
}


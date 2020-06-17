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
string travelName;
string AlgoName="StowageAlgo";
string travel_path="",algorithm_path="",output="";
queue<string> algoQueue;
queue<string> travelQueue;
std::map<string,std::vector<int>> resMap;
int travelNum=0;
/*---------------FUNC DEC-------------*/
int getFiveElementsIntoArray(string line,int& seek,string* fivedArray,int INDICATOR);

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
	for(auto it=resMap.begin();it!=resMap.end();it++){
	cout << it->first <<"<";
	for(auto it2=(it->second).begin();it2!=(it->second).end();it2++){	
	cout << *it2<<",";
	}
	cout<<endl;
	}
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
		cout<<"sum="<<Sum <<endl;
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
/*thius function checks if the instrucctions that we got from the algorithm is readable and that every Load done after Unload operation
and also checks that in each stop we rejected or loaded the cargo*/
int isItFineInstructions(string** instructions,string file_name , int routeIndex){
cout << "file_name is "<<file_name<<endl;
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
		cout << endl<< "SUCCESS"<<endl;
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
		cout<<"isInRoute="<<error <<"with :"<<currentContainer.container->destPort.toString()<<","<<routeIndex<<endl;
		error = error | CraneTester::isValidId(currentContainer.container->uniqueId);						// is the container's unique ID invalid?
		cout<<"isValidId="<<error<<endl;
		error = error | CraneTester::isDuplicateIdOnShip(ship->planMap,currentContainer.container->uniqueId);				// is the container's unique ID already on the ship?
		cout<<"isDuplicateIdOnShip="<<error<<endl;
		error= error | CraneTester::isFull(ship);
		cout<<"isFull="<<error<<endl;
		error = error | CraneTester::isLegalWeight(currentContainer.container->weight);					// is the container's weight illegal?
		cout<<"isLegalWeight="<<error<<endl;
		return error;
	}

	/*get the port of the container*/
string getContainerPort(string name,Ship* ship){
cout<<"here1"<<endl;
	if(ship->contMap->empty()){
cout<<"here1"<<endl;

		return "";
	}
	std::map<std::string,std::string>::iterator it=ship->contMap->find(name);
	if(it!=ship->contMap->end()){
		return it->second;
	}

	return "";
}

//[??]

/*this is the validation algorithm .. after we got the instructions from the algorithm we know want to check if the 
instructions are valid otherwise we say that the algorithm fails*/
int validateAlgorithm(std::string **currentInstructions, Port port, Ship *ship,Port* route, int routeIndex ){
	int error = 0;
	struct node currentContainer;
	int row, column,floor;
	bool MoveFlag=false;
	set<string *> rejectedElementsByAlgo; 
	Crane crane = Crane(ship);
	for(int i=0;currentInstructions[i][0]!="last";i++){
		MoveFlag=false;

		if(currentInstructions[i][0]=="M"){
			MoveFlag=true;
			
		}
		//all the operations must be M/L/U/R
		if(currentInstructions[i][0] != "U" && currentInstructions[i][0] != "R" && currentInstructions[i][0] != "L"  && currentInstructions[i][0] != "M"){
			return -1;
		}	
		cout<< "+-+- VALIDATE the instruction :" <<currentInstructions[i][0]<<","<<currentInstructions[i][1]<<","<<currentInstructions[i][2]
<<","<<currentInstructions[i][3]
<<","<<currentInstructions[i][4];if(MoveFlag){cout<<"[,"<<currentInstructions[i][5]<<","<<currentInstructions[i][6]<<","<<currentInstructions[i][7]<<"]";}
cout<<endl;
		
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
			MoveFlag=true;
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
				cout<<"size=="<< ship->planLinkedList[row][column].size << " , maxHeight="<<ship->planLinkedList[row][column].maxHeight<<endl;
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
		cout<<"**reading expected :"<< expectedInstructions[index][0] << " , "<< expectedInstructions[index][1]<<"  " <<index <<endl;
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
int simulateTravel(){
	  //intiate the ship and get the route
	ErrorCode errCode;
cout<< endl << "DELETING THE EMPTY FILES" << endl<< endl<< endl;
//these are the emoty ports that was in the last travel .. se we clear it
emptyPorts.erase(emptyPorts.begin(),emptyPorts.end());
//handleError(output,"=#=#=#Simulator running : <"+ travelName+"> travel=#=#",0);

/**********************in this section we check and initiate the route/ship plan and prepare the simulation********************/
	cout << "*initShipPlan"<<endl;
	string shipPlanName;
	string routeName;
	int err=getTheFileNameFromTheTravel(travelPath,"ship_plan",shipPlanName);
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
	cout<<"********************the ship plane :"<<travelPath+"/"+shipPlanName<<"/n******************and the route : "<<travelPath+"/"+routeName<<endl;
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
	cout << "*initRoute"<<endl;
	err=initRoute(route,routeName);
	cout << "init route error is "<< err <<endl<<endl;
	if(err!=0){
		status isIgnore =handleError(output,"Simulator",err);
		if(isIgnore!=status::Ignore){
			return 1;}
	}

	cout << "*end_initRoute"<<endl;
	
	err=checkCargoFiles(travelPath);
	if(err!=0){
		if(err==1){
			return 1;
		}
		handleError(output,"Simulator","missing cargo files / there is additional cargo files");
	}
	cout << "*end_checkCargoFiles"<<endl;
	cout << "*end_ports"<<endl;
/******************END initiation SECTION****************************/

/******************START in this section we initiate the registrar and get the algorithms*********************/
//new implemntation using algo regestrar
	int flag=0;
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
    		while ((entry = readdir(fd_Algo)))
		if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
			string algoName=getNameWithoutExtinsion(entry->d_name,'.',"so");
			if(algoName.compare("/")!=0){
//cout << "the algoritim is : "<< algoName <<endl;
				
				if (!registrar.loadAlgorithmFromFile((algorithm_path+"/"+string(entry->d_name)).c_str(), error)) {
	        			std::cerr << error << '\n';
					 handleError(output,"Simulator",algoName +" : bad with error : "+error);
            				continue;
        			}
				algoQueue.push(algoName);
//cout << "*good_regestration"<<endl;
				flag=1;

			}
			
		}
	cout << "*end_registration"<<endl;
/******************END initiate the registrar and get the algorithms*********************/

	closedir(fd_Algo);
    	}
	if(flag){
	//adding the travel to the queue
	travelQueue.push(travelName);
	travelNum++;
	cout<<"******starting the loop over the algos **********"<<endl;
	cout << endl<<endl<<"#==#==#==#==#==#==#==#NEW_TRAVEL = "<<travelName<<"#==#==#==#==#==#==#==#"<<endl<<endl;
	//for each algorithm :
    	for (auto algo_iter = registrar.begin();algo_iter != registrar.end(); ++algo_iter) {	
		string algoName=algoQueue.front();
		algoQueue.pop();
	cout << endl<<endl<<"##################### Algo = "<<algoName<<"##################"<<endl<<endl;
		auto algo = (*algo_iter)();
		try{
		cout<<"start calling readShipPlan"<<endl;

		err=algo->readShipPlan(shipPlanName);
		cout<<"end calling readShipPlan"<<endl;

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
		cout<<"******starting the loop over the ports **********"<<endl;
		int firstTiem=1;
		for(int routeIndex = 0; routeIndex < routeSize ; routeIndex++ ){
		cout<<"******port number = "<<routeIndex<<" **********"<<endl;

		string FileNameCarge=getCargoFileName(routeIndex,true);
		string FileNameInstruction=getCargoFileName(routeIndex,false);
		cout<<"****** getting instructions **********"<<endl;
		std::set<string>::iterator it;
		string FileNameCargewithout=getNameWithoutExtinsion(FileNameCarge,'.',"cargo_data");
		it =emptyPorts.find(FileNameCargewithout);
		int isEmpty=0;	
		if(it!=emptyPorts.end()){
			for(it=emptyPorts.begin() ;it!=emptyPorts.end();it++){
			//cout<< string(*it);
			}
			//cout<<"is empty ??? : "<<it <endl;
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

		
		std::cout << "\n \n \n Error number: " << err << "\n \n \n";
		if(err!=0){
		status isIgnore =handleError(output,algoName+"/"+travelName,err);
		if(isIgnore!=status::Ignore){
			continue;}
			}

		/********************Start in this section we validate the route*************/
		//validate 
		cout<<"instructions "<<endl;
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
			err=isItFineInstructions(instructions,input,routeIndex);
			if(err!=ERROR){
				numOfInstructions=getNumOfLines(fd_info);//get container size
				cout<<"validation "<<endl;
				err=0;
				Port* ports=getPortsFromRoute(route);
				 err= validateAlgorithm(instructions, ports[routeIndex], ship, ports, routeIndex);
				if(err==0 && routeIndex ==routeSize-1 && !(ship->planMap->empty())){
					//ship must be empty
					err=-1;
				}
				//cout<<"VALIDATION==========================="<<validation<<endl;
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
			cout<<"its time to play"<<endl;
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
/*parse the command line prameters*/
int getFromCommandLine(char *argv[],int argc,string& travel_path,string& algorithm_path ,string& output){
	for(int i=1;i<argc;i+=2){
		if(string(argv[i]).compare("-travel_path")==0 && travel_path.compare("")==0){
			travel_path=string(argv[i+1]);
		}else if(string(argv[i]).compare("-algorithm_path")==0 && algorithm_path.compare("")==0){
			algorithm_path=string(argv[i+1]);
		}else if(string(argv[i]).compare("-output")==0 && output.compare("")==0){
			output=string(argv[i+1]);
		}else{
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
	return SUCCESS;
}

/* argv[1] will be the path of the workspace(IO-Files)*/
//[1]
int main(int argc, char *argv[]) {
	DIR *fd_path;
	int flag=0;
	try{
		if(argc!=3 && argc!=5 && argc!=7){
			std::cout << "ERROR[1][1]- Wrong Number of Parameters!" << std::endl;
			return ERROR;
		}
    	int checkErr=getFromCommandLine(argv,argc,travel_path,algorithm_path ,output);
		if (checkErr==ERROR){
			std::cout << "ERROR[1][2]- Wrong Parameters foramt! or A missing -travel_path argument" << std::endl;
			std::cout << "did you mean : simulator [-travel_path <path>] [-algorithm_path <algorithm path>] [-output <output path>]" << std::endl;
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
		//start the simulation
		simulate(fd_path);	
	}catch(...){	//there is an error with the command line prameters
	cout<< "catched xD"<<endl;
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
	
	
	cout << "Done!"<<endl;
	return 0;
}


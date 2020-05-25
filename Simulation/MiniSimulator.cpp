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
//#include "../Algorithms/Stowage.cpp"
#include "../Simulation/WeightBalanceCalculator.cpp"
//#include "../Interfaces/IOHandler.h"//may changed
#include "../Common/IOHandler.cpp"
#include "AlgorithmRegistrar.h"
#include "../Interfaces/AbstractAlgorithm.h"
#include <algorithm>
//#include "../Algorithms/StowageTester.cpp"
#define LAST 1
#define REGULAR 0
using namespace std;
const char* EXPECTED_OUTPUT="/expected_output";
string travelName;
string AlgoName="StowageAlgo";
//Stowage* curAlgo;
string travel_path="",algorithm_path="",output="";
queue<string> algoQueue;
//aubaida 
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
void parseResults (std::pair<string,std::vector<int>> const &pair, bool isFirstLine){
	//static int sum;
cout<<"222222222222222222"<<endl;

	if(!fd_results.is_open()&& isFirstLine){
//cout<<"222222222222222222222"<<endl;

		fd_results.open(output+"/"+"simulation.results");
		fd_results<<"RESULTS,	";
//cout<<"222222222222222222222"<<endl;

		while(!travelQueue.empty()){
//cout<<"222222222222222222222"<<endl;

		fd_results<<travelQueue.front()<<",	";
		travelQueue.pop();
		}
		fd_results<<"Sum,	"<<"NumErrors"<<endl;
	}
	if(!isFirstLine){
//cout<<"333333333333333333333"<<endl;

		fd_results<<pair.first<<",	";
		auto v=pair.second;
		for(int i=0;i<travelNum;i++){
			fd_results<<v[i+2]<<",		";
		}
		fd_results<<v[0]<<",		";
		fd_results<<v[1]<<endl;


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

int isItFineInstructions(string** instructions,string file_name , int routeIndex){
cout<<"1111111111111"<<endl;
	Container* containers=parseCargoFile(file_name);
cout<<"22222222222222222"<<endl;
	int sizeArray=sizeOfArray(containers);
	int flag_load=0;
	std::set<int> indexes;
	int last_port=0;
cout<<"33333333333333333333"<<endl;

	if(routeSize-1==routeIndex){
		last_port=1;
	}
	int numOfInstructins_reject_load=0;
	for(int i=0;instructions[i][0]!="last";i++){
cout<<"44444444444 the i="<<i<<endl;
		string * array=instructions[i];
		if(array[0]=="L"){
			flag_load=1;
		}

		if(last_port){
			if(array[0]!="L" || array[0]!="M"){//no R/U actions in the last Port 
				return ERROR;
			}
		}

		if(!last_port){
			if(array[0]=="U" && flag_load){ return ERROR;}
			if(array[0]=="L" || array[0]=="R"){
				numOfInstructins_reject_load++;
				for(int index=0;containers[index].uniqueId!="last";index++){
cout<<"55555555555555 the index="<<index<<endl;
cout<<"66666666666666 array[i]="<<array[1]<<", with, containers[index].uniqueId="<<containers[index].uniqueId<<endl;
					if(array[1]==containers[index].uniqueId && (array[0]=="L" || array[0]=="R")){
						if(indexes.count(index)!=0){
							return ERROR;
						}
						indexes.insert(index);
						break;
					}
				}
			}
		}
	}

	if(numOfInstructins_reject_load==indexes.size()){
		cout << endl<< "SUCCESS"<<endl;
		return SUCCESS;	
	}
	return ERROR;
}

// rejection test
	int isRejected(node currentContainer, Ship *ship,Port* route, int routeIndex) {
		int error = 0;
		error = error | StowageTester::isInRoute(currentContainer.container->destPort.toString(),route,routeIndex);	// is the container's destination NOT port in route?
		error = error | CraneTester::isValidId(currentContainer.container->uniqueId);						// is the container's unique ID invalid?
		error = error | CraneTester::isDuplicateIdOnShip(ship->planMap,currentContainer.container->uniqueId);				// is the container's unique ID already on the ship?
		error = error | CraneTester::isLegalWeight(currentContainer.container->weight);						// is the container's weight illegal?
		return error;
	}

//[??]
int validateAlgorithm(std::string **currentInstructions, int numOfInstructions, Port port, Ship *ship,Port* route, int routeIndex ){
	int error = 0;
	struct node currentContainer;
	int row, column;
	Crane crane = Crane(ship);
	for(int i=0;i<numOfInstructions;i++){
		if(currentInstructions[i][0] == "U" && currentInstructions[i][0] == "R" && currentInstructions[i][0] == "L"){
return -1;}	
		cout<< "the instruction :" <<currentInstructions[i][0]<<","<<currentInstructions[i][1]<<","<<currentInstructions[i][2]
<<","<<currentInstructions[i][3]
<<","<<currentInstructions[i][4]
<<endl;
		*currentContainer.container = Container(0,port,currentInstructions[i][1]);
		row = std::stoi(currentInstructions[i][2]);
		column = std::stoi(currentInstructions[i][3]);
		// Is the command load/reject?
		if(currentInstructions[i][0] == "L" || currentInstructions[i][0] == "R"){
			error = error | isRejected(currentContainer,ship,route,routeIndex);
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
			error = error | CraneTester::isValidLoad(row, column, ship->planLinkedList[row][column].size, ship->shipWidth, ship->shipLength, ship->planLinkedList[row][column].maxHeight, ship->planMap,currentContainer.container->uniqueId);
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
		if(currentInstructions[i][1] == "U"){
			int *dimensions = ship->planMap->find(currentContainer.container->uniqueId)->second;
			error = error | CraneTester::isValidUnload(row, column,dimensions[0], dimensions[1]);
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
//[14]
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
				if(err!=SUCCESS|| seek==(int)line.length() || parse_out==""){
					return ERROR;
				}
				
				err=getElem(line,seek,',');fivedArray[1]=string(parse_out);
				if(err!=SUCCESS||seek==(int)line.length() || parse_out==""){
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
string** ReadExpectedInstructions(string cargoFileName){
	ifstream fd_info;
	string** expectedInstructions;

	fd_info.open(cargoFileName,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		std::cout << "ERROR[13][1]- can't open "<< cargoFileName<< std::endl;
		return NULL;
	}
cout << "getting num of lines ******"<<endl; 

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
cout << "reading expected ******"<<endl; 
	while(getline(fd_info,line)){
		if(line=="" || line.at(0)=='#'){
			continue;
		}else{
			seek=0;
			err=getFiveElementsIntoArray(line,seek,expectedInstructions[instructionIndex],REGULAR);
			if(err==ERROR){
				cout << "ERRORRRRR :("<< endl;
				return NULL;
			}
			instructionIndex++;
		}
	}

	err=getFiveElementsIntoArray("",seek,expectedInstructions[instructionIndex],LAST);
	if(err==ERROR){
		cout << "ERRORRRRR :("<< endl;
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
cout << "finish expected ******"<<endl; 

	return expectedInstructions;
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
int simulateTravel(){
	  //intiate the ship and get the route
	ErrorCode errCode;

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
	Port* ports = getPortsFromRoute(route);
	cout << "*end_ports"<<endl;

	//parseResults (AlgoName ,travelName ,0 ,0);

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

	closedir(fd_Algo);
    	}
	if(flag){
	travelQueue.push(travelName);
	cout<<"******starting the loop over the algos **********"<<endl;
	cout << endl<<endl<<"#==#==#==#==#==#==#==#NEW_TRAVEL = "<<travelName<<"#==#==#==#==#==#==#==#"<<endl<<endl;
    	for (auto algo_iter = registrar.begin();algo_iter != registrar.end(); ++algo_iter) {	
		string algoName=algoQueue.front();
		algoQueue.pop();
	cout << endl<<endl<<"##################### Algo = "<<algoName<<"##################"<<endl<<endl;
		auto algo = (*algo_iter)();
		err=algo->readShipPlan(shipPlanName);
		if(err!=0){
		status isIgnore =handleError(output,algoName+"/"+travelName,err);
		if(isIgnore!=status::Ignore){
			return 1;}
			}

		err=algo->readShipRoute(routeName);
		if(err!=0){
		status isIgnore =handleError(output,algoName+"/"+travelName,err);
		if(isIgnore!=status::Ignore){
			return 1;}
			}
		//whightBalance
		WeightBalanceCalculator calculator;
		algo->setWeightBalanceCalculator( calculator);

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
		err=algo->getInstructionsForCargo(input,makeDir+"/"+FileNameInstruction);
		std::cout << "\n \n \n Error number: " << err << "\n \n \n";
		if(err!=0){
		status isIgnore =handleError(output,algoName+"/"+travelName,err);
		if(isIgnore!=status::Ignore){
			return 1;}
			}

		
		//validate 
		cout<<"instructions "<<endl;
		string **instructions=ReadExpectedInstructions( output+"/"+makeDir+"/"+FileNameInstruction);
cout<<"11111111111111111 "<<endl;
		ifstream fd_info;
		fd_info.open( output+"/"+makeDir+"/"+FileNameInstruction,ios_base::in);//open the file
		//checking the access to the file
			if(!fd_info){
			std::cout << "ERROR[8][1]- can't open "<< output+"/"+makeDir+"/"+FileNameInstruction<< std::endl;
			}
			int numOfInstructions;
		if(instructions!=NULL){
cout<<"isItFineInstructions starts--------------"<<endl;
			err=isItFineInstructions(instructions,input,routeIndex);
cout<<"isItFineInstructions ends ----------"<<endl;
			if(err!=ERROR){
				numOfInstructions=getNumOfLines(fd_info);//get container size
				cout<<"validation "<<endl;
				 err=0;//= validateAlgorithm(instructions, numOfInstructions, ports[routeIndex], ship, ports, routeIndex);
				//cout<<"VALIDATION==========================="<<validation<<endl;
			}
		}else{
			err=ERROR;
		}

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

		//if(err){numOfInstructions=-1;}//new
		if(firstTiem==1){
			isThere=resMap.find(algoName);
			(isThere->second).resize(travelNum+3);

			(isThere->second).push_back(0);
			firstTiem=0;}
			if(err!=0 || (isThere->second)[travelNum+2]<0){
				(isThere->second)[travelNum+2]=-1;
				}else{
					(isThere->second)[travelNum+2]+=numOfInstructions;
				}
		
			}


		
		}	
	}
	
	travelNum++;
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
	//printing the results
	cout<<"results prints"<<endl;
	for(auto it=resMap.begin();it!=resMap.end();it++){
	cout << it->first <<"<";
	for(auto it2=(it->second).begin();it2!=(it->second).end();it2++){	
	cout << *it2<<",";
	}
	cout<<endl;
	}
	bool isFirstLine=true;
	std::pair<string,std::vector<int>> x=pair<string,std::vector<int>>();
cout<<"111111111111111111111"<<endl;

	parseResults (x,isFirstLine);
cout<<"111111111111111111111"<<endl;

	std::vector<std::pair<string,std::vector<int>>> vec;
	std::copy(resMap.begin(),resMap.end(),std::back_inserter<std::vector<pair<string,std::vector<int>>>>(vec));
cout<<"111111111111111111111"<<endl;

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
	/*string **instructions=ReadExpectedInstructions( "/specific/a/home/cc/students/cs/aubaidaasad/ex2_c++/may15/git/ContainerShip/output/AAAAA_1.crane_instructions");
	for(int i=0;instructions[i][0]!="last";i++){
		string * array=instructions[i];
		cout<<array[0]<<","<<array[1]<<","<<array[2]<<","<<array[3]<<","<<array[4]<<"[,"<<array[5]<<","<<array[6]<<","<<array[7]<<"]"<<endl;
	}*/

	
	cout << "Done!"<<endl;
	return 0;
}

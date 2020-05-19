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
#include "../Algorithms/Stowage.cpp"
#include "../Simulation/WeightBalanceCalculator.cpp"
//#include "../Interfaces/IOHandler.h"//may changed
#include "../Common/IOHandler.cpp"
#define LAST 1
#define REGULAR 0
using namespace std;
const char* EXPECTED_OUTPUT="/expected_output";
string travelName;
string AlgoName="StowageAlgo";
Stowage* curAlgo;
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
void getFiveElementsIntoArray(string line,int& seek,string* fivedArray,int INDICATOR){
	switch(INDICATOR){
		case LAST :{fivedArray[0]="last";fivedArray[1]="";fivedArray[2]="";fivedArray[3]="";fivedArray[4]="";
				break;
				}
		case REGULAR:{	getElem(line,seek,',');fivedArray[0]=string(parse_out);
				getElem(line,seek,',');fivedArray[1]=string(parse_out);
				getElem(line,seek,',');fivedArray[2]=string(parse_out);
				getElem(line,seek,',');fivedArray[3]=string(parse_out);
				getElem(line,seek,',');fivedArray[4]=string(parse_out);
				break;
				}	
		default : std::cout<<"ERROR[14][1] : Unknown indicator in getFiveElementsIntoArray"<<std::endl;
			}
}

//[13]
string** ReadExpectedInstructions(char* cargoFileName){
	ifstream fd_info;
	string** expectedInstructions;
	char* expectedPath=new char[strlen(travelPath)+strlen(EXPECTED_OUTPUT)+strlen(cargoFileName)+15];
 	strcpy(expectedPath,travelPath);
	strcat(expectedPath,EXPECTED_OUTPUT);
	strcat(expectedPath,"/");
	strcat(expectedPath,cargoFileName);
	strcat(expectedPath,".expected");

	fd_info.open(expectedPath,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		std::cout << "ERROR[13][1]- can't open "<< expectedPath<< std::endl;
		return NULL;
	}
	int lineNum = getNumOfLines(fd_info);
	expectedInstructions = new string*[lineNum+1];//the +1 used as indicator of the last container
	for(int i = 0;i < lineNum+1;i++){
		expectedInstructions[i]=new string[5];
	
	}
	//start reading from file
	int instructionIndex=0;
	string line;
	int seek; 
	while(getline(fd_info,line)){
		if(line.at(0)=='#'){
			continue;
		}else{
			seek=0;
			getFiveElementsIntoArray(line,seek,expectedInstructions[instructionIndex],REGULAR);
			instructionIndex++;
		}
	}
	getFiveElementsIntoArray("",seek,expectedInstructions[instructionIndex],LAST);
	
	int index=0;
	while((expectedInstructions[index][0]).compare("last")!=0){
		cout<<"**reading expected :"<< expectedInstructions[index][0] << " , "<< expectedInstructions[index][1]<<"  " <<index <<endl;
		index++;
	}
	return expectedInstructions;
}
//[12]
int checkInstructionPerPort(int portIndex,string** algoInstructions){
	char* cargoFileName = getCargoFileName(portIndex);
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
	cout << "*initShipPlan"<<endl;
	  initShipPlan();
	cout << "*initRoute"<<endl;
	  initRoute();
	  //example
	cout<<"printing the cargo file ame"<<endl;
	int check;
	//try

	Port* ports = getPortsFromRoute();
	parseResults (AlgoName ,travelName ,0 ,0);
	for(int routeIndex = 0; routeIndex < routeSize ; routeIndex++ ){
		char* FileName=getCargoFileName(routeIndex);
		Container * containers=parseCargoFile(FileName);
		printContainerArray(containers, FileName);
	 	curAlgo =new Stowage(routeIndex,ship,ports,containers);
	  	string** algoInstructions=curAlgo->currentInstructions;
		instructionsOut(algoInstructions,FileName);
	   	check=checkInstructionPerPort(routeIndex,algoInstructions);
		if(check==SUCCESS){
			parseResults (AlgoName,travelName,numInstructions,routeIndex+1);
		}else{
			parseResults (AlgoName,travelName,numInstructions,0-(routeIndex+1));
		}
	}
	 	
}

//[2] called in [1]
void simulate(DIR* fd){
    struct dirent *entry;
    while ((entry = readdir(fd)))
      if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
		  //open the travel dir
		  travelPath=new char[strlen(workPath)+strlen(entry->d_name)+2];
		  strcpy(travelPath,workPath);
		  strcat(travelPath,"/");
		  strcat(travelPath,entry->d_name);
		  DIR* fd_travel=opendir(travelPath);
		  std::cout <<entry->d_name<<std::endl;
		  if(fd_travel==NULL){
			  std::cout << "ERROR[2][1]- can't open "<<entry->d_name<<std::endl; 
			  }else{
				travelName=string(entry->d_name);
				  simulateTravel();//simulate the travel
				  //free resources
				  closedir(fd_travel);
			          delete[] travelPath;
			  }
	  }
	if(fd_results.is_open()){
		fd_results.close();	
	}
}
	

/* argv[1] will be the path of the workspace(IO-Files)*/
//[1]
int main(int argc, char *argv[]) {

	if(argc!=2){
		std::cout << "ERROR[1][1]- Wrong Number of Parameters!" << std::endl;
		return ERROR;
	}
	//getting the path
	workPath=argv[1];
	//checking the access to the DIR
	DIR *fd_path=opendir(workPath);
	if(fd_path==NULL){
		std::cout << "ERROR[1][2]- can't open "<<workPath<<std::endl;
		return ERROR;
	}
	//get the travels
	simulate(fd_path);

	return 0;
}

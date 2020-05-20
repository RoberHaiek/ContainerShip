
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <fstream>
#include<string.h>
#include <string>
#include <map>
#include "../Interfaces/IOHandler.h"
/*#define SUCCESS 1
#define ERROR 0
#define MAX_LINE 1024
using namespace std;
int width,length,maxHeight;
char *travelPath;
char *workPath;
char **route;
ofstream fd_results;
int routeSize;
const char* SHIP_PLAN="ship_plan";
const char* ROUTE="route";
const char* OUTPUT="/output";

int numInstructions=0;
Ship* ship;
Stowage* curAlgo;*/

/*--------------------------PARSING METHODS--------------------------*/
char* getElem(string s , int& seek,char delmiter){
	parse_out="";
	if(delmiter==' '){//find the first index which not whitspace
		while(seek < (int)s.length() && s.at(seek)==delmiter){
			seek++;
		}
	}
	while(seek < (int)s.length()){
		if(s.at(seek)==delmiter){
			break;
		}
		parse_out=parse_out+s.at(seek++);
	}
	cout <<"getElem : "<< parse_out<<endl;
	seek++;
	return NULL;//must return
}
/*-------------------------------------------------------------------------------------------------------------*/

//[17]

void parseResults (string algoName,string travelName,int numInst, int port){
	static int sum;

	if(!fd_results.is_open()){
		fd_results.open("results.txt");
		fd_results<< "********Travel name= "<<travelName<<"\n\t"<<"Algorithem name="<< algoName<<"\n\t";

	}
	else if(fd_results.is_open() && port==0){
		fd_results<<"---------------------------------------------------------------\n";
		fd_results<< "********Travel name= "<<travelName<<"\n\t"<<"Algorithem name="<< algoName<<"\n\t";
	}
	else{
		sum+=numInst;
		fd_results<< "In port "<< port <<": "<< numInst<<"\n\t";


	}
	if(port==routeSize){
		fd_results<<"Sum: "<< sum<<"\n";
		sum=0;
	}
}


//[11]
Port* getPortsFromRoute(char** &currRoute){
	int index;
	Port* ports=new Port[routeSize+1];
	for(index=0;index<routeSize;index++){
		ports[index]=Port(string(currRoute[index]));
	}
	ports[index]=Port("last");
	return ports;

}
//[10]//not finished yet
int instructionsOut(string** instructions,string outName){
	//open output file to write the instruction to output dir ??????????????????????? we must open a output dir to every algo????
	ofstream fd_info;
	cout << "in out" <<endl;
	numInstructions=0;
	/*char* filePath=new char[strlen(travelPath)+strlen(OUTPUT)+strlen(outName)+6];

	strcpy(filePath,travelPath);
	strcat(filePath,OUTPUT);
	strcat(filePath,"/");
	strcat(filePath,outName);
	strcat(filePath,".out");*/
	string filePath=outName;
	cout << filePath <<endl;
	fd_info.open(filePath,ios_base::out);//open the file to out
	if(!fd_info){
		std::cout << "ERROR[10][1]- can't open "<< filePath<< std::endl;
	}
	int instIndex=0;
	while(instructions[instIndex][0].compare("last")!=0){
		//uid,L/R/U,row,column,height
		numInstructions++;
		fd_info<< instructions[instIndex][0]<<","<<instructions[instIndex][1]<<","<<instructions[instIndex][2]<<","<<instructions[instIndex][3]<<","<<instructions[instIndex][4];
		instIndex++;
		if(instructions[instIndex][0].compare("last")!=0){
			fd_info<<endl;
		}
	} 
	fd_info.close();
	return SUCCESS;
}
//[9]
string getCargoFileName(int portIndex,bool cargoData){
	int cnt=1;
	for(int i=0;i<portIndex;i++){
		if(strcmp(route[portIndex],route[i])==0){
			cnt++;
		}
	}
	/*char* fileName=new char[20];
	strcpy(fileName,route[portIndex]);
	strcat(fileName,"_");
	fileName[strlen(fileName)]='0'+cnt;
	fileName[strlen(fileName)]='\0';*/
	string fileName=string(route[portIndex])+"_"+to_string(cnt);
	if(cargoData==true){
	fileName+=".cargo_data";
	}else{
	fileName+=".crane_instructions";
	}
	return fileName;

}
//[8]
    Container* parseCargoFile(string fileName){
	ifstream fd_info;
	int is_err=SUCCESS;//must change this.........................................
	Container* containers;
	int containerNum;
	string filePath=fileName;
	fd_info.open(filePath,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		std::cout << "ERROR[8][1]- can't open "<< filePath<< std::endl;
	}

	containerNum=getNumOfLines(fd_info);//get container size
	containers=new Container[containerNum+1];
	containers[containerNum]=Container(0,Port(""),"last");//to know we reached the last container
	string line;
	int containerIndex=0;
	int seek;
	string containerUID;
	string containerDstPort;
	int containerWeight;
	while(getline(fd_info,line)){
		if(line.at(0)=='#'){
			continue;
			}
			seek=0;
			getElem(line,seek,',');
			containerUID=string(parse_out);
			getElem(line,seek,',');
			containerWeight=std::stoi(parse_out); // get length
			getElem(line,seek,',');
			containerDstPort=string(parse_out);
			/*is_err=checkUid(containerUID);*/
			if(is_err==ERROR){
				std::cout << "ERROR[8][1]- wrong container uid format  "<< containerUID<< std::endl;
			}
			else{
				Port dstPort(containerDstPort);
				containers[containerIndex]=Container(containerWeight,dstPort,containerUID);
			}
			containerIndex++;
	}
	return containers;
}

//[7] called in [5]
int checkPortName(string name){
	if(name.length()!=5){
		return ERROR;
	}
	char c;
	for(int i=0;i<5;i++){
		c=name.at(i);
		if(c>='A' && c<='Z'){
			continue;
		}else{
			if(c>='a'&& c<='z'){
				name[i]=c+'A'-'a';
			}else{
				return ERROR;
			}
		}
	}
	return SUCCESS;
}
//[6] called in [5]
int getNumOfLines(ifstream& fd){
	string line;
	int numOfLines=0;
	while(getline(fd,line)){
		if(line.at(0)=='#'){
			continue;
			}
		numOfLines++;
	}
	fd.clear();
	fd.seekg(0,ios::beg);//back to the start
	return numOfLines;
}

//[5] called in [3]
void initRoute(char** &currRoute){
	ifstream fd_info;
	int is_err;
	string filePath=travelPath+"/"+ROUTE;
	fd_info.open(filePath,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		std::cout << "ERROR[5][1]- can't open "<< filePath<< std::endl;
	}
	routeSize=getNumOfLines(fd_info);//get route size
	currRoute=new char*[routeSize];
	for(int i=0;i<routeSize;i++){
		currRoute[i]=new char[5];
	}
	//get the route
	string line;
	int portIndex=0;
	int seek;
	while(getline(fd_info,line)){
		if(line.at(0)=='#'){
			continue;
			}
			seek=0;
			getElem(line,seek);
			is_err=checkPortName(parse_out);
			if(is_err==ERROR){
				std::cout << "ERROR[5][2]- wrong port name "<< parse_out<< std::endl;
			}
			if(is_err!=ERROR){
				strcpy(currRoute[portIndex],parse_out.c_str());
				cout<<currRoute[portIndex]<<endl;
			}
			portIndex++;
	}
}
//[6] called in[4]
void getTripleElem(string line,int& seek,int& firstElem ,int& secElem ,int& thirdElem){
	getElem(line,seek,',');
	firstElem=std::stoi(parse_out); // get max height
	getElem(line,seek,',');
	secElem=std::stoi(parse_out); // get length
	getElem(line,seek,',');
	thirdElem=std::stoi(parse_out);// get width
	cout << firstElem <<","<<secElem<<","<<thirdElem<<endl;
}
//[4] called in [3]
void initShipPlan(Ship* &currShip){
	ifstream fd_info;
	/*char* filePath=new char[strlen(travelPath)+strlen(SHIP_PLAN)+2];
	strcpy(filePath,travelPath);
	strcat(filePath,"/");
	strcat(filePath,SHIP_PLAN);*/
	string filePath=travelPath+"/"+SHIP_PLAN;
	fd_info.open(filePath,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		std::cout << "ERROR[4][1]- can't open "<< filePath<< std::endl;
	}
	//start parsing
	string line;
	bool firstLine=true;

	while(getline(fd_info,line)){
		if(line.at(0)=='#'){
			continue;
			}

	int seek =0;
	cout << "the parsing line : " <<line<<endl;
	if(firstLine){
		getTripleElem(line,seek,maxHeight,width,length);//each seprated with coma
		/*
		 intiate the ship
		*/
		cout<<"initiate the ship :"<<width<<", "<<length<<", "<<maxHeight<<endl;
		currShip=new Ship(width,length,maxHeight);
		cout << "ship done init"<<endl;

		firstLine=!firstLine;
		}else{
		int x,y,floors;

		getTripleElem(line,seek,x,y,floors);//each seprated with coma
		if(floors<maxHeight){// you can check for an error format
			/*
			update ship plan
			*/
			(*currShip).setHeight(x,y,floors);
			}
		}
	}
}
/**********implement some funcs*********/

	int getRouteIndex(int &routeIndex,const std::string& input_full_path_and_file_name){
		int seek=0;
		while(seek < (int)input_full_path_and_file_name.length()){
		getElem(input_full_path_and_file_name,seek,'/');
		}
		seek=0;
		string fileName(parse_out);
		getElem(fileName,seek,'_');
		string portName(parse_out);
		getElem(fileName,seek,'.');
		std::cout<<"the parse_out is :"<<parse_out<<std::endl;
		int portNum=std::stoi(parse_out);
		routeIndex=0;
		while(true){
			if(portName.compare(route[routeIndex])==0){
				portNum--;	
			}
			if(portNum==0){
				break;
			}
			routeIndex++;
		}

		return 0;
	}
	string getTheFileName(string fullFilePath){
		int seek=0;
		string fileName;
		cout<<"in getTheFileName: "<<seek<<endl;
		while(seek < (int)fullFilePath.length()){
		getElem(fullFilePath,seek,'/');
		}
		fileName=string(parse_out);
		cout<<"for npw the filename is :"<<fileName<<endl;

		return fileName;

	}
	/*****************************************/

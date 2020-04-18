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
#include "Container.cpp"
#include "Ship.cpp"
#define SUCCESS 1
#define ERROR 0
#define MAX_LINE 1024
using namespace std;
int width,length,maxHeight;
char *workPath;
char *travelPath;
char **route;
int routeSize;
const char* SHIP_PLANE="ship_plan";
const char* ROUTE="route";
const char* OUTPUT="/output";
const char* EXPECTED="/expected_output";
char* parse_out=new char[MAX_LINE];
Ship* ship;

/*---------------FUNC DEC-------------*/
int getNumOfLines(ifstream& fd);
/*------------------DEBUGGING METHODS-------------------*/
void printFiles(DIR* fd){
	struct dirent *entry;
    while ((entry = readdir(fd)))
		if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
			std::cout <<entry->d_name<<std::endl;
	}
}
void printContainerArray(Container** arr,char* fileName){
	int index=0;
	Container* pt=arr[index];
	std::cout << "-list of containers in " <<fileName<<" :"<<std::endl;
	while(pt!=NULL){
		std::cout <<"	"<< pt->uniqueId <<" ,"<<pt->weight<<" ,"<<pt->destPort.toString()<<std::endl;
		pt=arr[++index];
	}
	
}

/*--------------------------PARSING METHODS--------------------------*/
char* getElem(string s , int& seek,char delmiter=' '){
	int index=0;
	if(delmiter==' '){//find the first index which not whitspace
		while(seek < s.length() && s.at(seek)==delmiter){
			seek++;
		}
	}
	while(seek < s.length()){
		/*if(delmiter==','){
				cout << s.at(seek)<<endl;
			}*/
		if(s.at(seek)==delmiter){
			break;
		}
		parse_out[index++]=s.at(seek++);
	}
	parse_out[index]='\0';
	seek++;
}
//[10]
int instructionsOut(string** instructions,char* outName){
	//open output file to write the instruction to output dir ??????????????????????? we must open a output dir to every algo????
	ifstream fd_info;
	char* filePath=new char[strlen(travelPath)+strlen(OUTPUT)+strlen(outName)+5];
	strcpy(filePath,travelPath);
	strcat(filePath,OUTPUT);
	strcat(filePath,"/");
	strcat(filePath,outName);
	strcat(filePath,".out");
	fd_info.open(filePath,ios_base::out);//open the file to out
	int instIndex=0;
	while(instructions[instIndex]!=NULL){
		return 0;
	}
	delete[] filePath;
	fd_info.close();
	return 0;
}
//[9]
char* getCargoFileName(int portIndex){
	int cnt=0;
	for(int i=0;i<portIndex;i++){
		if(strcmp(route[portIndex],route[i]==0)){
			cnt++;
		}
	}
	char* fileName=new char[20];
	strcpy(fileName,route[portIndex]);
	strcat(fileName,"_");
	fileName[strlen(fileName)-1]='0'+cnt;
	fileName[strlen(fileName)]='\0';
	strcat(fileName,".cargo_data");
	return fileName;
	
}
//[8] 
    Container** parseCargoFile(char* fileName){
	ifstream fd_info;
	int is_err=SUCCESS;//must change this.........................................
	Container** containers;
	int containerNum;
	char* filePath=new char[strlen(travelPath)+strlen(fileName)+2];
	strcpy(filePath,travelPath);
	strcat(filePath,"/");
	strcat(filePath,fileName);
	fd_info.open(filePath,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		std::cout << "ERROR[8][1]- can't open "<< filePath<< std::endl;
	}
	
	containerNum=getNumOfLines(fd_info);//get container size
	containers=new Container*[containerNum+1];
	containers[containerNum]=NULL;//to know we reached the last container
	string line;
	int containerIndex=0;
	int seek;
	char* containerUID=new char[MAX_LINE];
	char* containerDstPort=new char[MAX_LINE];
	int containerWeight;
	while(getline(fd_info,line)){
		if(line.at(0)=='#'){
			continue;
			}
			seek=0;
			getElem(line,seek,',');
			strcpy(containerUID,parse_out);
			getElem(line,seek,',');
			containerWeight=std::stoi(parse_out); // get length
			getElem(line,seek,',');
			strcpy(containerDstPort,parse_out);
			/*is_err=checkUid(containerUID);*/
			if(is_err==ERROR){
				std::cout << "ERROR[8][1]- wrong container uid format  "<< containerUID<< std::endl;
			}
			else{
				Port dstPort(containerDstPort);
				containers[containerIndex]=new Container(containerWeight,dstPort,containerUID);
			}
			containerIndex++;
	}
	return containers;
}

//[7] called in [5]
int checkPortName(char* name){
	if(strlen(name)!=5){
		return ERROR;
	}
	char c;
	for(int i=0;i<5;i++){
		c=name[i];
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
	bool firstLine=true;
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
void initRoute(){
	ifstream fd_info;
	int is_err;
	char* filePath=new char[strlen(travelPath)+strlen(ROUTE)+2];
	strcpy(filePath,travelPath);
	strcat(filePath,"/");
	strcat(filePath,ROUTE);
	fd_info.open(filePath,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		std::cout << "ERROR[5][1]- can't open "<< filePath<< std::endl;
	}
	routeSize=getNumOfLines(fd_info);//get route size
	route=new char*[routeSize];
	for(int i=0;i<routeSize;i++){
		route[i]=new char[5];
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
				strcpy(route[portIndex],parse_out);
				cout<<route[portIndex]<<endl;
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
void initShipPlan(){
	ifstream fd_info;
	char* filePath=new char[strlen(travelPath)+strlen(SHIP_PLANE)+2];
	strcpy(filePath,travelPath);
	strcat(filePath,"/");
	strcat(filePath,SHIP_PLANE);
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
	cout << "the parsing line" <<line<<endl;
	if(firstLine){
		getTripleElem(line,seek,maxHeight,length,width);//each seprated with coma
		/*
		 intiate the ship
		*/
		ship=new Ship(width,length,maxHeight);
		firstLine=!firstLine;	
		}else{
		int x,y,floors;
		Ship ourShip=*(ship);
		getTripleElem(line,seek,x,y,floors);//each seprated with coma
		if(floors<maxHeight){// you can check for an error format
			/*
			update ship plan
			*/
			ourShip.setHeight(x,y,floors);
			}
		}
	}
}
//[3] called in [2]
void simulateTravel(DIR* fd){
	  //intiate the ship and get the route
	  initShipPlan();
	  initRoute();
	  //example
	/*  char* filePath=new char[strlen(travelPath)+20];
		strcpy(filePath,"AAAAA_1.cargo_data");
		cout << "11111111111111"<<endl;
		Container** arr=parseCargoFile(filePath);
		printContainerArray(arr,filePath);*/
	  /*
	  *
	  init algorithm ? and start to play
	  *
	  */
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
				  simulateTravel(fd_travel);//simulate the travel
				  //free resources
				  closedir(fd_travel);
				  delete[] travelPath;
			  }
	  }
	  exit(0);
}
	

/* argv[1] will be the path of the workspace(IO-Files)*/
//[1]
int main(int argc, char *argv[]) {

	if(argc!=2){
		std::cout << "ERROR[1][1]- Wrong Number of Parameters!" << std::endl;
		return ERROR;
	}
	/*initiate wieght balance???????????????????????????????????????????????????????????*/
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
	
	char *infoFile=new char[strlen(argv[2])+1];
	strcpy(infoFile,argv[2]);
	ifstream fd_info;
	fd_info.open(infoFile,ios_base::in);
	//checking the access to the file
	if(!fd_info){
		std::cout << "ERROR[3]- can't open ";
		std::cout << infoFile<< std::endl;
		return 0;
	}
	
	fd_info.close();
	closedir(fd_path);
	delete[] workPath;
	delete[] infoFile;
	delete[] parse_out;
	return 0;
}

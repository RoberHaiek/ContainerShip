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
#include "Port.cpp"
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
char* parse_out=new char[MAX_LINE];
/*------------------DEBUGGING METHODS-------------------*/
void printFiles(DIR* fd){
	struct dirent *entry;
    while ((entry = readdir(fd)))
		if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
			std::cout <<entry->d_name<<std::endl;
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


/*int parse(ifstream& fd){//known format
	string line;
		bool isParams=true;
		int portIndex=0;
		while(getline(fd,line)){
			if(line.at(0)=='#'){
				continue;
			}
			int seek =0;
			char* ptr;
			if(isParams){//parse and get width,length,height
				ptr=getElem(line,seek);
				width=std::stoi(ptr);// get width
				delete[] ptr;
				ptr=getElem(line,seek);
				length=std::stoi(ptr); // get length
				delete[] ptr;
				ptr=getElem(line,seek);
				maxHeight=std::stoi(ptr); // get max height
				delete[] ptr;
				cout << width <<"\n"<<length<<"\n"<<maxHeight<<endl;
				isParams=false;
			}else{
				ptr=getElem(line,seek,'-');
				string portName=std::string(ptr);
				delete[] ptr;
				cout << portName<<":"<<endl;
				while(true){
					ptr=getElem(line,seek,'[');
					delete[] ptr;
					ptr=getElem(line,seek,',');
					if(ptr[0]=='\0'){
						delete[] ptr;
						break;
					}
					int weight=std::stoi(ptr);
					delete[] ptr;
					ptr=getElem(line,seek,',');
					string uid=std::string(ptr);
					delete[] ptr;ptr=getElem(line,seek,']');
					string dst=std::string(ptr);
					delete[] ptr;
					cout <<weight<<" "+uid+" "+dst<<endl;
				}
			}
		}

	return 0;
}*/

//[8] 
    Container** parseCargoFile(char* fileName){
	ifstream fd_info;
	int is_err=SUCCESS;//must change this.........................................
	Container::Container** containers;
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
	containers=new Container::Container*[containerNum];
	string line;
	int containerIndex=0;
	int seek;
	char* containerUID;
	char* containerDstPort;
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
				Port::Port dstPort=new Port::Port(containerDstPort);
				containers[containerIndex]=new Container::Container(containerWeight,dstPort,containerUID);
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
		firstLine=!firstLine;	
		}else{
		int x,y,floors;
		getTripleElem(line,seek,x,y,floors);//each seprated with coma
		if(floors<maxHeight){
			/*
			update ship plan
			*/
			}
		}
	}
}
//[3] called in [2]
void simulateTravel(DIR* fd){
	  //intiate the ship and get the route
	  initShipPlan();
	  initRoute();
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
	/*initiate wieght balance????*/
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
/* argv[1] will be the path of the workspace(IO-Files)
int main(int argc, char *argv[]) {

	if(argc!=3){
		std::cout << "ERROR[1]- Wrong Number of Parameters!" << std::endl;
		return 0;
	}
	//getting the path
	char *workPath=new char[strlen(argv[1])+1];
	strcpy(workPath,argv[1]);
	//checking the access to the DIR
	DIR *fd_path=opendir(workPath);
	if(fd_path==NULL){
		std::cout << "ERROR[2]- can't open ";
		return 0;
	}
	//getting the infoFile
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


	parse(fd_info); // parse the file and get the needed info.


	fd_info.close();
	closedir(fd_path);
	delete[] workPath;
	delete[] infoFile;
	return 0;
}*/


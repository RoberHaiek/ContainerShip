
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <fstream>
#include<string.h>
#include <string>
#include <map>
#include "../Interfaces/IOHandler.h"
#define SUCCESS 0
#define ERROR 1
#define LAST 1
#define REGULAR 0
#define MAX_LINE 1024
/*int width,length,maxHeight;
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
int getElem(string s , int& seek,char delmiter){
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
	if(delmiter==','){
	parse_out.erase(std::remove_if(parse_out.begin(), parse_out.end(), ::isspace), parse_out.end());
	}
	if(parse_out.compare("")==0){
		return ERROR;
	}
	seek++;
	return SUCCESS;//must return
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
	if(fd_info.is_open()){
	fd_info.close();
	}

	return SUCCESS;
}
//[9]
string getCargoFileName(int portIndex,bool cargoData,char** route){
	int cnt=1;
	for(int i=0;i<portIndex;i++){
		if(strcmp(route[portIndex],route[i])==0){
			cnt++;
		}
	}
if(route==NULL){
		cout<<"route in 0 null"<<endl;
	}
	if(route[portIndex]==NULL){
		cout<<"route is null"<<endl;
	}
	

	cout<<"in getCargoFileName "<<route[portIndex]<<endl;
	string fileName=string(route[portIndex])+"_"+to_string(cnt);
	cout<<"in getCargoFileName "<<fileName<<endl;
	if(cargoData==true){
	fileName+=".cargo_data";
	}else{
	fileName+=".crane_instructions";
	}
	cout<<"exit getCargoFileName "<<fileName<<endl;
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
	if(fd_info.is_open()){
	fd_info.close();
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
int initRoute(char** &currRoute,string travelPath){
	ifstream fd_info;
	int is_err;

	fd_info.open(travelPath,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		std::cout << "ERROR[5][1]- can't open "<< travelPath<< std::endl;
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
	if(fd_info.is_open()){
	fd_info.close();
	}

	return SUCCESS;
}
//[6] called in[4]
int getTripleElem(string line,int& seek,int& firstElem ,int& secElem ,int& thirdElem){
	try{
	int err=getElem(line,seek,',');
	firstElem=std::stoi(parse_out); // get max height
	if(err!=SUCCESS|| firstElem<0){
		return err;
	}
	err=getElem(line,seek,',');
	secElem=std::stoi(parse_out); // get length
	if(err!=SUCCESS|| secElem<0){
		return err;
	}
	err=getElem(line,seek,',');
	thirdElem=std::stoi(parse_out);// get width
	if(err!=SUCCESS || thirdElem<0){
		return err;
	}
	}catch(...){
		cout << "error catched"<<endl;

		return ERROR;
	}
	if(seek-1 < (int)line.length()){
		return ERROR;

	}
	cout << firstElem <<","<<secElem<<","<<thirdElem<<endl;
	return SUCCESS;
}
//[4] called in [3]
int initShipPlan(Ship* &currShip ,string travelPath){
	ifstream fd_info;
	int err=0;
	int isErr=0;	
	fd_info.open(travelPath,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		return err+(int)ErrorID::ShipPlanBadFirstLine;
	}
	//check if empty 
	if(getNumOfLines(fd_info)==0){
		return err+(int)ErrorID::ShipPlanBadFirstLine;
	}
	//start parsing
	string line;
	bool firstLine=true;
	int flag=0;
	while(getline(fd_info,line)){
		if(line.at(0)=='#'){
			continue;
			}

	int seek =0;
	cout << "the parsing line : " <<line<<endl;
	if(firstLine){
		isErr=getTripleElem(line,seek,maxHeight,width,length);//each seprated with coma
		if(isErr!=SUCCESS || maxHeight==0 || width==0 || length==0){
		cout << "errrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrror  "<<err+(int)ErrorID::ShipPlanBadLineFormat<<endl;
		return err|(int)ErrorID::ShipPlanBadLineFormat;
		}

		/*
		 intiate the ship
		*/
		cout<<"initiate the ship :"<<width<<", "<<length<<", "<<maxHeight<<endl;
		currShip=new Ship(width,length,maxHeight);
		cout << "ship done init"<<endl;

		firstLine=!firstLine;
		}else{
		int x,y,floors;

		isErr=getTripleElem(line,seek,x,y,floors);//each seprated with coma
		if(isErr!=SUCCESS ){
		err|=(int)ErrorID::ShipPlanBadFirstLine;
		flag=1;
		}
		if(flag!=1){
			if(floors>=maxHeight){
				cout <<"hereeeeeeeeeee 3333333333333333"<<endl;
				err|=(int)ErrorID::ShipPlanWrongFloors;
				flag=1;
			}
			if(x>=width || y>=length){
				err|=(int)ErrorID::ShipPlanWrongXY;
				flag=1;
			}
		}
		
		


		if(flag!=1){
			/*
			update ship plan
			*/
			cout << "the max floor :"<<(*currShip).planLinkedList[x][y].maxHeight << " and floors is :"<<floors <<" and maxH :"<<maxHeight<<endl;
			if((*currShip).planLinkedList[x][y].maxHeight!=maxHeight && (*currShip).planLinkedList[x][y].maxHeight!=floors){
					cout <<"hereeeeeeeeeee 11111111111111111"<<endl;
					return err|(int)ErrorID::ShipPlanDuplicateXY;
				}
			if((*currShip).planLinkedList[x][y].maxHeight==floors){
				cout <<"hereeeeeeeeeee 22222222222222222 erer :"<<err<<endl;

				err|=(int)ErrorID::ShipPlanBadLineFormat;
				cout <<"hereeeeeeeeeee 22222222333333 erer :"<<err<<endl;	
				}
			
			(*currShip).setHeight(x,y,floors);
			}
		}
		flag=0;
	}
	if(fd_info.is_open()){
	fd_info.close();
	}
	return err;
}
//22
int getTheFileNameFromTheTravel(string travelPath,string extention,string& theNeededFile){
	ErrorCode errCode;
	const char *cstr = travelPath.c_str();
	DIR* fd_travel=opendir(cstr);
	if(fd_travel==NULL){
		//string errorMsg="ERROR[22][1]- can't open "+travelPath+" (exiting the travel)";
		//handleError(errorOutputPath,"Simulator",errorMsg);
		return -1;
	}
	int numOfFilesWithTheExtention=0;
	struct dirent *entry;
    	while ((entry = readdir(fd_travel)))
	if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
		
		string currFile=getNameWithoutExtinsion(entry->d_name,'.',extention);
		if(currFile.compare("/")!=0){
			cout<<"-*-*-*-*-*-"<<entry->d_name<<endl;
			if(numOfFilesWithTheExtention>0){
				//string errorMsg="***ERROR[22][2]: too many "+extention+" files (exiting the travel)";
				//handleError(errorOutputPath,errorMsg);
				return (extention.compare("route") ? (int)ErrorID::TravelRouteEmptyOrCantReadFile : (int)ErrorID::ShipPlanBadFirstLine);
			}
			theNeededFile=currFile;
			numOfFilesWithTheExtention++;
		}
			
	}
	if(numOfFilesWithTheExtention==0){
		//string errorMsg="***ERROR[22][3]: no "+extention+" file (exiting the travel)";
		//handleError(errorOutputPath,errorMsg);
		return (extention.compare("route")==0 ? (int)ErrorID::TravelRouteEmptyOrCantReadFile : (int)ErrorID::ShipPlanBadFirstLine);

	}
	closedir(fd_travel);
	return SUCCESS;
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

		return SUCCESS;
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
//20
string getNameWithoutExtinsion(string fileName,char delemiter,string extension){
	int seek=0;
	if(fileName.compare(extension)==0){
		cout<<"ERROR[20][1]: "<<fileName<<" is the same as the extension"<<endl;

		return "/";

	}
	while(seek < (int)fileName.length()){
		getElem(fileName,seek,delemiter);
	}
	if(extension.compare(parse_out)==0){
		cout<<"+++int get Name without : "<<parse_out.substr(0,parse_out.find(string(1,delemiter)+extension))<<endl;
		return fileName.substr(0,fileName.find(string(1,delemiter)+extension));
	}
	
	return "/";

}

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
//21
string** ReadExpectedInstructions(string cargoFileNamePath){
	ifstream fd_info;
	string** expectedInstructions;
 	
	fd_info.open(cargoFileNamePath,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		std::cout << "ERROR[13][1]- can't open "<< cargoFileNamePath<< std::endl;
		return NULL;
	}
	int lineNum = getNumOfLines(fd_info);
	numInstructions=lineNum;
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

	/*****************************************/

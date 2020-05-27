
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <fstream>
#include<string.h>
#include <string>
#include <map>
#include <set>
#include "../Interfaces/IOHandler.h"
#define SUCCESS 0
#define ERROR 1
#define MAX_LINE 1024
using namespace std;

std::set<string> myset;
std::set<string>emptyPorts;


int checkPortName(string& name);

/*--------------------------PARSING METHODS--------------------------*/
/*this function got a string and a seek with a delemiter and searchs for the next matched item*/
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
	
	//if(delmiter==','){
	parse_out.erase(std::remove_if(parse_out.begin(), parse_out.end(), ::isspace), parse_out.end());
	//}
	if(parse_out.compare("")==0){
		seek++;
		return ERROR;
	}
	seek++;
	return SUCCESS;//must return
}
/*-------------------------------------------------------------------------------------------------------------*/




//[11]
/*get the ports from the char***/
Port* getPortsFromRoute(char** &currRoute){
	int index;
	Port* ports=new Port[routeSize+1];
	for(index=0;index<routeSize;index++){
		ports[index]=Port(string(currRoute[index]));
	}
	ports[index]=Port("last");
	return ports;

}
//[10]
/*this function prints the algo instruction to a file*/
int instructionsOut(string** instructions,string outName){
	//open output file to write the instruction to output dir ??????????????????????? we must open a output dir to every algo????
	ofstream fd_info;
//	cout << "in out" <<endl;
	numInstructions=0;
	string filePath=outName;
//	cout << filePath <<endl;
	fd_info.open(filePath,ios_base::out);//open the file to out
	if(!fd_info){
	//	std::cout << "ERROR[10][1]- can't open "<< filePath<< std::endl;
	}
	int instIndex=0;

	//printing loop
	while(instructions[instIndex][2].compare("last")!=0){
		//uid,L/R/U,row,column,height
		//cout<<"the instruction : "<<instructions[instIndex][0]<<","<<instructions[instIndex][1]<<","<<instructions[instIndex][2]<<","<<instructions[instIndex][3]<<","<<instructions[instIndex][4]<<endl;
		fd_info<< instructions[instIndex][1]<<","<<instructions[instIndex][0]<<","<<instructions[instIndex][2]<<","<<instructions[instIndex][3]<<","<<instructions[instIndex][4];
		instIndex++;

		if(instructions[instIndex][2].compare("last")!=0){

			fd_info<<endl;
		}
	} 
	if(fd_info.is_open()){
	fd_info.close();
	}

	return SUCCESS;
}
//[9]
/*adding the extension of .cargo_data/.crane_instructions to the port file name*/
string getCargoFileName(int portIndex,bool cargoData){
	int cnt=1;
	for(int i=0;i<portIndex;i++){
		if(strcmp(route[portIndex],route[i])==0){
			cnt++;
		}
	}
	string fileName=string(route[portIndex])+"_"+to_string(cnt);
	if(cargoData==true){
	fileName+=".cargo_data";
	}else{
	fileName+=".crane_instructions";
	}
	return fileName;

}
//[8]
/*read the cargo from a file*/
Container* parseCargoFile(string fileName){
	ifstream fd_info;
	//cout << "in parseCargoFile"<<endl;
	int is_err=SUCCESS;
	Container* containers;
	int containerNum=0;
	string filePath=fileName;
	fd_info.open(filePath,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		//std::cout << "ERROR[8][1]- can't open "<< filePath<< std::endl;
		containers=new Container[1];
		containers[containerNum]=Container(0,Port(""),"last");
		return containers;

	}
	
	containerNum=getNumOfLines(fd_info);//get container size
	//cout<<"in parseCargoFile with lines "<<containerNum<<endl;
	containers=new Container[containerNum+1];
	string line;
	int containerIndex=0;
	int seek;
	string containerUID;
	string containerDstPort;
	int containerWeight;
	//looping over the lines
	while(getline(fd_info,line)){
	//	cout<<"line :  "<<line<<endl;
		if(line=="" || line.at(0)=='#'){
			continue;
			}
			seek=0;
			try{
			getElem(line,seek,',');
			if(parse_out==""){
				throw((int)ErrorID::ContainersIDCannotBeRead);
			}
			containerUID=string(parse_out);
			if(parse_out==""){
				throw((int)ErrorID::ContainersIDCannotBeRead);
			}
			getElem(line,seek,',');
			containerWeight=std::stoi(parse_out); // get length
			if(parse_out=="" || containerWeight<0){
				throw ((int)ErrorID::ContainersMissingOrBadWeight);
			}

			getElem(line,seek,',');
			if(parse_out=="" || containerWeight<0 || seek <(int)line.length()){
				throw ((int)ErrorID::ContainersMissingOrBadPortDest);
			}
			containerDstPort=string(parse_out);
			int isErrName=checkPortName(containerDstPort);
			if(isErrName!=SUCCESS){
				throw ((int)ErrorID::ContainersMissingOrBadPortDest);
			}

			Port dstPort(containerDstPort);
			containers[containerIndex]=Container(containerWeight,dstPort,containerUID);
			containerIndex++;
			}catch(int e){
				if(e==(int)ErrorID::ContainersIDCannotBeRead){
					containerNum--;
				}else{
				containers[containerIndex]=Container(-1,Port("reject"),containerUID);
				containerIndex++;
				}
				is_err|=e;
			//	cout << " got catched :((((("<<endl;			
			}
	}
	if(fd_info.is_open()){
	fd_info.close();
	}
	//don't forget to add the last container
	containers[containerNum]=Container(is_err,Port(""),"last");//to know we reached the last container
	//cout<<"exit  :  "<<parseCargoFile<<endl;

	return containers;
}

//[7] called in [5]
/*checks if the port name is valid*/
int checkPortName(string& name){
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
		if(line=="" || line.at(0)=='#'){
			continue;
			}
		numOfLines++;
	}
	fd.clear();
	fd.seekg(0,ios::beg);//back to the start
	return numOfLines;
}

//[5] called in [3]
/*initiate the route from a travel path */
int initRoute(char** &currRoute,string travelPath){
	ifstream fd_info;
	int is_err;
	int err=0;
	fd_info.open(travelPath,ios_base::in);//open the file
	//checking the access to the file
	if(!fd_info){
		return err|=(int)ErrorID::TravelRouteEmptyOrCantReadFile;
	}
	routeSize=getNumOfLines(fd_info);//get route size
	if(routeSize==0){
		return err|=(int)ErrorID::TravelRouteEmptyOrCantReadFile;
	}
	currRoute=new char*[routeSize];
	for(int i=0;i<routeSize;i++){
		currRoute[i]=new char[5];
	}
	//get the route
	string line;
	int portIndex=0;
	int seek;
	//int numOfValids=0;
	string currPort;
	while(getline(fd_info,line)){
		if(line=="" || line.at(0)=='#'){
			continue;
			}
			seek=0;
			getElem(line,seek);
			is_err=checkPortName(parse_out);
			if(is_err==ERROR){
			//	std::cout << "ERROR[5][2]- wrong port name "<< parse_out<< std::endl;
				err|=(int)ErrorID::TravelRouteBadPortSymbol;
				routeSize--;
				continue;
			}
			if(is_err!=ERROR){
				if(parse_out.compare(currPort)==0){
					err|=(int)ErrorID::TravelRoutePortAppearsTwice;
					routeSize--;
					continue;
				}
				strcpy(currRoute[portIndex],parse_out.c_str());
				//cout<<currRoute[portIndex]<<endl;
				currPort=parse_out;
				portIndex++;
			}
			
	}
	if(fd_info.is_open()){
	fd_info.close();
	}
	if(routeSize==1){
		return err|=(int)ErrorID::TravelRoureFileWithOnlySinglePort;

	}
	if(routeSize==0){
		return err|=(int)ErrorID::TravelRouteEmptyOrCantReadFile;
	}

	
	return err;
}
//[6] called in[4]
/*getting three elements seprated by ',' ---- containerID,weight,port*/
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
	err=getElem(line,seek,',');
	if(err!=ERROR || seek<=(int)line.length()){
		return ERROR;
	}				

	}catch(...){
		//cout << "error catched"<<endl;

		return ERROR;
	}
	if(seek-1 < (int)line.length()){
		return ERROR;

	}
	//cout << firstElem <<","<<secElem<<","<<thirdElem<<endl;
	return SUCCESS;
}
//[4] called in [3]
/*initiate the ship plan by a travel path*/
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
		if(line=="" ||line.at(0)=='#'){
			continue;
			}

	int seek =0;
	//getting the first line
	if(firstLine){
		isErr=getTripleElem(line,seek,maxHeight,width,length);//each seprated with coma
		
		if(isErr!=SUCCESS || maxHeight==0 || width==0 || length==0){
//cout << "errrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrror  "<<err+(int)ErrorID::ShipPlanBadLineFormat<<endl;
		return err|(int)ErrorID::ShipPlanBadLineFormat;
		}

		/*
		 intiate the ship
		*/
		//maxHeight=maxHeight-1;
		//cout<<"initiate the ship :"<<width<<", "<<length<<", "<<maxHeight<<endl;
		currShip=new Ship(width,length,maxHeight);
		//cout << "ship done init"<<endl;

		firstLine=!firstLine;
	}else{
		int x,y,floors;

		isErr=getTripleElem(line,seek,x,y,floors);//each seprated with coma
		if(isErr!=SUCCESS ){//problem with the line
			err|=(int)ErrorID::ShipPlanBadFirstLine;
			flag=1;
		}
		if(flag!=1){
			//cout<< "the floors "<<floors << " and the max height  "<<maxHeight<<endl; 
			if(floors>=maxHeight){
				err|=(int)ErrorID::ShipPlanWrongFloors;
				flag=1;
			}
			if(x>=width || y>=length){
				err|=(int)ErrorID::ShipPlanWrongXY;
				flag=1;
			}
		}
		
		

		//if its a good line ..no need to ignore
		if(flag!=1){
			/*
			update ship plan
			*/
		//	cout << "the max floor :"<<(*currShip).planLinkedList[x][y].maxHeight << " and floors is :"<<floors <<" and maxH :"<<maxHeight<<endl;
			if((*currShip).planLinkedList[x][y].maxHeight-(*currShip).planLinkedList[x][y].size!=maxHeight && (*currShip).planLinkedList[x][y].maxHeight-(*currShip).planLinkedList[x][y].size!=floors){
				
					return err|(int)ErrorID::ShipPlanDuplicateXY;				}
			if((*currShip).planLinkedList[x][y].size==(*currShip).planLinkedList[x][y].maxHeight-floors){


				err|=(int)ErrorID::ShipPlanBadLineFormat;

				}
			
			(*currShip).setHeight(x,y,floors);
		//	cout<<"------------------------------------------------------------------------"<<endl;
		//	cout<< "	*in X,Y the maxHeight is :"<<(*currShip).planLinkedList[x][y].maxHeight<< "  and the size is :"<<(*currShip).planLinkedList[x][y].size<<endl;
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
/*searchs for a file with an extension and returns true if found*/
int getTheFileNameFromTheTravel(string travelPath,string extention,string& theNeededFile){
	ErrorCode errCode;
	const char *cstr = travelPath.c_str();
	DIR* fd_travel=opendir(cstr);
	if(fd_travel==NULL){
		string errorMsg="ERROR[22][1]- can't open "+travelPath+" (exiting the travel)";
		handleError(errorOutputPath,"Simulator",errorMsg);
		return -1;
	}
	int numOfFilesWithTheExtention=0;
	struct dirent *entry;
    	while ((entry = readdir(fd_travel)))
	if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
		
		string currFile=getNameWithoutExtinsion(entry->d_name,'.',extention);
		if(currFile.compare("/")!=0){
//cout<<"-*-*-*-*-*-"<<entry->d_name<<endl;
			//if there is a lot of file extensions so we return an error
			if(numOfFilesWithTheExtention>0){
				string errorMsg="***ERROR[22][2]: too many "+extention+" files (exiting the travel)";
				handleError(errorOutputPath,"Simulator",errorMsg);
				return (extention.compare("route") ? (int)ErrorID::TravelRouteEmptyOrCantReadFile : (int)ErrorID::ShipPlanBadFirstLine);
			}
			theNeededFile=currFile;
			numOfFilesWithTheExtention++;
		}
			
	}
		//if there is no extensions file
	if(numOfFilesWithTheExtention==0){
		string errorMsg="***ERROR[22][3]: no "+extention+" file in <<travelPath<<(exiting the travel)";
		handleError(errorOutputPath,"Simulator",errorMsg);
		return (extention.compare("route")==0 ? (int)ErrorID::TravelRouteEmptyOrCantReadFile : (int)ErrorID::ShipPlanBadFirstLine);

	}
	closedir(fd_travel);
	return SUCCESS;
}

/**********implement some funcs*********/
int checkCargoFiles(string travelPath){
	//int err=0;
	const char *cstr = travelPath.c_str();
	DIR* fd_travel=opendir(cstr);
	if(fd_travel==NULL){
		string errorMsg="***ERROR: can't open "+travelPath;
		handleError(errorOutputPath,"Simulator",errorMsg);
		return ERROR;
	}
	struct dirent *entry;
    	while ((entry = readdir(fd_travel)))
	if(strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0){
//cout<<entry->d_name<<endl;
		string currFile=getNameWithoutExtinsion(entry->d_name,'.',"cargo_data");
//cout<<currFile<<endl;
		if(currFile.compare("/")!=0){
//cout<<"11111111111111111"<<endl;

				numOfCargoFiles++;
				myset.insert(currFile);
//cout<<"2222222222222222"<<endl;
		}
	}
	std::set<string>::iterator it;
//cout << "the size is :"<<myset.size()<<endl;
//	for(it=myset.begin() ;it!=myset.end();it++){
//cout<< string(*it);
//			}

//cout<<"******************************start loop with :"<<routeSize <<endl;
	for(int routeIndex=0;routeIndex<routeSize;routeIndex++){
//cout<<routeIndex<<endl;
		string FileNameCarge=getCargoFileName(routeIndex,true);
//cout<<FileNameCarge<<endl;

		FileNameCarge=getNameWithoutExtinsion(FileNameCarge,'.',"cargo_data");
//cout<<FileNameCarge<<endl;
		it=myset.find(FileNameCarge);
		if(it!=myset.end()){
			myset.erase(it);
			numOfCargoFiles--;
		}else{
			emptyPorts.insert(FileNameCarge);
		}
	

	}
//cout<<"end loop"<<endl;
	if(numOfCargoFiles!=0){
		return -1;
	}
	return SUCCESS;
	
	}

/*getting the port index from the route*/
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
//std::cout<<"the parse_out is :"<<parse_out<<std::endl;
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
//cout<<"in getTheFileName: "<<fullFilePath<<endl;
		while(seek <= (int)fullFilePath.length()){
		getElem(fullFilePath,seek,'/');
//cout << parse_out<<endl;
		}
		fileName=string(parse_out);
//cout<<"for npw the filename is :"<<fileName<<endl;

		return fileName;

	}
//20
/*getting a name without extensions*/
string getNameWithoutExtinsion(string fileName,char delemiter,string extension){
//cout<<"getNameWithoutExtinsion start"<<endl;

	int seek=0;
	if(fileName.at(0)==delemiter){
	return "/";
	
	}
//cout<<"extension comparing start"<<endl;
	if(fileName.compare(extension)==0){
		//cout<<"ERROR[20][1]: "<<fileName<<" is the same as the extension"<<endl;

		return "/";

	}
//cout<<"getNameWithoutExtinsion start  while"<<endl;
	while(seek < (int)fileName.length()){
		getElem(fileName,seek,delemiter);
	}
//cout<<"getNameWithoutExtinsion end  while"<<endl;
	if(extension.compare(parse_out)==0){
		//cout<<"+++int get Name without : "<<parse_out.substr(0,parse_out.find(string(1,delemiter)+extension))<<endl;
		return fileName.substr(0,fileName.find(string(1,delemiter)+extension));
	}
	
	return "/";

}


	/*****************************************/

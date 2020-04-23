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
#include "Stowage.cpp"
#include "WeightBalanceCalculator.cpp"
#define SUCCESS 1
#define ERROR 0
#define LAST 1
#define REGULAR 0
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
const char* EXPECTED_OUTPUT="/expected_output";
char* parse_out=new char[MAX_LINE];
Ship* ship;
Stowage* curAlgo;
ofstream fd_results;
const char* fake_out="/fake_out";
string travelName;
string AlgoName="fakeAlgorithim";
int numInstructions=0;
/*---------------FUNC DEC-------------*/
int getNumOfLines(ifstream& fd);
char* getCargoFileName(int portIndex);
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
//must remove this 
string** ReadExpectedInstructionsFAKE(char* cargoFileName){
	ifstream fd_info;
	string** expectedInstructions;
	char* expectedPath=new char[strlen(travelPath)+strlen(fake_out)+strlen(cargoFileName)+15];
 	strcpy(expectedPath,travelPath);
	strcat(expectedPath,fake_out);
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

/*--------------------------PARSING METHODS--------------------------*/
char* getElem(string s , int& seek,char delmiter=' '){
	int index=0;
	if(delmiter==' '){//find the first index which not whitspace
		while(seek < (int)s.length() && s.at(seek)==delmiter){
			seek++;
		}
	}
	while(seek < (int)s.length()){
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
//[11]
Port* getPortsFromRoute(){
	int index;
	Port* ports=new Port[routeSize+1];
	for(index=0;index<routeSize;index++){
		ports[index]=Port(string(route[index]));
	}
	ports[index]=Port("last");
	return ports;

}
//[10]//not finished yet
int instructionsOut(string** instructions,char* outName){
	//open output file to write the instruction to output dir ??????????????????????? we must open a output dir to every algo????
	ofstream fd_info;
	cout << "in out" <<endl;
	numInstructions=0;
	char* filePath=new char[strlen(travelPath)+strlen(OUTPUT)+strlen(outName)+6];
	
	strcpy(filePath,travelPath);
	strcat(filePath,OUTPUT);
	strcat(filePath,"/");
	strcat(filePath,outName);
	strcat(filePath,".out");
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
	delete[] filePath;
	fd_info.close();
	return SUCCESS;
}
//[9]
char* getCargoFileName(int portIndex){
	int cnt=1;
	for(int i=0;i<portIndex;i++){
		if(strcmp(route[portIndex],route[i])==0){
			cnt++;
		}
	}
	char* fileName=new char[20];
	strcpy(fileName,route[portIndex]);
	strcat(fileName,"_");
	fileName[strlen(fileName)]='0'+cnt;
	fileName[strlen(fileName)]='\0';
	strcat(fileName,".cargo_data");
	return fileName;
	
}
//[8] 
    Container* parseCargoFile(char* fileName){
	ifstream fd_info;
	int is_err=SUCCESS;//must change this.........................................
	Container* containers;
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
	containers=new Container[containerNum+1];
	containers[containerNum]=Container(0,Port(""),"last");//to know we reached the last container
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
				containers[containerIndex]=Container(containerWeight,dstPort,containerUID);
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
	cout << "the parsing line : " <<line<<endl;
	if(firstLine){
		getTripleElem(line,seek,maxHeight,width,length);//each seprated with coma
		/*
		 intiate the ship
		*/
		cout<<"initiate the ship :"<<width<<", "<<length<<", "<<maxHeight<<endl;
		ship=new Ship(width,length,maxHeight);
		cout << "ship done init"<<endl;

		firstLine=!firstLine;	
		}else{
		int x,y,floors;
		
		getTripleElem(line,seek,x,y,floors);//each seprated with coma
		if(floors<maxHeight){// you can check for an error format
			/*
			update ship plan
			*/
			(*ship).setHeight(x,y,floors);
			}
		}
	}
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
	/*for(int i=0;i<routeSize;i++){
		cin  >>check ;
	getch();
		char* FileName=getCargoFileName(i);
		cout<<"	parsing :"<<FileName<<endl;
		string** fakeInstructions=ReadExpectedInstructionsFAKE(FileName);
		instructionsOut(fakeInstructions,FileName);
		check=checkInstructionPerPort(i,fakeInstructions);
		if(check==SUCCESS){
			parseResults (AlgoName,travelName,numInstructions,i+1);

		}else{
			parseResults (AlgoName,travelName,numInstructions,0-(i+1));
		}*/
	//try
	Port* ports=getPortsFromRoute();
	parseResults (AlgoName,travelName,0,0);
	for(int routeIndex=0;routeIndex<routeSize;routeIndex++){
		char* FileName=getCargoFileName(routeIndex);
	Container * containers=parseCargoFile(FileName);
		printContainerArray(containers, FileName);
	  curAlgo =new Stowage(routeIndex,ship,ports,containers);
		(*ship).printPlanMap();
	  string** algoInstructions=curAlgo->currentInstructions;
		instructionsOut(algoInstructions,FileName);
	   check=checkInstructionPerPort(routeIndex,algoInstructions);
	if(check==SUCCESS){
			parseResults (AlgoName,travelName,numInstructions,routeIndex+1);

		}else{
			parseResults (AlgoName,travelName,numInstructions,0-(routeIndex+1));
	}
	 /* get the instruction
	 * update the ship
	 */

	}
	 /*	
	  *
	  init algorithm ? and start to play
	  *
	  */
	
	Container* instructions=NULL;
	if(instructions!=NULL){
	//Port* ports=getPortsFromRoute();
	for(int routeIndex=0;routeIndex<routeSize;routeIndex++){
	/*clone the ship
	 */
	  curAlgo =new Stowage(routeIndex,ship,ports,instructions);
	  string** algoInstructions=curAlgo->currentInstructions;
	   int check=checkInstructionPerPort(0,algoInstructions);
	bool o=tryOperation();
		cout<<check<<o<<endl;
	 /* get the instruction
	 * update the ship
	 */
	}

	//

	
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
/*
	//container* containers=new container[5];
	cout <<"got a new life"<<endl;
	string ** ptr=new string*[5];
	ptr[0]=new string[5];
	ptr[1]=new string[5];
	ptr[2]=new string[5];
	ptr[3]=new string[5];
	ptr[4]=new string[5];
	cout <<"got a new life"<<endl;
	
	cout <<"got a new life"<<endl;
	ptr[0][0]="container1";
	ptr[0][1]="load";
	ptr[0][2]="1";
	ptr[0][3]="2";
	ptr[0][4]="1";
	ptr[1][0]="container2";
	ptr[1][1]="unload";
	ptr[1][2]="1";
	ptr[1][3]="3";
	ptr[1][4]="1";
	ptr[2][0]="container3";
	ptr[2][1]="load";
	ptr[2][2]="1";
	ptr[2][3]="3";
	ptr[2][4]="1";
	ptr[3][0]="container3";
	ptr[3][1]="load";
	ptr[3][2]="1";
	ptr[3][3]="3";
	ptr[3][4]="1";
	ptr[4][0]="last";
	ptr[4][1]="load";
	ptr[4][2]="1";
	ptr[4][3]="2";
	ptr[4][4]="1";
	int check=checkInstructionPerPort(0,ptr);
	
cout << "is valid : "<<check<<endl;
	char* s=new char[25];
	strcpy(s,"AAAAA_1.cargo_data");
	
	instructionsOut(ptr,s);
	*/

	//closedir(fd_path);
	//delete[] workPath;
	//delete[] parse_out;
	return 0;
}
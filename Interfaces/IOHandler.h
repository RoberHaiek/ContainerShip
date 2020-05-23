// IOHandler.h
#ifndef IOHANDLER_H_INCLUDED
#define IOHANDLER_H_INCLUDED

//includes and globals
#include "../Algorithms/StowageTester.cpp"
#include <fstream>
#define SUCCESS 0
#define ERROR 1
#define MAX_LINE 1024
using namespace std;
int width,length,maxHeight;
string travelPath;
string workPath;
char **route;
ofstream fd_results;
int routeSize;
const char* SHIP_PLAN="ship_plan";
const char* ROUTE="route";
const char* OUTPUT="/output";
string parse_out;
int numInstructions=0;
int numOfCargoFiles=0;
Ship* ship;

//functions
int getElem(string s , int& seek,char delmiter);
int getElem(string s , int& seek){
	return getElem(s ,seek,' ');
}

void parseResults (string algoName,string travelName,int numInst, int port);
Port* getPortsFromRoute(char** &currRoute);
int instructionsOut(string** instructions,string outName);
string getCargoFileName(int portIndex,bool cargoData);
Container* parseCargoFile(string fileName);
int checkPortName(string name);
int getNumOfLines(ifstream& fd);
int initRoute(char** &currRoute,string travelPath);
int getTripleElem(string line,int& seek,int& firstElem ,int& secElem ,int& thirdElem);
int initShipPlan(Ship* &currShip,string travelPath);
int getRouteIndex(int &routeIndex,const std::string& input_full_path_and_file_name);
string getTheFileName(string fullFilePath);
string getNameWithoutExtinsion(string fileName,char delemiter,string extension);
int getTheFileNameFromTheTravel(string travelPath,string extention,string& theNeededFile);
#endif

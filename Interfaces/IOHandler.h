// IOHandler.h
#ifndef IOHANDLER_H_INCLUDED
#define IOHANDLER_H_INCLUDED

//includes and globals
#include "../Algorithms/StowageTester.cpp"
#define SUCCESS 1
#define ERROR 0
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
char* parse_out=new char[MAX_LINE];
int numInstructions=0;
Ship* ship;

//functions
char* getElem(string s , int& seek,char delmiter);
char* getElem(string s , int& seek){
	return getElem(s ,seek,' ');
}

void parseResults (string algoName,string travelName,int numInst, int port);
Port* getPortsFromRoute(char** &currRoute);
int instructionsOut(string** instructions,string outName);
string getCargoFileName(int portIndex,bool cargoData);
Container* parseCargoFile(string fileName);
int checkPortName(char* name);
int getNumOfLines(ifstream& fd);
void initRoute(char** &currRoute);
void getTripleElem(string line,int& seek,int& firstElem ,int& secElem ,int& thirdElem);
void initShipPlan(Ship* &currShip);
#endif

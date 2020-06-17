
/*
 * IOHandler.h
 *
 *  Created on: Mar 29, 2020
 *      Author: Aubaida
 */

#ifndef IOHANDLER_H_INCLUDED
#define IOHANDLER_H_INCLUDED
/*
*this interface is rensponsiple of handling the I/O files and parsing them.. 
*and initiate the route/ship plan by a file path..
*/
//includes and globals
#include "../algorithm/StowageTester.cpp"
#include <fstream>
#define SUCCESS 0
#define ERROR 1
#define MAX_LINE 1024
using namespace std;

string workPath;
//string parse_out;
ofstream fd_results;

//string travelPath;
//char **route;
//int routeSize;

//int numInstructions=0;
//Ship* ship;

//functions
int getElem(string s , int& seek,char delmiter,string& parse_out);
int getElem(string s , int& seek,string& parse_out){
	return getElem(s ,seek,' ',parse_out);
}

void parseResults (string algoName,string travelName,int numInst, int port);
Port* getPortsFromRoute(char** &currRoute,int &routeSize);
int instructionsOut(string** instructions,string outName);
string getCargoFileName(int portIndex,bool cargoData,char** route);
Container* parseCargoFile(string fileName);
int checkPortName(string& name);
int getNumOfLines(ifstream& fd);
int initRoute(char** &currRoute,string travelPath,int &routeSize);
int getTripleElem(string line,int& seek,int& firstElem ,int& secElem ,int& thirdElem);
int initShipPlan(Ship* &currShip,string travelPath);
int getRouteIndex(int &routeIndex,const std::string& input_full_path_and_file_name,char** route);
string getTheFileName(string fullFilePath);
string getNameWithoutExtinsion(string fileName,char delemiter,string extension);
int getTheFileNameFromTheTravel(string travelPath,string extention,string& theNeededFile);
#endif

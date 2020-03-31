//============================================================================
// Name        : MiniSimulator.cpp
// Author      : Aubaida
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string.h>
#include <dirent.h>
#include <fstream>
using namespace std;

/* argv[1] will be the path of the workspace(IO-Files)*/
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
	/*
	char* line =new char[1024];
	int i=0;
	while(!fd_info.eof()){
		line[i]=fd_info.get();
		i++;
	}
	line[--i]='\0';
	cout << line <<endl;
	*/


	fd_info.close();
	closedir(fd_path);
	delete[] workPath;
	delete[] infoFile;
	return 0;
}


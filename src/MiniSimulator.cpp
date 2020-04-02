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
class container {
	public :
		string uid;
		int weight;
		string dst;
		container(string uid,int weight,string dst){
			this->uid=uid;
			this->dst=dst;
			this->weight=weight;
		}
		container(){
			this->uid="empty";
			this->dst="empty";
			this->weight=0;
		}
};
int width,length,maxHeight;
//-----------------------------------------------------------------------------
char* getElem(string s , int& seek,char delmiter=' '){
	char* out=new char[1024];
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
		out[index++]=s.at(seek++);
	}
	out[index]='\0';
	seek++;
	return out;
}

int parse(ifstream& fd){//known format
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
}
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


	parse(fd_info); // parse the file and get the needed info.


	fd_info.close();
	closedir(fd_path);
	delete[] workPath;
	delete[] infoFile;
	return 0;
}


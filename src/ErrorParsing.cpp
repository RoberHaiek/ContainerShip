#include "ErrorCode.h"
#include <stdio.h>
#include<iostream>

status handleError const(const string& outputPath,const string& errorFounder,int errorCode){
	if(!fd_errors.is_open()){
			fd_errors.open("simulation.errors");

	}

	status currentStatus=documentErrors(fd_errors,errorCode);
	fd_errors<<"---------------------------------------------------------------\n"
	return currentStatus;

}
status documentErrors(ofstream& fd_errors,int errorCode){
	status currentStatus = Ignore;
	for (auto it = ErrorID.begin(); it != ErrorID.end(); it++) {
			if (it & errorCode == 1){
				string msg;
				getInfo(it,msg,currentStatus);
				fd_errors<<"\t* "+msg+"\n";

			}
		}

	return currentStatus;
}

void getInfo(int errorId,stirng& msg,status& currentStatus){

	/*
	 *
	 * switch case
	 */
	 */

}

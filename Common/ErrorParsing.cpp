#include "ErrorCode.cpp"
#include <stdio.h>
#include<iostream>
void getInfo(int errorId,string& msg,status& currentStatus){

	switch(errorId){
		case(ShipPlanWrongFloors):msg="-2^0 - ship plan: a position has an equal number of floors, or more, than the number of floors provided in the first line (ignored) ";//0
		       currentStatus=Ignore;
			break;
		case(ShipPlanWrongXY):msg="-2^1 - ship plan: a given position exceeds the X/Y ship limits (ignored) ";//1
		       currentStatus=Ignore;
			break;
		case(ShipPlanBadLineFormat):msg="-2^2 - ship plan: bad line format after first line or duplicate x,y appearance with same data (ignored) ";//2
		       currentStatus=Ignore;
			break;
		case(ShipPlanBadFirstLine):msg="-2^3 - ship plan: travel error - bad first line or file cannot be read altogether (cannot run this travel)";//3
		       currentStatus=FatalError;
			break;
		case(ShipPlanDuplicateXY):msg="-2^4 - ship plan: travel error - duplicate x,y appearance with different data (cannot run this travel)";//4
		       currentStatus=FatalError;
			break;
		case(TravelRoutePortAppearsTwice):msg="-2^5 - travel route: a port appears twice or more consecutively (ignored) ";//5
		       currentStatus=Ignore;
			break;
		case(TravelRouteBadPortSymbol):msg="-2^6 - travel route: bad port symbol format (ignored)";//6
		       currentStatus=Ignore;
			break;
		case(TravelRouteEmptyOrCantReadFile):msg="-2^7 - travel route: travel error - empty file or file cannot be read altogether (cannot run this travel) ";//7
		       currentStatus=FatalError;
			break;
		case(TravelRoureFileWithOnlySinglePort):msg="-2^8 - travel route: travel error - file with only a single valid port (cannot run this travel) ";//8
		       currentStatus=FatalError;
			break;
		case(Reserved):msg="-2^9 - reserved ";//9
		       currentStatus=FatalError;
			break;
		case(ContainersDuplicateID):msg="-2^10 - containers at port: duplicate ID on port (ID rejected)";//10
		       currentStatus=Ignore;
			break;
		case(ContainersIDArleadyOnShip):msg="-2^11 - containers at port: ID already on ship (ID rejected)";//11
		       currentStatus=Ignore;
			break;
		case(ContainersMissingOrBadWeight):msg="-2^12 - containers at port: bad line format, missing or bad weight (ID rejected) ";//12
		       currentStatus=Ignore;
			break;
		case(ContainersMissingOrBadPortDest):msg="-2^13 - containers at port: bad line format, missing or bad port dest (ID rejected) ";//13
		       currentStatus=Ignore;
			break;
		case(ContainersIDCannotBeRead):msg="-2^14 - containers at port: bad line format, ID cannot be read (ignored) ";//14
		       currentStatus=Ignore;
			break;
		case(ContainersIllegalIDCheck):msg="-2^15 - containers at port: illegal ID check ISO 6346 (ID rejected) ";//15
		       currentStatus=Ignore;
			break;
		case(ContainersFileCannotBeRead):msg="-2^16 - containers at port: file cannot be read altogether (assuming no cargo to be loaded at this port) ";//16
		       currentStatus=FatalError;
			break;
		case(ContainersLastPortHasWaitingContainers):msg="-2^17 - containers at port: last port has waiting containers (ignored)";//17
		       currentStatus=Ignore;
			break;
		case(ContainersTooManyContainers):msg="-2^18 - containers at port: total containers amount exceeds ship capacity (rejecting far containers)";//18
		       currentStatus=Ignore;
			break;
	}

}

status documentErrors(ofstream& fd_errors,int errorCode){
	status currentStatus = Ignore;
	if(errorCode==0){
		return currentStatus ;
	}
	for (int index=0 ;index<19 ; index++) {
			auto it =(ErrorID)index;
			if (((int)(pow(2,(int)it)) & errorCode) == (int)(pow(2,(int)it))){
				string msg;
				getInfo((int)(pow(2,(int)it)),msg,currentStatus);
				fd_errors<<"\t"<<msg<<"\n";

			}
		}

	return currentStatus;
}

status handleError(const string& errorOutputPath,const string& errorFounder,int errorCode){
	if(!fd_errors.is_open()){
			fd_errors.open(errorOutputPath+"/"+"simulation.errors");

	}
	fd_errors<<"-----------------"<<errorFounder<<"-----------0------------------\n";
	status currentStatus=documentErrors(fd_errors,errorCode);
	fd_errors<<"------------------------------------------------------------------\n";
	return currentStatus;

}

void handleError(const string& errorOutputPath,const string& errFounder,const string& errorMsg){
	if(!fd_errors.is_open()){
			fd_errors.open(errorOutputPath+"/"+"simulation.errors");
	fd_errors<<"-----------------"<<errFounder<<"--------1---------------------\n";

	}
	fd_errors<<"\t* "+errorMsg+"\n";
	fd_errors<<"-------------------------------------------------------------------\n";
}


/*
 * ErrorCode.h
 *
 *  Created on: May 16, 2020
 *      Author: n_b_a
 */
#pragma once
#include <cmath>
#include <string>
#include <fstream>
using namespace std;
ofstream fd_errors;
string errorOutputPath;
enum status{Ignore,FatalError};
enum ErrorID{
	ShipPlanWrongFloors=(int)pow(2,0),
	ShipPlanWrongXY=(int)pow(2,1),
	ShipPlanBadLineFormat=(int)pow(2,2),
	ShipPlanBadFirstLine=(int)pow(2,3),
	ShipPlanDuplicateXY=(int)pow(2,4),
	TravelRoutePortAppearsTwice=(int)pow(2,5),
	TravelRouteBadPortSymbol=(int)pow(2,6),
	TravelRouteEmptyOrCantReadFile=(int)pow(2,7),
	TravelRoureFileWithOnlySinglePort=(int)pow(2,8),
	Reserved=(int)pow(2,9),
	ContainersDuplicateID=(int)pow(2,10),
	ContainersIDArleadyOnShip=(int)pow(2,11),
	ContainersMissingOrBadWeight=(int)pow(2,12),
	ContainersMissingOrBadPortDest=(int)pow(2,13),
	ContainersIDCannotBeRead=(int)pow(2,14),
	ContainersIllegalIDCheck=(int)pow(2,15),
	ContainersFileCannotBeRead=(int)pow(2,16),
	ContainersLastPortHasWaitingContainers=(int)pow(2,17),
	ContainersTooManyContainers=(int)pow(2,18)
	};

class ErrorCode;


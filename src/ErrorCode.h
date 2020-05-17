/*
 * ErrorCode.h
 *
 *  Created on: May 16, 2020
 *      Author: n_b_a
 */
#pragma once
#include <cmath>
#include <string>

ofstream fd_errors;
enum status{Ignore,FatalError};
enum ErrorID{
	ShipPlanWrongFloors=0,
	ShipPlanWrongXY=1,
	ShipPlanBadLineFormat=2,
	ShipPlanBadFirstLine=3,
	ShipPlanDuplicateXY=4,
	TravelRoutePortAppearsTwice=5,
	TravelRouteBadPortSymbol=6,
	TravelRouteEmptyOrCantReadFile=7,
	TravelRoureFileWithOnlySinglePort=8,
	Reserved=9,
	ContainersDuplicateID=10,
	ContainersIDArleadyOnShip=11,
	ContainersMissingOrBadWeight=12,
	ContainersMissingOrBadPortDest=13,
	ContainersIDCannotBeRead=14,
	ContainersIllegalIDCheck=15,
	ContainersFileCannotBeRead=16,
	ContainersLastPortHasWaitingContainers=17,
	ContainersTooManyContainers=18
	};
class ErrorCode;


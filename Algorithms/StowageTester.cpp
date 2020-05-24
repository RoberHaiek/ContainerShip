#include "../Common/Crane.cpp"

class StowageTester{
public:
	static int sizeOfArray(Port* array){
			int c=0;
			while(array[c].toString()!="last")
				c++;
			return c;
	}

	// checks if a container's destination port is in route
	static int isInRoute(std::string destPort, Port* route,int routeIndex){
		int sizeArray=sizeOfArray(route);
		//std::cout<<"isInRoute size= "<< sizeArray<<" dst: "<<destPort <<" route[1] :"<<route[1].toString()<<std::endl;
		if(routeIndex==sizeArray-1){
			return -1;
		}
		for(int i=routeIndex+1;i<sizeArray;i++){
			if(destPort.compare(route[i].toString())==0){
				return 0;
			}
		}
		return -1;
	}

};

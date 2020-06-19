#include "MiniSimulator.cpp"


/* argv[1] will be the path of the workspace(IO-Files)*/
//[1]
int main(int argc, char *argv[]) {
	DIR *fd_path;
	int flag=0;
	try{
		if(argc!=3 && argc!=5 && argc!=7 && argc!=9){
			std::cout << "ERROR[1][1]- Wrong Number of Parameters!" << std::endl;
			return ERROR;
		}
    	int checkErr=getFromCommandLine(argv,argc,travel_path,algorithm_path ,output, num_threads);
		if (checkErr==ERROR){
			std::cout << "ERROR[1][2]- Wrong Parameters foramt! or A missing -travel_path argument" << std::endl;
			std::cout << "did you mean : simulator [-travel_path <path>] [-algorithm_path <algorithm path>] [-output <output path>] [-num_threads <num of threads>]" << std::endl;
			handleError(output,"Simulator","ERROR- Wrong Parameters foramt! or A missing -travel_path argument");
			cout<<output<<endl;
			throw 1;
			flag=1;
			return ERROR;
		}
		//getting the path
		workPath=travel_path;
		errorOutputPath=output;
		//checking the access to the DIR
		const char *cstr = workPath.c_str();
		fd_path=opendir(cstr);
		if(fd_path==NULL){
			handleError(output,"Simulator","ERROR :: can't open the travel path :"+travel_path);
			flag=1;
			throw 1;
		}

		cout << pairingTravelAlgo(fd_path);

		if(std::stoi(num_threads)>1){
			ThreadPoolExecuter executer1 {SimpleTasksProducer{(int)travelAlgoPairs.size(), travelAlgoPairs, fd_path},std::stoi(num_threads)};
    			executer1.start();
    			executer1.wait_till_finish();
		}
		else{
			rewinddir(fd_path);
			//start the simulation
			for(int i=0;i<(int)travelAlgoPairs.size();i++){
			simulate(fd_path,travelAlgoPairs[i]);
			}
		}
	}catch(...){	//there is an error with the command line prameters
	}
	if(fd_errors.is_open()){
		fd_errors.close();	
	}
	if(flag){
		return -1;
	}
	//printing the results
	cout<<"results prints"<<endl;
	printResults();
	
	for(auto it=threadsCount.begin();it!=threadsCount.end();it++){
		cout<<"*)in thread "<<(*it).first<< ":"<<endl;
		int index=1;
		for(auto it2=(*it).second->begin();it2!=(*it).second->end();it2++){
			cout <<"	"<<index++<<"- ("<<(*it2).first<<","<<(*it2).second<<")"<<endl;
		}
	
	}
	
	cout << "Done! - ";
	return 0;
}

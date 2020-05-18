#include <iostream>
//#include <dlfcn.h>
//#include <memory>

//#include "_205962657_a.cpp"
#include "../Algorithms/_205962657_b.cpp"

/*
struct DLCloser
{
	void operator()(void *dlhandle) const noexcept {
		std::cout << "Closing dl \n";
		dlclose(dlhandle);
	}
};
*/
int main(){
/*
	std::unique_ptr<void, DLCloser> handle1(dlopen( "../Algorithms/_205962657_a.so", RTLD_LAZY));
	if(!handle1)
		std::cerr << "dlopen failed " << dlerror() << "\n";
	else
		std::cout << "_205962657_a.so opened \n";
	
	std::unique_ptr<void, DLCloser> handle2(dlopen("../Algorithms/_205962657_b.so", RTLD_LAZY));
	if(!handle1)
		std::cerr << "dlopen failed " << dlerror() << "\n";
	else
		std::cout << "_205962657_b.so opened \n";
*/
	// init route
	Port* route = new Port[4];
	Port port0, port1, port3;
	port0 = Port("TEST1");
	port1 = Port("TEST2");
	port3 = Port("last");
	route[0] = port0;
	route[1] = port1;
	route[2] = port0;
	route[3] = port3;
	// init instructions
	Container** instructions;
	instructions = new Container*[3];
	instructions[0] = new Container[3];
	instructions[1] = new Container[3];
	instructions[2] = new Container[2];
	instructions[0][0] = Container(0,port1,"ABCJ1111110");
	instructions[0][1] = Container(1,port1,"ABCJ1111111");
	instructions[0][2] = Container(0,port1,"last");
	instructions[1][0] = Container(2,port0,"ABCJ1111112");
	instructions[1][1] = Container(3,port0,"ABCJ1111113");
	instructions[1][2] = Container(0,port1,"last");
	instructions[2][0] = Container(4,port1,"test");
	instructions[2][1] = Container(0,port1,"last");

	// init ship
	Ship *ship  = new Ship(3,3,3);

	// running stowage a
	//std::cout << "Running a";
	//_205962657_a a = _205962657_a(0,ship,route,instructions[0]);

	// running stowage b
	std::cout << "Running b";
	_205962657_b b = _205962657_b(0, ship, route, instructions[0]);

	std::cout << "done";

	//for(int i=0;i<3;i++){
	//	a = Stowage(i,*ship,route,instructions[i]);
	//}
	return 0;

}
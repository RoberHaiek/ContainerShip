#include "Stowage.cpp"
int main() {
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
		Ship ship  = Ship(3,3,3);

		// running stowage
		Stowage a = Stowage(0,ship,route,instructions[0]);
		//for(int i=0;i<3;i++){
		//	a = Stowage(i,*ship,route,instructions[i]);
		//}
		return 0;

}

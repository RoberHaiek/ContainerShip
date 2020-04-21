#include "Stowage.cpp"
int main() {
		// init route
		Port* route = new Port[3];
		Port port0, port1;
		port0 = Port("TEST1");
		port1 = Port("TEST2");
		route[0] = port0;
		route[1] = port1;
		route[2] = port0;

		// init instructions
		Container** instructions;
		instructions = new Container*[3];
		instructions[0] = new Container[2];
		instructions[1] = new Container[2];
		instructions[2] = new Container[1];
		instructions[0][0] = Container(0,port1,"ABCJ1111110");
		instructions[0][1] = Container(1,port1,"ABCJ1111111");
		instructions[1][0] = Container(2,port0,"ABCJ1111112");
		instructions[1][1] = Container(3,port0,"ABCJ1111113");
		instructions[2][0] = Container(4,port1,"test");

		// init ship
		Ship ship;
		ship = Ship(10,10,10);

		// running stowage
		for(int i=0;i<3;i++){
			Stowage(i,ship,route,instructions[i]);
		}
		return 0;

}
#include "Stowage.cpp"
int main() {
		// init route
		Port* route = new Port[3];
		Port port0, port1;
		port0 = Port("TEST1");
		port1 = Port("TEST2");
		route[0] = port0;
		route[1] = port1;
		route[2] = port0;

		// init instructions
		Container** instructions;
		instructions = new Container*[3];
		instructions[0] = new Container[2];
		instructions[1] = new Container[2];
		instructions[2] = new Container[1];
		instructions[0][0] = Container(0,port1,"ABCJ1111110");
		instructions[0][1] = Container(1,port1,"ABCJ1111111");
		instructions[1][0] = Container(2,port0,"ABCJ1111112");
		instructions[1][1] = Container(3,port0,"ABCJ1111113");
		instructions[2][0] = Container(4,port1,"test");

		// init ship
		Ship ship;
		ship = Ship(10,10,10);

		// running stowage
		for(int i=0;i<3;i++){
			Stowage(i,ship,route,instructions[i]);
		}
		return 0;

}

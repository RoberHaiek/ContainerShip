How to use
make
Then run the following command from ContainerShip simulator -travel_path ../TRAVELS/ -algorithm_path ../algorithm/ -output ../output/ -num_threads x

Container ship – Design
Classes, fields and data structures:
1- Ship
Constructor: Ship(int width, int length, int height)  
Fields:  
    a- int weight of the ship  
    b- int shipWidth
    c- int shipLength
    d- int shipHeight
    e- map<string, int*> planMap - key = container unique id, value = [row,column,height] of container
    f- cellLinkedList planLinkedList - 2D array representation of the containers in the ship
      i- int size - how many containers are in the current cell [row][column]
      ii- int maxHeight - of current cell
      iii- node linkedList:
        1- Container
        2- next*
    g-map<string, string> contMap - key = container unique id, value = destPort

2- Container
Constructor: Container(int weight, Port destPort, string uniqueId)
Fields:
    a.	Int weight
    b.	Port destPort
    c.	String uniqueId
3- Port
Constructor: Port(string portId)
Fields:
    a.	String port (5 English letter code)
4- Crane // operates on a ship (loads/unloads containers)
Constructor: Crane(Ship ship)
    a- Ship ship - the ship on which the crane operates on
5- CraneTester // tests the laods/unloads validity
6- _205962657_a // the algorithm a
7- _205962657_b // the algorithm b

7- StowageTester // tests if a container's destPort is in route or not
8- Simulator // runs the stowage
9- IOHandler // Handles input/output files

*Input/Output:
input is directory path (root)

in the TRAVEL directory, there are travel directories, in each there are:
1- ship plan 2- route 3- cargo file for each port in the route 
-- these tow travels show the deffrences between the two algo results.


*The flow:

The Simulator receives info about the ship, initializes it, then receives the route of ports, and a list of instructions for each port, and runs the all the Stowage algorithms for each port with it's instructions.

The Stowage goes over all the containers in the ship and unloads what's relevant to the port, if a container is blocked, the algorithm puts it in a temporary dequeue, unlaods all above, then the container/s we need, then puts the containers that dont belong to the port back.Loads the containers to the ship, and returns a list of instructions.

The Simulator takes the instructions and runs them on an empty ship using the crane and validates that they're legal.

and if there is an error the simulator will document it .. and finally the simulator always print the results of the simulation.

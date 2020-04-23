[Link to homework](https://docs.google.com/document/d/1PloUy0vfoR0AWHmpWb0W85FZXM_BZqbaQhKXzHhwC1o/edit)

# **Container ship – Design**  

## **Inclusion flow:**  
Simulator >> Stowage >> StowageTester >> Crane >> CraneTester >> Ship >> Container >> Port   

## **Classes, fields and data structures:**  
### **1-	Ship**  
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
### **2-	Container**  
    Constructor: Container(int weight, Port destPort, string uniqueId)
    Fields:
        a.	Int weight
        b.	Port destPort
        c.	String uniqueId
### **3-	Port**  
    Constructor: Port(string portId)
    Fields:
        a.	String port (5 English letter code)
### **4- Crane**              // operates on a ship (loads/unloads containers)  
    Constructor: Crane(Ship ship)
        a- Ship ship - the ship on which the crane operates on
### **5- CraneTester**        // tests the laods/unloads validity  
### **6- Stowage**          // the algorithm  
   Constructor: Stowage(int i, Ship *ship, Port *route, Container *instructions)
        a- int instNum   // the index of the current instruction to be returned in the end
        b- Ship ship
        c- Port* route
        e- int routeIndex
        f- string** currentInstructions // the returned instructions in the end:
                                           {container's unique id, "load/unload/reject", row, column, height}
        g- deque<node*> tempContainers  // the containers which are unloaded but need to be loaded back in the same port
        h- 
### **7- StowageTester**  // tests if a container's destPort is in route or not  
### **8- Simulator** // runs the stowage  

## **The flow:**  
The Simulator receives info about the ship, initializes it, then receives the route of ports, and a list of instructions for each port, and runs the Stowage algorithm for each port with it's instructions.

The Stowage goes over all the containers in the ship and unloads what's relevant to the port, if a container is blocked, the algorithm puts it in a temporary dequeue, unlaods all above, then the container/s we need, then puts the containers that dont belong to the port back.Loads the containers to the ship, and returns a list of instructions.

The Simulator takes the instructions and runs them on an empty ship using the crane and validates that they're legal.

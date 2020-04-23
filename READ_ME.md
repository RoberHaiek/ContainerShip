[Link to homework](https://docs.google.com/document/d/1PloUy0vfoR0AWHmpWb0W85FZXM_BZqbaQhKXzHhwC1o/edit)

Container ship – Design

Inclusion flow:
Simulator >> Stowage >> StowageTester >> Crane >> CraneTester >> Ship >> Container >> Port

Data structures:
1-	Ship
  a.	Container Plan [][]		# 2D matric of stacks of containers
  b.	Port Route []			# the path of the ship to the ports
  c.	Int weight				# starts with 0
2-	Container
  a.	Int weight
  b.	Port destPort
  c.	String uniqueId
3-	Port
  a.	String port (5 English letter code)
4-	Container Map (dictionary)
5-	Stack for unloaded containers to be loaded back
6-	Array of linked lists of stacks for loading to the lowest floor

Functions:
1-	Main()
2-	Init_ship(Port route[])
3-	getShipPlan()
4-	getInstructionsForCargo(container Instructions[])
5-	Stowage(ship, port)
6-	Optimize()
7-	Load(ship, container)
8-	Unload(ship, container)
9-	Weight_balance(ship)
10-	Simulation ?

Flow:
1-	Main function calls:
  a.	init_ship to initialize the ship with weight = 0 and the given route
  b.	for ( each port )
          stowage(ship, current_port)
2-	stowage(ship, current_port) calls optimize() to find the number of loads/unloads, optimize calls weight_balance(ship) to make sure the optimal solution gives a balanced ship, then loads/unloads according to the output of the optimize function

Variables can be passed as parameters or be a global variable which can be shared with all the functions

COMP = g++-9.3.0

OBJS = MiniSimulator.o AlgorithmRegistration.o AlgorithmRegistrar.o
EXEC = simulator
CPP_COMP_FLAG = -std=c++2a -Wall -Werror -Wextra -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -ldl -export-dynamic -pthread #(-rdynamic)

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@ 
	
AlgorithmRegistrar.o: AlgorithmRegistrar.cpp #put dependencies here!
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

AlgorithmRegistration.o: AlgorithmRegistration.cpp #put dependencies here!
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

#use g++ -MM main.cpp to see dependencies
MiniSimulator.o: MiniSimulator.cpp #put dependencies here!
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
	
clean:
	rm -f $(OBJS) $(EXEC)

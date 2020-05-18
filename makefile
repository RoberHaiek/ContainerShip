COMP = g++-8.3.0

OBJS = MiniSimulator.o

EXEC = ex1

CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -pedantic-errors -DNDEBUG
//CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -lstdc++fs

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
MiniSimulator.o: MiniSimulator.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)
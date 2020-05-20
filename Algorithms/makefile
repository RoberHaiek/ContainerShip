COMP = g++-9.3.0

SHARED_OBJS = _205962657_a.so 
OBJS = _205962657_a.o
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -pedantic-errors -DNDEBUG -fPIC -fvisibility=hidden
CPP_LINK_FLAG = -shared

all: $(SHARED_OBJS)

_205962657_a.so: $(OBJS)
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

_205962657_a.o: _205962657_a.cpp 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp 
	
clean:
	rm -f $(OBJS) $(SHARED_OBJS)

COMP = g++-9.3.0

SHARED_OBJS_A = _205962657_a.so 
OBJS_A = _205962657_a.o
EXTERNAL_OBJ = IOHandler.o 
EXTERNAL_DIR=../Common/
EXTERNAL_CPP= IOHandler.cpp
SHARED_OBJS_B = _205962657_b.so 
OBJS_B = _205962657_b.o
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -fPIC -fvisibility=hidden 
CPP_LINK_FLAG = -shared

all: $(SHARED_OBJS_A) $(SHARED_OBJS_B)

_205962657_a.so: $(OBJS_A)
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

_205962657_a.o: _205962657_a.cpp 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp 


_205962657_b.so: $(OBJS_B) 
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

_205962657_b.o: _205962657_b.cpp 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp 


	
clean:
	rm -f $(OBJS_A) $(OBJS_B) $(SHARED_OBJS_A) $(SHARED_OBJS_B) 


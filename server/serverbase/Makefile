include ../../cl.mk

APPFILE=$(wildcard *.cpp)
OBJ=$(APPFILE:.cpp=.o)
LIB=libserversocket.a
INC=-I$(SOCKETAPI) -I$(COMMON)

.PHONY:all clean

all:$(LIB)

%.o:%.cpp
	$(CC) -c $< $(INC)
$(LIB):$(OBJ)
	rm -f $@
	ar rsv $@ $^

clean:
	$(RM) *.o
	$(RM) $(LIB)

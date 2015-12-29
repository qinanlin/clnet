include ../cl.mk

SOURCES = $(wildcard *.cpp)
OBJ = $(SOURCES:.cpp=.o)
INC = -I$(COMMON)
SOCKETAPILIB =libsocketapi.a

.PHONY:all clean

all:$(SOCKETAPILIB)
-include $(SOURCES:.cpp=.d)
$(SOCKETAPILIB):$(OBJ)
	rm -rf $(SOCKETAPILIB) 
	ar rsv $(SOCKETAPILIB) $^
%.d:%.cpp
	$(CC) -MM $(INC) $< -o $@
%.o:%.cpp %.d
	$(CC) -c $(INC) $<
Socket.o:SocketAPI.o	

clean:
	$(RM) *.o *.d
	$(RM) $(SOCKETAPILIB)

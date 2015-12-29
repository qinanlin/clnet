include ../cl.mk

APPNAME=conn_client

SOURCE=$(wildcard *.cpp)
APPOBJ=$(SOURCE:.cpp=.o)
INC=-I$(COMMON) -I$(SOCKETAPI)
LIB=$(SOCKETAPI)/libsocketapi.a

.PHONY: all clean

all:$(APPNAME)

-include $(SOURCE:.cpp=.d)  #需放在all之后

%.d:%.cpp
	$(CC) -MM $(INC) $< -o $@
%.o:%.cpp %.d
	$(CC) -g -c $(INC) $<
$(APPNAME):$(APPOBJ)
	$(CC) -o $(APPNAME) $(INC) $^ $(LIB) -lpthread

clean:
	$(RM) *.o *.d
	$(RM) $(APPNAME)

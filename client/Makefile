include ../cl.mk

APPNAME=conn_client

SOURCE=$(wildcard *.cpp)
APPOBJ=$(SOURCE:.cpp=.o)
INC=-I$(COMMON) -I$(SOCKETAPI) -I$(PB)
LIB=$(SOCKETAPI)/libsocketapi.a $(PB)/libpb.a

.PHONY: all clean

all:$(APPNAME)

-include $(SOURCE:.cpp=.d)  #需放在all之后

%.d:%.cpp
	$(CC) -MM $(INC) $< -o $@
%.o:%.cpp %.d
	$(CC) -g -c $(INC) $<
$(APPNAME):$(APPOBJ)
	$(CC) -o $(APPNAME) $(INC) $^ $(LIB) `pkg-config --cflags --libs protobuf`

clean:
	$(RM) *.o *.d
	$(RM) $(APPNAME)

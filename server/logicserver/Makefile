include ../../cl.mk

APPNAME=lgcsvr

SOURCE=$(wildcard *.cpp)
APPOBJ=$(SOURCE:.cpp=.o)
INC=-I$(COMMON) -I$(SOCKETAPI) -I$(SERVERBASE) -I$(LOGAPI) -I$(SHMCHANNEL) -I$(SHAREMEM) -I$(PB)
LIB=$(SERVERBASE)/libserversocket.a $(SHAREMEM)/libsharemem.a $(LOGAPI)/liblogapi.a $(SHMCHANNEL)/libshmchannel.a $(PB)/libpb.a
#OBJ = $(patsubst %,%.o,$(SOURCE))

.PHONY:all clean

all:$(APPNAME)

-include $(SOURCE:.cpp=.d)

%.d:%.cpp
	$(CC) -MM $(INC) $< -o $@

%.o:%.cpp %.d
	$(CC) -c -g $(INC) $<

$(APPNAME):$(APPOBJ)
	$(CC) -o $(APPNAME) $(INC) $^ $(LIB) `pkg-config --cflags --libs protobuf`

clean:
	$(RM) *.o *.d
	$(RM) $(APPNAME)

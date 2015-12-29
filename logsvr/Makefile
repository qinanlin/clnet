include ../cl.mk

APPNAME=logsvr

SOURCE=$(wildcard *.cpp)
APPOBJ=$(SOURCE:.cpp=.o)
INC = -I$(COMMON) -I$(SHMCHANNEL) -I$(SOCKETAPI) -I$(SHAREMEM)
LIB = $(SHMCHANNEL)/libshmchannel.a

.PHONY:all clean

all:$(APPNAME)

-include $(SOURCE:.cpp=.d)

%.d:%.cpp
	$(CC) -MM $(INC) $< -o $@

%.o:%.cpp %.d
	$(CC) -c -g $(INC) $<

$(APPNAME):$(APPOBJ)
	$(CC) -o $(APPNAME) $(INC) $^ $(LIB) 

clean:
	$(RM) *.o *.d
	$(RM) $(APPNAME)

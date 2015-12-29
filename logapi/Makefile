include ../cl.mk

SOURCES = $(wildcard *.cpp)
OBJ = $(SOURCES:.cpp=.o)
INC = -I$(COMMON) -I$(SHMCHANNEL) -I$(LOGSVR) -I$(SHAREMEM)
LIB = $(SHMCHANNEL)/libshmchannel.a
LOGAPILIB = liblogapi.a

.PHONY:all clean

all:$(LOGAPILIB)
-include $(SOURCES:.cpp=.d)
$(LOGAPILIB):$(OBJ)
	rm -rf $(LOGAPILIB) 
	ar rsv $(LOGAPILIB) $^ $(LIB)
%.d:%.cpp
	$(CC) -MM $(INC) $< -o $@
%.o:%.cpp %.d
	$(CC) -c $(INC) $< 

clean:
	$(RM) *.o *.d
	$(RM) $(LOGAPILIB)

include ../cl.mk

SOURCES = $(wildcard *.cpp)
OBJ = $(SOURCES:.cpp=.o)
INC = -I$(COMMON) -I$(SHAREMEM)
LIB = $(SHAREMEM)/libsharemem.a
SHMCHANNELLIB = libshmchannel.a

.PHONY:all clean

all:$(SHMCHANNELLIB)
-include $(SOURCES:.cpp=.d)
$(SHMCHANNELLIB):$(OBJ)
	rm -rf $(SHMCHANNELLIB) 
	ar rsv $(SHMCHANNELLIB) $^ $(LIB)
%.d:%.cpp
	$(CC) -MM $(INC) $< -o $@
%.o:%.cpp %.d
	$(CC) -c $(INC) $< 

clean:
	$(RM) *.o *.d
	$(RM) $(SHMCHANNELLIB)

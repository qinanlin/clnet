include ../cl.mk

SOURCES = $(wildcard *.cpp)
OBJ = $(SOURCES:.cpp=.o)
INC = -I$(COMMON)
SHAREMEMLIB = libsharemem.a

.PHONY:all clean

all:$(SHAREMEMLIB)
-include $(SOURCES:.cpp=.d)
$(SHAREMEMLIB):$(OBJ)
	rm -rf $(SHAREMEMLIB) 
	ar rsv $(SHAREMEMLIB) $^
%.d:%.cpp
	$(CC) -MM $(INC) $< -o $@
%.o:%.cpp %.d
	$(CC) -c $(INC) $<

clean:
	$(RM) *.o *.d
	$(RM) $(SHAREMEMLIB)

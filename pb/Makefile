include ../cl.mk

SOURCES = $(wildcard *.cc)
OBJ = $(SOURCES:.cc=.o)
PBLIB = libpb.a

.PHONY:all clean comp


all:$(PBLIB)
#-include $(SOURCES:.cc=.d)
$(PBLIB):$(OBJ)
	rm -rf $(PBLIB)
	ar rsv $(PBLIB) $^
#%.d:%.cc
#	$(CC) -MM  $< -o $@
%.o:%.cc #%.d
	$(CC) -c  $<

comp:
	protoc --cpp_out=. msg.proto
	#protoc --cpp_out=. msg2.proto	//add new file

clean:
	$(RM) *.o *.d
	$(RM) *.pb.*
	$(RM) $(PBLIB)

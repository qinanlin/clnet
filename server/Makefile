include ../cl.mk

MAKE = make
SUBDIR= serverbase connserver logicserver

SUBCLEAN =$(patsubst %, %.clean, $(SUBDIR))

.PHONY: $(SUBDIR) clean
All:$(SUBDIR)

clean:$(SUBCLEAN)
$(SUBDIR):  #没有依赖文件且当前目录有此文件需定义为伪目标才能执行，否则被认为是最新的而不执行
	cd $@ && $(MAKE)
	

$(SUBCLEAN):
	@echo "make sub module:'$@'...."
	cd $(patsubst %.clean, %, $@) && make clean;
	@echo "finish...."

TARGET=HTTPclient
TARGET-LIB=HTTPclient.a

INSTALL_DIR=$(TARGET_ROOTFS)/opt/goji

modules = $(wildcard *.c)
modules-obj = $(subst .c,.o,$(modules))

-include $(modules-obj:.o=.d)

%.o : %.c
	$(CROSS_COMPILE)gcc $(CPPFLAGS) -c $*.c
	$(CROSS_COMPILE)gcc -MM $(CPPFLAGS) $*.c >$*.d
	
build: $(modules-obj)
	$(CROSS_COMPILE)gcc $(CPPFLAGS) -o $(TARGET) -lrt -lpthread $^ ;\
	$(CROSS_COMPILE)ar rc $(TARGET-LIB) $(modules-obj)

install:
	@if (!(test -d $(INSTALL_DIR))); then \
		mkdir -p $(INSTALL_DIR); \
	fi
	cp $(TARGET) $(INSTALL_DIR)/

clean:
	rm -fr *.o *.d $(TARGET) $(TARGET-LIB)

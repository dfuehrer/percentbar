CC ?= tcc
TGTS = percentbar cpupercentbars
DESTDIR ?= ~
PREFIX ?= /.local
INSTALL_DIR = ${DESTDIR}${PREFIX}/bin

.PHONY : all clean install
all: $(TGTS)
	chmod 755 $(TGTS)

$(TGTS): percentlib.o
percentlib.o: %.o : %.c %.h

clean :
	rm -f $(TGTS)

$(INSTALL_DIR):
	mkdir -p $(INSTALL_DIR)
install : all | $(INSTALL_DIR)
	cp -f $(TGTS) $(INSTALL_DIR)

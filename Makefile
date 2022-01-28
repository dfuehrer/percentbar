CC ?= tcc
#SRC = percentbar.c
TGTS = percentbar
DESTDIR ?= ~
PREFIX ?= /.local
INSTALL_DIR = ${DESTDIR}${PREFIX}/bin

.PHONY : all clean install
all: $(TGTS)
	chmod 755 $(TGTS)

percentbar: percentbar.c percentlib.o

clean :
	rm -f $(TGTS)

$(INSTALL_DIR):
	mkdir -p $(INSTALL_DIR)
install : all | $(INSTALL_DIR)
	cp -f $(TGTS) $(INSTALL_DIR)

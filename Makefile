CC = tcc
SRC = percentbar.c
DESTDIR=~
PREFIX=/.local

percentbar : $(SRC)
	$(CC) -o $@ $(SRC)

PHONY : clean install

clean :
	rm -f percentbar

install : percentbar
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f percentbar ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/percentbar

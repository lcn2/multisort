#!/usr/bin/env make
#
# multisort - sort multiple time logs into a single time log

SHELL= bash
INSTALL= install
DESTDIR= /usr/local/sbin
CC= cc
CFLAGS= -Wall -g3 -O3
#CFLAGS= -Wall -g3
RM= rm

all: multisort

multisort: multisort.c
	${CC} ${CFLAGS} -o multisort multisort.c

clean:
	${RM} -f multisort.o

clobber: clean
	${RM} -f multisort

install: all
	${INSTALL} -m 0555 multisort ${DESTDIR}/multisort

#!/usr/bin/make
#
# multisort - sort multiple time logs into a single time log
#
# @(#) $Revision: 1.6 $
# @(#) $Id: Makefile,v 1.6 2004/01/06 14:10:22 chongo Exp chongo $
# @(#) $Source: /usr/local/src/etc/multisort/RCS/Makefile,v $

SHELL = /bin/sh
INSTALL = install
DESTDIR = /usr/local/etc
CC = gcc
CFLAGS = -Wall -g3 -O3
#CFLAGS = -Wall -g3

all: multisort

multisort: multisort.c
	$(CC) $(CFLAGS) -o multisort multisort.c

clean:
	rm -f multisort.o

clobber: clean
	rm -f multisort

install: all
	${INSTALL} -m 0555 multisort ${DESTDIR}/multisort

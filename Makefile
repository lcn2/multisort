#!/usr/bin/make
#
# multisort - sort multiple time logs into a single time log
#
# @(#) $Revision: 1.5 $
# @(#) $Id: Makefile,v 1.5 2003/10/26 08:45:52 chongo Exp chongo $
# @(#) $Source: /usr/local/src/etc/multisort/RCS/Makefile,v $

SHELL = /bin/sh
INSTALL = install
DESTDIR = /usr/local/etc
CC = gcc
CFLAGS = -Wall -g3 -O3 -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
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

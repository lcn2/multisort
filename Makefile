#!/usr/bin/make
#
# multisort - sort multiple time logs into a single time log
#
# @(#) $Revision: 1.8 $
# @(#) $Id: Makefile,v 1.8 2015/09/06 10:35:51 root Exp $
# @(#) $Source: /usr/local/src/sbin/multisort/RCS/Makefile,v $

SHELL = /bin/sh
INSTALL = install
DESTDIR = /usr/local/sbin
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

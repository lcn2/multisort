#!/usr/bin/make
#
# multisort - sort multiple time logs into a single time log
#
# @(#) $Revision: 1.3 $
# @(#) $Id: Makefile,v 1.3 2001/11/23 10:33:07 chongo Exp chongo $
# @(#) $Source: /usr/local/src/etc/multisort/RCS/Makefile,v $
#
# Copyright (c) 2000 by Landon Curt Noll.  All Rights Reserved.
#
# Permission to use, copy, modify, and distribute this software and
# its documentation for any purpose and without fee is hereby granted,
# provided that the above copyright, this permission notice and text
# this comment, and the disclaimer below appear in all of the following:
#
#       supporting documentation
#       source copies
#       source works derived from this source
#       binaries derived from this source or from derived source
#
# LANDON CURT NOLL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
# INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
# EVENT SHALL LANDON CURT NOLL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
# CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
# USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
# OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
#
# chongo <was here> /\oo/\
#
# Share and enjoy!

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

/***********************************************************************
 *
 * multisort - sort multiple Common Log Format files into a single,
 *             date-ordered file
 *
 * $Id: multisort.c,v 1.15 2004/01/06 14:26:51 chongo Exp $
 *
 * Version 1.0 - 14 Jan 1999
 *
 * Version 1.1 - 28 Oct 1999
 *
 *    - Fixed some potential segfaults by checking the return values
 *      of various functions.
 *
 * Version 1.1.3 - 04 Jan 2004	(unofficial by chongo)
 *
 *    + Fixed sort bug where 01/Feb/2001:03:26:15 was incorrectly sorted
 *      before 31/Jan/2001:23:25:08
 *    + Fixed bugs related to very old dates and dates far in the future.
 *    + Fixed bugs related to processing empty input files.
 *    + Correctly distinguishes between file EOF and read errors.
 *    + Allows multisort to just process a single file.
 *    + Added slightly better sanity checks on timestamp string formats.
 *    + Fixed a bug where multisort could hang on an I/O error.
 *    + Correctly computes POSIX "Seconds since the Epoch" values
 *      with full leapyear rules.
 *    + Speedup as per Bertrand Demiddelaer's patch of:
 *	http://bert.tuxfamily.org/patches/multisort.patch
 *    + Added -m maxage which will output only lines <= maxage seconds old
 *      instead of all lines.
 *    + Updated the usage message.
 *    + Compile with LFS support to be able to process log files >2GB
 *
 *
 * Version 1.1.4 - 2025 Mar 22	(unofficial by chongo)
 *
 *    * Source code whitespace cleanup
 *    * Fix GNU static inline compile problem
 *    * Updated chongo usage message
 *    * Misc changes to get ready for a GitHub release
 *
 * Zachary Beane <xach@mint.net>
 *
 *    Copyright (C) 1999 Zachary Beane
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***********************************************************************/

/*
 * force Large File Usage (LFS) under Linux -
 * see: http://www.suse.de/~aj/linux_lfs.html
 */
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

static char *version = "1.1.4 2025-03-22";

struct _input_file {
        int enabled;
        char *name;
        FILE *in_fh;
        long long atime;
        char buf[BUFSIZ + 1];
};

typedef struct _input_file InputFile;

static char *program = NULL;	/* our name */


/* ANSI-C code produced by gperf version 2.7 */
/* Command-line: gperf -t -k* -L ANSI-C  */
struct month { char *name; int pos; };

long long month_offset[12] = {
	-1,  /* Jan */
	30,  /* Feb */
	58,  /* Mar */
	89,  /* Apr */
	119, /* May */
	150, /* Jun */
	180, /* Jul */
	211, /* Aug */
	242, /* Sep */
	272, /* Oct */
	303, /* Nov */
	333  /* Dec */
};

#define TOTAL_KEYWORDS 12
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 3
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 33
/* maximum key range = 31, duplicates = 0 */

#ifdef __GNUC__
__inline
#endif
static unsigned int
hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 15, 34, 34,  8, 34,
       5, 34, 34, 34,  0, 34, 34, 10,  3, 14,
      34, 34, 34,  9, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 10,  0,  0,
      34,  0, 34, 10, 34, 34, 34, 34,  4, 34,
       0,  0,  0, 34,  0, 34,  0,  0,  0, 34,
      34, 10, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34, 34, 34, 34, 34,
      34, 34, 34, 34, 34, 34
    };
  register int hval = len;

  switch (hval)
    {
      default:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

#ifdef __GNUC__
static __inline
#endif
struct month *
in_word_set (register const char *str, register unsigned int len)
{
  static struct month wordlist[] =
    {
      {""}, {""}, {""},
      {"Jun", 5},
      {""}, {""},
      {"Nov", 10},
      {"Jul", 6},
      {"Feb", 1},
      {""}, {""},
      {"Dec", 11},
      {"Sep", 8},
      {"Jan", 0},
      {""}, {""}, {""},
      {"Oct", 9},
      {"Apr", 3},
      {""}, {""}, {""}, {""},
      {"Mar", 2},
      {""}, {""}, {""}, {""},
      {"Aug", 7},
      {""}, {""}, {""}, {""},
      {"May", 4}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}

/* End of gperf output */
#if 0
void
debug(char *format, ...)
{
        return;
        if (format) {
                va_list args;
                fprintf(stderr, "XXX ");
                va_start(args, format);
                vfprintf(stderr, format, args);
                va_end(args);
                fprintf(stderr, "\n");
        }
        fflush(stderr);
}
#endif


/* conv_time
 *  Take a common log format string and return a sortable value. Use a hash
 *  table for month lookup.
 *
 *  Ok, for this new update, don't be so bloody slack about not checking
 *  return values and string lengths and such. Lazy bastard.
 *
 *  Return POSIX "Seconds since the Epoch" with th extension that a
 *  long long (64 bit) value is returned and the 100/400 year leapyear
 *  rule.
 */

long long
conv_time(char *s)
{
        char *ptr;
        char *orig_ptr;
        int year;
        int mon;
        int mday;
        int hour;
        int min;
        int sec;
        struct month *m;

        ptr = s;
        ptr = strchr(ptr, '[');

        if (ptr == NULL)
                return 0;

        ptr++;			/* skip the bracket */

        if (strlen(ptr) < 21)
                return 0;

        orig_ptr = ptr;
        if (ptr[2] != '/' ||
	    ptr[6] != '/' ||
	    ptr[11] != ':' ||
	    ptr[14] != ':' ||
	    ptr[17] != ':' ||
	    ptr[20] != ' ') {
	    /* malformed date string */
	    return 0;
	}
        ptr[2] = '\0';
        ptr[6] = '\0';
        ptr[11] = '\0';
        ptr[14] = '\0';
        ptr[17] = '\0';
        ptr[20] = '\0';

        mday = atoi(ptr);

        ptr += 3;
        m = in_word_set(ptr, 3);
        if (m == NULL) {
                return 0;
        } else {
                mon = m->pos;
        }


        ptr += 4;
        year = atoi(ptr) - 1900;

        ptr += 5;
        hour = atoi(ptr);

        ptr += 3;
        min = atoi(ptr);

        ptr += 3;
        sec = atoi(ptr);

        /* Restore the characters to their original state (this saves us a
           strdup!) Yay. */

        ptr = orig_ptr;
        ptr[2] = '/';
        ptr[6] = '/';
        ptr[11] = ':';
        ptr[14] = ':';
        ptr[17] = ':';
        ptr[20] = ' ';

	return (sec + (min * 60LL) + (hour * 3600LL) +	    /* sec of day */
		((month_offset[mon]+mday) * 86400LL) +	    /* day of year */
		((year-70) * 31536000LL) +		    /* Epoch year */
		(((year-69)/4) * 86400LL) -		    /* leap days */
		(((year-100)/100) * 86400LL) +		    /* 100yr rule */
		(((year-100)/400) * 86400LL));		    /* 400yr rule */
}


void
usage(void)
{
        fprintf(stderr, "usage: multisort [-h] [-m maxage] LOGFILE1 [LOGFILEn ...]\n"
		        "\n"
			"    -h		print help message and exit\n"
			"    -m maxage	output only lines <= maxage secs old (def: output all lines)\n"
		        "\n"
			"    LOGFILE*	httpd log file in Common Log Format, - ==> read stdin\n"
			"\n"
			"multisort version: %s\n"
			"\n"
			"Copyright (C) 1999 Zachary Beane\n"
			"\n"
			"    This program is free software; you can redistribute it and/or modify\n"
			"    it under the terms of the GNU General Public License as published by\n"
			"    the Free Software Foundation; either version 2 of the License, or\n"
			"    (at your option) any later version.\n"
			"\n"
			"    This program is distributed in the hope that it will be useful,\n"
			"    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			"    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
			"    GNU General Public License for more details.\n"
			"\n"
			"    You should have received a copy of the GNU General Public License\n"
			"    along with this program; if not, write to the Free Software\n"
			"    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n"
			"\n"
			"For copy of the license, visit:\n"
			"\n"
			"    https://github.com/lcn2/multisort/blob/master/LICENSE\n"
			"\n"
			"For a copy of the GNU General Public v2, visit:\n"
			"\n"
			"    https://github.com/lcn2/multisort/blob/master/COPYING\n"
			"\n"
			"Modifications 2000,2001,2003-2004,2023,2025 by:\n"
			"\n"
			"    chongo (Landon Curt Noll) /\\oo/\\\n"
			"\n"
			"    http://www.isthe.com/chongo/index.html\n"
			"    https://github.com/lcn2\n"
			"\n"
			"    Share and enjoy!  :-)\n",
			version);
}


int
main(int argc, char *argv[])
{
        InputFile **if_list	= NULL;
        int if_count		= 0;	/* number of total input files */
        int if_nr		= 0;	/* number of active input files */
        char *ret		= NULL;
        long long min_time	= 0LL;
	long long oldest_time	= 0LL;	/* timestamp of old record to output */
	long long now;			/* prog start time */
	long long max_age;		/* -m arg, ignore this many secs old */
        int min_index		= 0;
	struct timeval utc_now;		/* prog start time in UTC */
	struct timezone ignored;	/* ignored timezone arg */
	extern char *optarg;		/* option argument */
	extern int optind;		/* argv index of the next arg */
        int i;
        int j;

	/* determine the time, now, in UTC */
	if (gettimeofday(&utc_now, &ignored) < 0) {
		perror("gettimeofday");
		exit(1);
	}
	now = (long long)utc_now.tv_sec;

	/*
	 * parse args
	 */
	program = argv[0];
	max_age = now;
	while ((i = getopt(argc, argv, "hm:")) != -1) {
	    switch (i) {
	    case 'h':
                usage();
		exit(2);
		break;
	    case 'm':
		errno = 0;
		max_age = strtoll(optarg, NULL, 0);
		if (errno != 0) {
			perror("strtoll");
			exit(3);
		}
		break;
	    default:
		fprintf(stderr, "%s: invalid command line\n\n", program);
                usage();
		exit(4);
	    }
	}
	if (optind >= argc) {
		fprintf(stderr, "%s: requires one or more logfile args\n\n", program);
                usage();
		exit(4);
	}

	/* compute time oldest record to output - cannot be before epoch */
	if (now > max_age) {
		oldest_time = now - max_age;
	}

        if_list = (InputFile **)malloc(sizeof(void *) * argc);

        if (if_list == NULL) {
                perror("malloc");
                exit(5);
        }

        /* Open up all the files */
        for (i = optind, j = 0; i < argc; i++, j++) {
                if_list[j] = (InputFile *)malloc(sizeof(InputFile));
                if (if_list[j] == NULL) {
                        perror("malloc");
                        exit(6);
                }
                if_list[j]->name = strdup(argv[i]);
		if (strcmp(if_list[j]->name, "-") == 0) {
		    if_list[j]->in_fh = stdin;
		} else {
		    if_list[j]->in_fh = fopen(argv[i], "r");
		}

                if (if_list[j]->in_fh == NULL) {
                        fprintf(stderr, "multisort: %s: %s\n", argv[i],
                                strerror(errno));
                        exit(7);
                }

                if_list[j]->enabled = 1;

                /* Read the first line for each open file */
                ret = fgets(if_list[j]->buf, BUFSIZ, if_list[j]->in_fh);
		if (ret == NULL) {
			if (ferror(if_list[j]->in_fh)) {
				fprintf(stderr,
				    "multisort: read error, file `%s'",
				    if_list[j]->name);
			}
			fclose(if_list[j]->in_fh);
			if_list[j]->enabled = 0;
                } else {
			if_list[j]->atime = conv_time(if_list[j]->buf);
			++if_nr;
		}
		++if_count;
        }

        while (if_nr > 0) {
                min_index = 0;
                min_time = 0x7fffffffffffffffLL;	/* 2^63-1 */
                for (i = 0; i < if_count; i++) {
                        if (!if_list[i]->enabled)
                                continue;

                        if (if_list[i]->atime < min_time) {
                                min_time = if_list[i]->atime;
                                min_index = i;
                        }
                }

                /* output the lowest - silently ignore if too old */
                /* printf("%s ", if_list[min_index]->name); */
		if (min_time >= oldest_time) {
			fputs(if_list[min_index]->buf, stdout);
		}

                /* refill the buffer */
		if (if_list[min_index]->enabled) {
			ret = fgets(if_list[min_index]->buf, BUFSIZ,
				    if_list[min_index]->in_fh);
			if (ret == NULL) {
				if (ferror(if_list[min_index]->in_fh)) {
					fprintf(stderr,
					    "multisort: fgets error, file `%s'",
					    if_list[min_index]->name);
				}
				if_list[min_index]->enabled = 0;
				fclose(if_list[min_index]->in_fh);
				if_nr--;
			} else {
				if_list[min_index]->atime = conv_time(if_list[min_index]->buf);
			}
                }
        }
        exit(0); /*ooo*/
}

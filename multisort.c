/***********************************************************************
 *
 * multisort - sort multiple Common Log Format files into a single, 
 *             date-ordered file
 *
 * $Id: multisort.c,v 1.6 2001/11/23 12:22:50 chongo Exp chongo $
 *
 * Version 1.0 - 14 Jan 1999
 *
 * Version 1.1 - 28 Oct 1999
 *
 *    - Fixed some potential segfaults by checking the return values
 *      of various functions.
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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


struct _input_file {
        int enabled;
        char *name;
        FILE *in_fh;
        long long atime;
        char buf[BUFSIZ + 1];
};

typedef struct _input_file InputFile;


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
__inline
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

        ptr++; 			/* skip the bracket */

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
		(((year-69)/4) * 86400LL) - 		    /* leap days */
		(((year-100)/100) * 86400LL) +		    /* 100yr rule */
		(((year-100)/400) * 86400LL));		    /* 400yr rule */
}


void
usage(void)
{
        fprintf(stderr, "usage: multisort LOGFILE1 LOGFILE2 [LOGFILEn ...]\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "multisort 1.1 Copyright (C) 1999 Zachary Beane\n");
        fprintf(stderr, "This program has NO WARRANTY and is licensed "
                "under the terms of the\nGNU General Public License.\n"
                "http://www.xach.com/multisort/ - bugs to xach@mint.net\n");
                
        exit(1);
}


int
main(int argc, char *argv[])
{
        InputFile **if_list 	= NULL;
        int if_count 		= 0;	/* number of total input files */
        int if_nr		= 0;	/* number of active input files */
        char *ret 		= NULL;
        long long min_time	= 0LL;
        int min_index		= 0;
        int i, j;
        
        if (argc < 2) {
                usage();
        }

        if_list = (InputFile **)malloc(sizeof(void *) * argc);

        if (if_list == NULL) {
                perror("malloc");
                exit(1);
        }

        /* Open up all the files */
        for (i = 1, j = 0; i < argc; i++, j++) {
                if_list[j] = (InputFile *)malloc(sizeof(InputFile));
                if (if_list[j] == NULL) {
                        perror("malloc");
                        exit(1);
                }
                if_list[j]->name = strdup(argv[i]);
                if_list[j]->in_fh = fopen(argv[i], "r");
                
                if (if_list[j]->in_fh == NULL) {
                        fprintf(stderr, "multisort: %s: %s\n", argv[i], 
                                strerror(errno));
                        exit(1);
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
			++if_nr;
		}
		++if_count;
        }

        while (if_nr > 0) {
                min_index = 0;
                min_time = 9223372036854775807LL;
                for (i = 0; i < if_count; i++) {
                        if (!if_list[i]->enabled)
                                continue;

                        if_list[i]->atime = conv_time(if_list[i]->buf);
                        if (if_list[i]->atime < min_time) {
                                min_time = if_list[i]->atime;
                                min_index = i;
                        }
                }

                /* output the lowest */
                /* printf("%s ", if_list[min_index]->name); */
                fputs(if_list[min_index]->buf, stdout);

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
			}
                }
        }

        exit(0);
}




# multisort

Merge multiple httpd Common Log Format logfiles them into a single log


# To install

```sh
make clobber all
sudo make install clobber
```


# To use

```
/usr/local/bin/multisort [-h] [-m maxage] LOGFILE1 [LOGFILEn ...]

    -h          print help message and exit
    -m maxage   output only lines <= maxage secs old (def: output all lines)

    LOGFILE*    httpd log file in Common Log Format, - ==> read stdin

multisort version: 1.1.4 2025-03-22

Copyright (C) 1999 Zachary Beane

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

For copy of the license, visit:

    https://github.com/lcn2/multisort/blob/master/LICENSE

For a copy of the GNU General Public v2, visit:

    https://github.com/lcn2/multisort/blob/master/COPYING

Modifications 2000,2001,2003-2004,2023,2025 by:

    chongo (Landon Curt Noll) /\oo/\

    http://www.isthe.com/chongo/index.html
    https://github.com/lcn2

    Share and enjoy!  :-)
```


# From the original README

    `multisort' is a small program designed to take multiple httpd
    logfiles in the Common Log Format and merge them into a single log,
    sorted by date, sent to standard output.

    This is useful if you're running a single website on multiple hosts
    and using round-robin DNS to do load distribution. With multisort, you
    can take the various logfiles from each server and merge them into a
    single file for analysis.

    Please report any bugs you find to xach@mint.net.

    Zach Beane
    14 January 1999


# See also

* https://github.com/lcn2/multisort


## For an older version 1.1 1999-01-14 of this code, see:

* http://www.xach.com/multisort/

* http://www.xach.com/multisort/multisort-1.1.tar.gz


# Reporting Security Issues

To report a security issue, please visit "[Reporting Security Issues](https://github.com/lcn2/multisort/security/policy)".

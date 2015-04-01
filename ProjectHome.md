# ZIPTOOL version 2.1.2 released #

Package can be found in the downloads, or you can pull the code via subversion.

This release is primarily a documentation update, the package now includes and installs a man page which you can read by running

man ziptool

---

# README #


## NAME ##
> ziptool - purify pkzip archive files

## SYNOPSIS ##
> ziptool pure.pk3 unpure.pk3

## DESCRIPTION ##
> ZIPTool allows Quake3 hackers to distribute client-side modificaitons which
> can circumvent a game servers set to pure.

> As of ZIPTool 2.0.0, you can now specify archives with arbitrary file counts
> and file displacement within the archive and ZIPTool will only purify the
> files with common file names and paths.

> The only command format is

> #### ziptool pure.pk3 unpure.pk3 ####

> where pure.pk3 is the valid archive that the server is expecting and
> unpure.pk3 is the archive that contains the client-side modifications that
> the server would not normally allow.

> ZIPTool will modify the file inplace, once you've purified the archive most
> unzip based utilities will likely give errors on extraction, this is to be
> expected as the CRC32 checksums are overwritten and will not match (i.e.
> the archive has been purified).

## OPTIONS ##
> There are currently no supported options.

> to see basic usage and package info run ziptool without any arguements.

> #### ziptool ####

## DIAGNOSTICS ##
> Error messages and they're causes


> #### "Error file\_name is not compatible with ziptool" ####

> you've probably used some fancy schmancy tool to build your archives deeming
> them incompatible with ziptool.


> #### "Error building file list from file\_name" ####

> somehow you've managed to make the archive unreadable yet bypass the
> archive validation, this is likely due to corruption of the archive.

> #### "Error writing local header(s) to file\_name" ####

> #### "Error writing central directory record(s) to file\_name" ####

> there was an issue writing to the unpure archive, this is likely due to
> corruption of the archive.

## AUTHOR ##
> Written by Victor J. Roemer

## REPORTING ISSUES ##
> Report Bugs to < http://code.google.com/p/ziptool/issues/list >

## KNOWN ISSUES ##
> ziptool 1.0.0 only handled archives of same file count and same file
> displacement.

> ziptool 2.0.0 no validation of archives, would likely lead to corruption
> of the unpure archive.

> ziptool 2.1.0 if a pure archive was invalid ziptool did not properly exit
> and tried to continue.

> ziptool 2.1.1 and older are only compatible with little endian systems.

## COPYRIGHT ##

> The Quake 3 logo is a trademark of ID Software LLC.


> Copyright (C) 2008-2011 Victor J. Roemer AKA Sploit.  License GPLv2: GNU GPL version 2
> < http://www.gnu.org/licenses/gpl-2.0.html >

> This program is free software; you can redistribute it and/or modify it
> under the terms of the GNU General Public License as published by the Free
> Software Foundation; either version 2 of the License, or (at your option)
> any later version.

> This program is distributed in the hope that it will be useful, but WITHOUT
> ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
> FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
> more details.
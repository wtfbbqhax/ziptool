/* ziptool - spoof zipfile CRC32 values
 *
 * Copyright (c) Victor Roemer 2008' aka Caffeinated 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
 * 02110-1301, USA.
 *
 * Build:
 *  gcc -O3 -Wall ziptool.c common.c -o ziptool
 *
 * TODO:  LOTS!!
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long  ulong;
typedef unsigned short ushort;

// Zlib error conditions
#define Z_OK              0
#define Z_STREAM_END      1
#define Z_NEED_DICT       2
#define Z_ERRNO         (-1)
#define Z_STREAM_ERROR  (-2)
#define Z_DATA_ERROR    (-3)
#define Z_MEM_ERROR     (-4)
#define Z_BUF_ERROR     (-5)
#define Z_VERSION_ERROR (-6)

#define Z_COMMENT_SIZE 65536 // XXX Future XXX

typedef struct s_zip {
    char filename[256];
    unsigned int version;
    unsigned int gpflag;
    unsigned int method;
    unsigned int fileTime;
    unsigned int fileDate;
    unsigned int crc32;
    unsigned int cpSize;
    unsigned int uncpSize;
    unsigned int fnSize;
    unsigned int extSize;
    unsigned int dataSize;
    struct s_zip *next;
} zipinfo;

/* Size Specific zip headers
 */
#pragma pack( push, 1 ) // HACK ATTACK WHATCH OUT
typedef struct s_localHeader {
    unsigned int   signature;// 0x04034B50
    unsigned short version;
    unsigned short gpflag;
    unsigned short method;   // maybe I should make this an enum
    unsigned short fileTime; // need to make functions to convert
    unsigned short fileDate; // these to text
    unsigned int   crc32;
    unsigned int   cpSize;   // this == uncpsize if nothing else
    unsigned int   uncpSize;
    unsigned short fnSize;
    unsigned short extSize;  // Should make some stuff for extra
} localheader;

typedef struct s_CentDirectoryHeader {
    unsigned int   signature;// 0x02014B50
    unsigned short version;
    unsigned short version2; // ugh... rename
    unsigned short gpflag;
    unsigned short method;
    unsigned short fileTime;
    unsigned short fileDate;
    unsigned int   crc32;
    unsigned int   cpSize;
    unsigned int   uncpSize;
    unsigned short fnSize;
    unsigned short extSize;
    unsigned short comSize;
    unsigned short disk;
    unsigned short int_attrb;
    unsigned int   ext_attrb;
    unsigned int   localOffset;
} centheader;

typedef struct s_FinalHeader {
    unsigned int   signature; // 0x06054B50
    unsigned short disk;
    unsigned short diskCD;    // same as disk
    unsigned short entriesCD; // I'm confused by this 
    unsigned short entries;
    unsigned int   sizeofCD;  // should be same as entries?
    unsigned int   offsetCD;
    unsigned short comSize;
} zipheader;
#pragma pack( pop )     // ok its safe to come out now

// whew, now that I finished that...

/*====================
 * extern float getVersion( unsigned int version )
 *
 * Return( printable version )
 *====================
 */
extern float getVersion( unsigned int version );

/*====================
 * int getEntries( const char* file )
 *
 * Return the number of files inside a zipfile
 *====================
*/
int getEntries( const char* file );

/*==================== 
 * extern int readZipHeaders( char* file, struct s_zip* zi )
 *
 * Parses through a zipfile and fills in a linked-list 
 * with the header information 
 * TODO
 *      Have it fill in another structure with information from the central
 *      Directory etc..
 *====================
 */
extern int readZipHeaders( char* file, struct s_zip* zi );


/*====================
 * extern int write_zipFile( const char* file, const char* fileout, zipinfo* zi )
 *
 * Interface to all the write_* functions. 
 *
 * Writes a zipfile base on input based on zipinfo* zi and options.
 *====================
 */
extern int spoof_ZipFile( const char* file, const char* fileIn, 
                          const char* fileOut);



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
#include "common.h"

/*==================== 
 * extern float getVersion( unsigned int version )
 *
 * Return( printable version )
 *====================
 */
extern float getVersion( unsigned int version ) 
{ return( (10/version) + (10%version) ); }


/*====================
 * int getEntries( const char* file )
 *
 * Return the number of files inside a zipfile
 *====================
 */
int getEntries( const char* file )
{
    FILE* fpZip = fopen( file, "rb");
    int i = 0;
    char header[30];
    localheader* lo =(localheader*)header;

    if( fpZip != NULL )
    while( fread( header, 1, 30, fpZip ) == 30 ) {
        if( lo->signature == 0x04034B50 ) 
        {
            fseek( fpZip, (lo->fnSize + lo->extSize +
                          (lo->cpSize ? lo->cpSize : lo->uncpSize )),
                          SEEK_CUR ); 
            ++i;
        } else break; 
    }
    fclose( fpZip );
    return i;
} 


/*====================
 * extern int read_zipHeaders( const char* file, zipinfo zi )
 *
 * returns any read error conditions
 *====================
 */
extern int read_zipinfo( const char* file, zipinfo* zi )
{
    int error = Z_OK;
    FILE* fpZip = fopen(file, "rb");

    char header[30];
    localheader* lo = (localheader*)header;
    zipinfo* root; root = zi;

    while(fread( header, 1, 30, fpZip ) == 30)
    {
        if( lo->signature == 0x04034B50 ) {
            zi->version  = lo->version;
            zi->gpflag   = lo->gpflag;
            zi->method   = lo->method;    
            zi->fileTime = lo->fileTime;
            zi->fileDate = lo->fileDate;
            zi->crc32    = lo->crc32;
            zi->cpSize   = lo->cpSize;
            zi->uncpSize = lo->uncpSize;
            zi->fnSize   = lo->fnSize;
            zi->extSize  = lo->extSize;
            zi->dataSize = (lo->cpSize ? lo->cpSize : lo->uncpSize );

            if( zi->fnSize > 0 )
                if( fread( zi->filename, 1, zi->fnSize, fpZip ) != zi->fnSize )
                    { error = Z_STREAM_ERROR; break; }

            if( fseek( fpZip,(zi->extSize ? zi->extSize : 0 ), SEEK_CUR ) != 0 )
                { error = Z_STREAM_ERROR; break; }

            if( fseek( fpZip, zi->dataSize, SEEK_CUR ) != 0 )
                { error = Z_STREAM_ERROR; break; }

        } else { break; }

        if((zi->next=(zipinfo* )malloc(sizeof(zipinfo))) == NULL)
            { perror("malloc"); exit( Z_MEM_ERROR ); }
        zi = zi->next;
    }

    zi = root;
    fclose( fpZip );
    return( error );
}

/*====================
 * extern int write_local( const char* file, const char* fileout, zipinfo zi )
 *
 * write a new zipfile
 *====================
 */
extern int write_local( const char* file, const char* fileout, zipinfo* zi )
{
    int   error = Z_OK;
    FILE* fpZip = fopen(file,    "rb");
    FILE* fpOut = fopen(fileout, "wb");

    zipinfo* root; root = zi;

    char header[30]; char extra[1024]; char filename[255]; char* data;
    localheader* lo = (localheader*)header;
	int dataSize;

    while(fread( header, 1, 30, fpZip ) == 30)
    {
        if( lo->signature == 0x04034B50 ) {
            lo->crc32      = ( zi->crc32 ? zi->crc32 : lo->crc32 );
 
            // data allocation
            dataSize       = ( lo->cpSize ? lo->cpSize : lo->uncpSize );
            if((data=(char*)malloc(dataSize)) == NULL )
                { error = Z_MEM_ERROR; break; }

            // Header
            if( fwrite( lo, 1, sizeof(header), fpOut ) != sizeof(header) )
                { error = Z_ERRNO; break; }

            // Filename
            if( lo->fnSize > 0 )
                {
                if( fread( filename, 1, lo->fnSize, fpZip ) != lo->fnSize )
                    { error = Z_STREAM_ERROR; break; }
                if( fwrite( filename, 1, lo->fnSize, fpOut ) != lo->fnSize )
                    { error = Z_ERRNO; break; }
                }

            // Extra
            if( lo->extSize > 0 )
                {
                if( fread( extra, 1, lo->extSize, fpZip ) != lo->extSize )
                    { error = Z_STREAM_ERROR; break; }
                if( fwrite( extra, 1, lo->extSize, fpOut ) != lo->extSize )
                    { error = Z_ERRNO; break; }
                }

            // Data
            if( dataSize > 0 )
                {
                if( fread( data, 1, dataSize, fpZip )  != dataSize )
                    { error = Z_STREAM_ERROR; break; }
                if( fwrite( data, 1, dataSize, fpOut ) != dataSize )
                    { error = Z_ERRNO; break; }
                }
            if( zi->next )
                zi = zi->next;
        } //else 
//            {
//            char buffer[1024];
//            int  bytesRead;
//            while( bytesRead=(int)fread(buffer, 1, sizeof(buffer), fpZip ) > 0 )
//                if( fwrite( buffer, 1, sizeof(buffer), fpOut ) != bytesRead )
//                    { error = Z_ERRNO; break; }
//                break; 
//            }
    }

    zi = root;
    fclose( fpZip ); 
    fclose( fpOut );
    return( error ); 
}

/*====================
 * extern int write_centdir( const char* file, const char* fileout, zipinfo zi )
 *
 * Test Function, If I get this to work then I will merge in the write_local();
 *
 * returns error;
 *
 * write a new zipfile
 *
 * XXX 
 *      either need to MERGE write_local 
 *      OR
 *      Split write_zipheader from this 
 *      function and make new function
 *      write_zipfile( fileIN, fileOUT, zipinfo* zi )
 *      which calls all three
 *====================
 */
extern int write_centdir( const char* file, const char* fileout, zipinfo* zi )
{
    int   error = Z_OK;
    FILE* fpZip = fopen(file,    "rb");
    FILE* fpOut = fopen(fileout, "ab");

    zipinfo* root; root = zi;

    char header[46]; char filename[255]; char extra[1024];
    localheader* lo = (localheader*)header;
    centheader* cd = (centheader*)header;
    zipheader* zh = (zipheader*)header;

    char* comment = "ziptool has touched your file";
    unsigned long sizeofCD = 0;
    unsigned long offsetCD = 0;

    // need to skip the local header this time. Stolen from getEntries
    // XXX this is better
    while( fread( header, 1, 30, fpZip ) == 30 )
        {
        if( lo->signature == 0x04034B50 )
            {
            fseek( fpZip, (lo->fnSize + lo->extSize +
                          (lo->cpSize ? lo->cpSize : lo->uncpSize )),
                          SEEK_CUR ); 
            offsetCD += (30 + lo->fnSize + lo->extSize +
                          (lo->cpSize ? lo->cpSize : lo->uncpSize ));
            }
            else 
                break; 
        }
    
    // because of the way I skip the local header i need to rewind
    // the file pointer
    fseek( fpZip, -30, SEEK_CUR );

    // Central Directory Entries
    while(fread( header, 1, 46, fpZip ) == 46)
        {
        if( cd->signature == 0x02014B50 )
            {
            cd->crc32 = ( zi->crc32 ? zi->crc32 : cd->crc32 );

            // Header
            if( fwrite( cd, 1, sizeof(header), fpOut ) != sizeof(header) )
                { error = Z_ERRNO; break; }

            // Filename
            if( cd->fnSize > 0 )
                {
                if( fread( filename, 1, cd->fnSize, fpZip ) != cd->fnSize )
                    { error = Z_STREAM_ERROR; break; }
                if( fwrite( filename, 1, cd->fnSize, fpOut ) != cd->fnSize )
                    { error = Z_ERRNO; break; }
                }

            // Extra
            if( cd->extSize > 0 )
                {
                if( fread( extra, 1, cd->extSize, fpZip )  != cd->extSize )
                    { error = Z_STREAM_ERROR; break; }
                if( fwrite( extra, 1, cd->extSize, fpOut ) != cd->extSize )
                    { error = Z_ERRNO; break; }
                }

            // Comment 
            if( cd->comSize > 0 )
                if( fread( comment, 1, cd->comSize, fpZip ) != cd->comSize )
                    { error = Z_STREAM_ERROR; break; }
                if( fwrite( comment, 1, cd->comSize, fpOut ) != cd->comSize )
                    { error = Z_ERRNO; break; }
            sizeofCD += ( 46 + cd->fnSize + cd->extSize + cd->comSize );
            
            if( zi->next )
                zi = zi->next;
            } 
            else break;
        }

    // because of the way I read in header data I need to rewind the file
    // pointer
    fseek( fpZip, -46, SEEK_CUR );

    // Final Header
    // XXX After trying many many times, I was unable to properly read in
    // the zipheader from the fpZip. After an hour of debuging and hex
    // dumping zipfiles I've come to the determination that this is JUST FINE.
    //
      zh->signature = 0x06054B50;
      zh->disk = 0;
      zh->diskCD = 0;
      zh->entriesCD = getEntries(file);
      zh->entries   = getEntries(file);
      zh->sizeofCD  = sizeofCD;
      zh->offsetCD  = offsetCD;
      zh->comSize   = (int)strlen(comment);
      if( fwrite( zh, 1, 22, fpOut ) != 22 )
        { error = Z_ERRNO; }
      if( fwrite( comment, 1, zh->comSize, fpOut ) != zh->comSize )
        { error = Z_ERRNO; }

    zi = root;
    fclose( fpZip );
    fclose( fpOut );
    return( error );
}

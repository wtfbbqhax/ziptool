/********************************************************************* 

   ziptool - spoof zipfile CRC32 values

   Copyright (c) Victor Roemer 2008' aka Caffeinated 

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  
   02110-1301, USA.

*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define PROGRAM_NAME "ziptool"

int spoof_ZipFile();
extern int read_zipinfo();
extern int free_zipinfo();
extern int write_local();
extern int write_centdir();
extern int getEntries();

void print_banner( void );
void print_help( void );

char* program_name;

/************************************
 * main
 ***********************************/
int main( int argc, char** argv )
{
    int error = Z_OK;
    program_name = argv[0];

    print_banner();

    if( argc < 4 )
        print_help();

    // verify we are acutally working with zip archives
    if( !verifyZipFile(argv[1]) ) {
        fprintf( stderr, "ERROR: \'%s\' is not recognized as a valid ZIP archive\n", argv[1] );
        fprintf( stderr, "Ziptool is only compatible with \'pkware ZIP archives\' not RAR, 7zip, Gzip, Bzip2, etc etc..\n");
        exit(-1);
    }
    if( !verifyZipFile(argv[2]) ) {
        fprintf( stderr, "ERROR: \'%s\' is not recognized as a valid ZIP archive\n", argv[2] );
        fprintf( stderr, "Ziptool is only compatible with \'pkware ZIP archives\' not RAR, 7zip, Gzip, Bzip2, etc etc..\n");
        exit(-1);
    }
    // no need to check 3rd arguement, we create it
    
    
    if( getEntries(argv[1]) != getEntries(argv[2]) )
    {
        fprintf( stderr, "ERROR, Conflicting number of entries between \'%s\' and \'%s\'\n", argv[1], argv[2] );
        fprintf( stderr, "Each archive needs to have the _same_ number of files\n"); 
        exit(-1);
    }

    // FIXME stop returning errors everytime pl0x 
    // error = 
    spoof_ZipFile( argv[1], argv[2], argv[3] );

    // KTHXBYE 
    return error;
}


/************************************
 * print_banner
 * display license banner
 ***********************************/
void print_banner( void )
{
    printf("ZipTool version .9b, Copyright (c) 2008 Victor Roemer\n");
    printf("This is free software and is provided as-is WITHOUT WARRANTY!\n\n");
    return;
}

/************************************
 * print_help
 * display basic usage
 ***********************************/
void print_help( void )
{
    printf( "usage: %s zipin.zip zipin2.zip output.zip.\n", program_name );
    printf( "      zipin.zip    zipfile that we want to look like.\n");
    printf( "      zipin2.zip   zipfile we want to spoof.\n");
    printf( "      output.zip   final product.\n\n");
    exit( 1 );
}

/************************************
 * extern int spoof_ZipFile( const char* fileIn, const char* fileOut );
 *
 * Interface to SPOOF Zip file headers
 *
 * Returns any error condition returned by write_* routines. 
 * Exits upon memory errors.
 ***********************************/
/* XXX Question is, should I move write_local & write_centdir
 *  into here?
 *  Probably not, lets leave this as the interactive front end.
 */
int spoof_ZipFile( const char* file, const char* fileIn, const char* fileOut )
{
    int error;

    zipinfo* zi;

    // memory
    if((zi = (zipinfo*)malloc(sizeof(zipinfo))) == NULL)
        { 
        fprintf(stderr, "MALLOC() Error allocating memory\n");
        exit(Z_MEM_ERROR);
        }

    // read/write
    if( (error = read_zipinfo( file, zi )) != Z_OK )
        { 
        free_zipinfo(zi); 
        return error; 
        }
    if( (error = write_local( fileIn, fileOut, zi )) != Z_OK )
        { 
        free_zipinfo(zi); 
        return error; 
        }
    if( (error = write_centdir( fileIn, fileOut, zi )) != Z_OK )
        { 
        free_zipinfo(zi); 
        return error; 
        }

    free_zipinfo( zi );
    return error;
}



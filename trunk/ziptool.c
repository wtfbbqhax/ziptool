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
#include "common.h"

extern int read_zipinfo();
extern int write_local();
extern int write_centdir();

int 
main( int argc, char** argv )
{
    zipinfo* zi;
    zipinfo* root;
	int err;

    printf("ZipTool version .9b, Copyright (c) 2008 Victor Roemer\n\n");
    
    // arguements please 
    if( argc < 4 )
        {
        printf( "usage: %s zipin.zip zipin2.zip output.zip.\n", argv[0] ); 
        printf( "      zipin.zip    zipfile that we want to look like.\n");
        printf( "      zipin2.zip   zipfile we want to spoof.\n");
        printf( "      output.zip   final product.\n\n");
        exit(0); 
        }

    // create 1st node
    if( (zi=(zipinfo*)malloc(sizeof(zipinfo))) == NULL ) 
        {
        printf("Error allocating memory\n"); 
        exit(-1);
        }

    root = zi;

    read_zipinfo( argv[1], zi );

    err = write_local( argv[2], argv[3], zi );
    err = write_centdir( argv[2], argv[3], zi );
    
    while( root ) 
        {
        root = zi->next;
        free(zi);
        zi = root;
        }

    // bai
    return(0);
}

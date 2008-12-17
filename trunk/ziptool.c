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
 */
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

#define PROGRAM_NAME "ziptool"

extern int spoof_ZipFile();

void print_banner( void );
void print_help( void );

char* prog_name;

/*============
 * M A I N ( )
 *============
 */
int main( int argc, char** argv )
{
    int error = Z_OK;
    prog_name = argv[0];

    // banner
    print_banner();

    // arguements please 
    if( argc < 4 )
        print_help();

    // spoof it good 
    spoof_ZipFile( argv[1], argv[2], argv[3] );

    // bai
    return error;
}


/*============
 * print_banner()
 *============
 */
void print_banner( void )
{
    printf("ZipTool version 1.0rc1, Copyright (c) 2008 Victor Roemer\n");
    printf("This is free software and is provided as-is WITHOUT WARRANTY!\n\n");
}

/*============
 * print_help()
 *============
 */
void print_help( void )
{
    printf( "usage: %s zipin.zip zipin2.zip output.zip.\n", prog_name );
    printf( "      zipin.zip    zipfile that we want to look like.\n");
    printf( "      zipin2.zip   zipfile we want to spoof.\n");
    printf( "      output.zip   final product.\n\n");
    exit( 1 );
}

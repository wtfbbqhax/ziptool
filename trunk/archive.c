#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

/************************
 * extern void free_zipinfo( zipinfo* zi )
 *
 * free all nodes of a zipinfo list
 ***********************/
extern void free_zipinfo( zipinfo* zi )
{
    zipinfo* root;
    root = zi;
    while( root )
        {
        root = zi->next;
        free( zi );
        zi = root;
        }
}

/************************ 
 * extern float getVersion( unsigned int version )
 *
 * Return( printable version )
 ***********************/
extern float getVersion( unsigned int version )
{ return( (10/version) + (10%version) ); }

/************************
 * verifyZipFile
 *
 * Checks that the file is actaully a zip archive
 ***********************/
int verifyZipFile( const char* file )
{
    FILE* fpZip = fopen( file, "rb");
    int i = 0;
    char header[30];
    localheader *lo = (localheader*)header;

    //if( (fpZip = fopen(file, "rb")) == NULL )
    //    return 0;

    if( fpZip != NULL )
    {
        if( fread( header, 1, 30, fpZip ) == 30 )
        {
            if( lo->signature == 0x04034B50 ) 
                i = 1;
        }
        fclose( fpZip );
    }
    return i;
} 

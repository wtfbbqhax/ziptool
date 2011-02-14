//////////////////////////////////////////////////////////////////////
// This file is part of ZIPTool, a tool to purify pkzip archives
//
// Copyright (c) 2008-2011 Victor J Roemer AKA Sploit 
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see
// http://www.gnu.org/licenses/gpl-2.0.html
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <iostream>
#include <fstream>
#include <iomanip>

#include <list>
#include <algorithm>

#include "config.h"
#include "zipformat.h"

using namespace std;

typedef struct _FileElement
{
    uint32_t    cksum;
    string      filename;
} FileElement;

// void ShowPackageInfo( void )
//
// Display the package version and the package bug report
static void ShowPackageInfo( void )
{
    cerr << PACKAGE_STRING << endl;
    cerr << "Report bugs to <" << PACKAGE_BUGREPORT << ">" << endl;
}

// void ShowUsage( void )
//
// Tell a user how to use this thing
static void ShowUsage( void )
{
    cerr << "ziptool pure.pk3 unpure.pk3" << endl << endl;
    ShowPackageInfo( );
}

// int FileExistsAndHasPermissions( string, int )
//
// Validate file exists and we have the necessary permissions.
// Report any errors that occur to the user via stderr.
int FileExistsAndHasPermissions( string filename, int permissions )
{
    int ret;

    ret = access( filename.c_str(), permissions );

    if (ret)
        cerr << filename << ": " << strerror(errno) << endl;

    return !ret;
}

// int ValidateArchiveCompatibility( fstream &, list<> & )
//
// Validate the users pkzip archive is compatible with this tool prior
// to potentially destroying their archive.
//
// return 0 on success
// return 1 on failure
int ValidateArchiveCompatibility( fstream &archive )
{
    int ret = 0;

    // validate the local headers and data
    LocalHeader local;

    archive.read( reinterpret_cast<char *> (&local), sizeof(local) );
    while (local.signature == 0x04034B50)
    {
        try
        {
            // Don't do it if we are suppose to expect a data descriptor
            if (local.flags & 0x4 )
                ret = 1;

            archive.seekg( local.namelen + local.extralen + local.compresslen,
                        ios_base::cur );
            archive.read( reinterpret_cast<char *> (&local), sizeof(local) );
        }
        catch (fstream::failure e)
        {
            ret = 1;
        }
    }

    // validate the central directory + as a bonus, ZIP64 formating should fail
    // here as well
    CentralDirectoryRecord central;

    archive.seekg( -(sizeof(local)), ios_base::cur );
    archive.read( reinterpret_cast<char *> (&central), sizeof(central) );
    while( central.signature ==  0x2014B50 ) {
        try
        {
            archive.seekg( central.namelen + central.extralen +
                        central.commentlen, ios_base::cur );
            archive.read( reinterpret_cast<char *> (&central),
                        sizeof(central) );
        }
        catch (fstream::failure e)
        {
            // We are going to end up reading into the End of central directory
            // header this will inevitably cause the failure condition, lets
            // just make sure its an actual failure
            if (central.signature != 0x6054B50)
                ret = 1;

            // Clear the error flags if this was expected, otherwise we will
            // catch a SIGTRAP on the next read/seek etc..  XXX perhaps we
            // should only do this if eof bit was set.
            else
                archive.clear( );
        }
    }

    archive.seekg( 0, ios_base::beg );
    return ret;
}

// int BuildFileList( fstream &, list<FileElement> & )
//
// For a given pkzip archive, build me a list of FileElements
//
// return 0 on success
// return 1 on failure
int BuildFileList(fstream &archive, list<FileElement> &list)
{
    LocalHeader lh;
    FileElement el;
    int ret = 0;

    archive.read( reinterpret_cast<char *> (&lh), sizeof(lh) );
    while( lh.signature == 0x04034B50 )
    {
        try
        {
            // read the filename out of the archive and assgin it to
            // the current file element.
            char * buff = new char[lh.namelen];

            archive.read( buff, lh.namelen );
            el.filename = buff;

            delete [] buff;

            // pull the CRC32 checksum
            el.cksum = lh.cksum;

            // add the element to our list
            list.push_back( el );
            archive.seekg( lh.extralen + lh.compresslen, ios_base::cur );
            archive.read( reinterpret_cast<char *> (&lh), sizeof(lh) );
        }
        catch (fstream::failure e)
        {
            ret = 1;
        }
    }

    archive.seekg( 0, ios_base::beg );
    return ret;
}

// This is my bottom bitch, Sweetest Taboo.
int main(int argc, char *argv[])
{
    list<FileElement> unpure_files;
    list<FileElement> pure_files;
    list<FileElement>::iterator it;

    fstream pure, unpure;

    // enable exceptions for everythang
    pure.exceptions( fstream::eofbit | fstream::failbit | fstream::badbit );
    unpure.exceptions( fstream::eofbit | fstream::failbit | fstream::badbit );

    // basic arguements check
    if (argc != 3)
    {
        ShowUsage( );
        exit( 1 );
    }

    // Check files
    if (!FileExistsAndHasPermissions( argv[1], R_OK ))
        exit( 1 );

    if (!FileExistsAndHasPermissions( argv[2], R_OK | W_OK ))
        exit( 1 );

    // Open the files
    pure.open( argv[1], fstream::in|fstream::binary );
    unpure.open( argv[2], fstream::in|fstream::out|ios::binary);

    // Validate the Archive's
    if (ValidateArchiveCompatibility(pure))
    {
        cerr << "Error " << argv[1] << " is not compatible with ziptool";
        cerr << endl;
        exit( 1 );
    }

    if (ValidateArchiveCompatibility(unpure))
    {
        cerr << "Error " << argv[2] << " is not compatible with ziptool";
        cerr << endl;
        exit( 1 );
    }

    // Build the File List's
    if (BuildFileList(pure, pure_files) )
    {
        cerr << "Error building file list from " << argv[1] << endl;
        exit( 1 );
    }

    if (BuildFileList(unpure, unpure_files))
    {
        cerr << "Error building file list from " << argv[2] << endl;
        exit( 1 );
    }

    // Set the data to be purified
    for (it=unpure_files.begin(); it != unpure_files.end(); it++)
    {
        list<FileElement>::iterator pit;
        for (pit=pure_files.begin(); pit != pure_files.end(); pit++)
        {
            if (pit->filename == it->filename)
                it->cksum = pit->cksum;
        }
    }

    // Purify the archive
    for (it=unpure_files.begin(); it != unpure_files.end(); it++)
    {
        LocalHeader lh;
        try
        {
            unpure.read( reinterpret_cast<char *> (&lh), sizeof(lh) );
            unpure.seekp( -sizeof(lh), ios_base::cur );
            lh.cksum = it->cksum;
            unpure.write( reinterpret_cast<char *> (&lh), sizeof(lh) );
            unpure.seekp( lh.namelen + lh.extralen + lh.compresslen,
                        ios_base::cur );
        }
        catch (fstream::failure f)
        {
            cerr << "Error writing local header(s) to " << argv[2] << endl;
            exit( 1 );
        }
    }

    for (it=unpure_files.begin(); it != unpure_files.end(); it++)
    {
        CentralDirectoryRecord cd;
        try
        {
            unpure.read( reinterpret_cast<char *> (&cd), sizeof(cd) );
            unpure.seekp( -sizeof(cd), ios_base::cur );
            cd.cksum = it->cksum;
            unpure.write( reinterpret_cast<char *> (&cd), sizeof(cd) );
            unpure.seekp( cd.namelen + cd.extralen + cd.commentlen,
                        ios_base::cur );
        }
        catch (fstream::failure f)
        {
            cerr << "Error writing central directory record(s) to " << argv[2];
            cerr << endl;
            exit( 1 );
        }
    }
    return 0;
}

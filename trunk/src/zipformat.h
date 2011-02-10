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
 
 ********************************************************************/
#include <stdint.h>

#define Z_COMMENT_SIZE 65536 // XXX Future XXX

#pragma pack( push, 1 ) // fuck you C++ for not liking __attribute__((packed))
typedef struct s_localHeader {
    uint32_t signature;// 0x04034B50
    uint16_t version;
    uint16_t flags;
    uint16_t method;   // maybe I should make this an enum?
    uint16_t fileTime; // need to make functions to convert
    uint16_t fileDate; // these to text or steal from somthing
    uint32_t cksum;
    uint32_t compresslen;   // this == uncpsize if nothing else
    uint32_t rawlen;
    uint16_t namelen;
    uint16_t extralen;  // Should make some stuff for extra
} LocalHeader;

typedef struct s_CentDirectoryHeader {
    uint32_t   signature;// 0x02014B50
    uint16_t version;
    uint16_t required_version; // ugh... rename
    uint16_t gpflag;
    uint16_t method;
    uint16_t fileTime;
    uint16_t fileDate;
    uint32_t   cksum;
    uint32_t   cpSize;
    uint32_t   uncpSize;
    uint16_t namelen;
    uint16_t extralen;
    uint16_t commentlen;
    uint16_t disk;
    uint16_t int_attrb;
    uint32_t   ext_attrb;
    uint32_t   localOffset;
} CentralDirectoryHeader;

typedef struct s_FinalHeader {
    uint32_t   signature; // 0x06054B50
    uint16_t disk;
    uint16_t diskCD;    // same as disk
    uint16_t entriesCD;
    uint16_t entries;
    uint32_t   sizeofCD;
    uint32_t   offsetCD;
    uint16_t comSize;
} EndOfCentralDirectoryHeader;
#pragma pack( pop )

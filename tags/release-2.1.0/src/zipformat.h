//////////////////////////////////////////////////////////////////////
// ZipTool - spoof zipfile CRC32 values
//
// Copyright (c) Victor Roemer 2011' aka Sploit
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301, USA.
//////////////////////////////////////////////////////////////////////
#include <stdint.h>

// fuck you C++ for not liking __attribute__((packed))
#pragma pack( push, 1 )
typedef struct _LocalHeader {
    uint32_t signature;// 0x04034B50
    uint16_t version;
    uint16_t flags;
    uint16_t method;
    uint16_t fileTime;
    uint16_t fileDate;
    uint32_t cksum;
    uint32_t compresslen;
    uint32_t rawlen;
    uint16_t namelen;
    uint16_t extralen;
} LocalHeader;

typedef struct _CentDirectoryRecord {
    uint32_t signature;// 0x02014B50
    uint16_t version;
    uint16_t required_version;
    uint16_t flags;
    uint16_t method;
    uint16_t fileTime;
    uint16_t fileDate;
    uint32_t cksum;
    uint32_t cpSize;
    uint32_t uncpSize;
    uint16_t namelen;
    uint16_t extralen;
    uint16_t commentlen;
    uint16_t disk;
    uint16_t int_attrb;
    uint32_t ext_attrb;
    uint32_t localOffset;
} CentralDirectoryRecord;

typedef struct _EndOfCentralDirectoryRecord {
    uint32_t signature; // 0x06054B50
    uint16_t disk;
    uint16_t diskCD;
    uint16_t entriesCD;
    uint16_t entries;
    uint32_t sizeofCD;
    uint32_t offsetCD;
    uint16_t commentlen;
} EndOfCentralDirectoryRecord;
#pragma pack( pop )

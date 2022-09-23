/* Copyright (C) 1997-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _BYTESWAP_H
#define _BYTESWAP_H	1

/* The following definitions must all be macros since otherwise some
   of the possible optimizations are not possible.  */

/* Return a value with all bytes in the 16 bit argument swapped.  */
#define bswap_16(x) \
   ((unsigned short int) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))

/* Return a value with all bytes in the 32 bit argument swapped.  */
#define bswap_32(x) \
   ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) |              \
   (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))

/* Return a value with all bytes in the 64 bit argument swapped.  */
#define bswap_64(x) \
   ((((x) & 0xff00000000000000ull) >> 56)                                  \
   | (((x) & 0x00ff000000000000ull) >> 40)                                 \
   | (((x) & 0x0000ff0000000000ull) >> 24)                                 \
   | (((x) & 0x000000ff00000000ull) >> 8)                                  \
   | (((x) & 0x00000000ff000000ull) << 8)                                  \
   | (((x) & 0x0000000000ff0000ull) << 24)                                 \
   | (((x) & 0x000000000000ff00ull) << 40)                                 \
   | (((x) & 0x00000000000000ffull) << 56))

#endif /* byteswap.h */

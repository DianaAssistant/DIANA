/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Product name: redemption, a FLOSS RDP proxy
   Copyright (C) Wallix 2012
   Author(s): Christophe Grosjean, Dominique Lafages
   Based on xrdp Copyright (C) Jay Sorg 2004-2010

   header file. Keylayout object, used by keymap managers
*/


#pragma once

#include "keyboard/keylayout.hpp"

namespace x0001043b {    // Sami (Northern) (Norway) // Sami Extended Norway

const static int LCID = 0x0001043b;

const static char * const locale_name = "se-NO.ext_norway";

static const Keylayout::KeyLayout_t noMod = {
    /* x00 - x07 */    0x0000, 0x001B,    '1',    '2',    '3',    '4',    '5',    '6',
    /* x08 - x0F */       '7',    '8',    '9',    '0', 0x002B, 0x005C, 0x0008, 0x0009,
    /* x10 - x17 */    0x00E1, 0x0161,    'e',    'r',    't', 0x0167,    'u',    'i',
    /* x18 - x1F */       'o',    'p', 0x00E5, 0x014B, 0x000D, 0x0000,    'a',    's',
    /* x20 - x27 */       'd',    'f',    'g',    'h',    'j',    'k',    'l', 0x00F8,
    /* x28 - x2F */    0x00E6, 0x007C, 0x0000, 0x0111,    'z', 0x010D,    'c',    'v',
    /* x30 - x37 */       'b',    'n',    'm', 0x002C, 0x002E, 0x002D, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,    '7',
    /* x48 - x4F */       '8',    '9',    '-',    '4',    '5',    '6',    '+',    '1',
    /* x50 - x57 */       '2',    '3',    '0', 0x002C, 0x0000, 0x0000, 0x017E, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x002E, 0x0000,
};

static const Keylayout::KeyLayout_t shift = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0021, 0x0022, 0x0023, 0x00A4, 0x0025, 0x0026,
    /* x08 - x0F */    0x002F, 0x0028, 0x0029, 0x003D, 0x003F, 0x0060, 0x0008, 0x0000,
    /* x10 - x17 */    0x00C1, 0x0160,    'E',    'R',    'T', 0x0166,    'U',    'I',
    /* x18 - x1F */       'O',    'P', 0x00C5, 0x014A, 0x000D, 0x0000,    'A',    'S',
    /* x20 - x27 */       'D',    'F',    'G',    'H',    'J',    'K',    'L', 0x00D8,
    /* x28 - x2F */    0x00C6, 0x00A7, 0x0000, 0x0110,    'Z', 0x010C,    'C',    'V',
    /* x30 - x37 */       'B',    'N',    'M', 0x003B, 0x003A, 0x005F, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x002C, 0x0000, 0x0000, 0x017D, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000, 0x0000, 0x007F, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */       '/', 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x002E, 0x0000,
};

static const Keylayout::KeyLayout_t altGr = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0000, 0x0040, 0x00A3, 0x0024, 0x20AC, 0x0000,
    /* x08 - x0F */    0x007B, 0x005B, 0x005D, 0x007D, 0x0000, 0x00B4, 0x0008, 0x0009,
    /* x10 - x17 */       'q',    'w', 0x20AC, 0x0000, 0x0000,    'y', 0x0000, 0x00EF,
    /* x18 - x1F */    0x00F5, 0x0000, 0x00A8, 0x007E, 0x000D, 0x0000, 0x00E2, 0x0000,
    /* x20 - x27 */    0x0000, 0x0000, 0x01E7, 0x01E5, 0x0000, 0x01E9, 0x0000, 0x00F6,
    /* x28 - x2F */    0x00E4, 0x0000, 0x0000, 0x0027, 0x0292,    'x', 0x0000, 0x0000,
    /* x30 - x37 */    0x0000, 0x0000, 0x00B5, 0x003C, 0x003E, 0x0000, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01EF, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const Keylayout::KeyLayout_t shiftAltGr = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x08 - x0F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0008, 0x0009,
    /* x10 - x17 */       'Q',    'W', 0x0000, 0x0000, 0x0000,    'Y', 0x0000, 0x00CF,
    /* x18 - x1F */    0x00D5, 0x0000, 0x005E, 0x02C7, 0x000D, 0x0000, 0x00C2, 0x0000,
    /* x20 - x27 */    0x0000, 0x0000, 0x01E6, 0x01E4, 0x0000, 0x01E8, 0x0000, 0x00D6,
    /* x28 - x2F */    0x00C4, 0x0000, 0x0000, 0x002A, 0x01B7,    'X', 0x0000, 0x0000,
    /* x30 - x37 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01EE, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const Keylayout::KeyLayout_t ctrl = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x08 - x0F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0008, 0x0009,
    /* x10 - x17 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x18 - x1F */    0x0000, 0x0000, 0x0000, 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x20 - x27 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x28 - x2F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x30 - x37 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const Keylayout::KeyLayout_t capslock_noMod = {
    /* x00 - x07 */    0x0000, 0x001B,    '1',    '2',    '3',    '4',    '5',    '6',
    /* x08 - x0F */       '7',    '8',    '9',    '0', 0x002B, 0x005C, 0x0008, 0x0009,
    /* x10 - x17 */    0x00C1, 0x0160,    'E',    'R',    'T', 0x0166,    'U',    'I',
    /* x18 - x1F */       'O',    'P', 0x00C5, 0x014A, 0x000D, 0x0000,    'A',    'S',
    /* x20 - x27 */       'D',    'F',    'G',    'H',    'J',    'K',    'L', 0x00D8,
    /* x28 - x2F */    0x00C6, 0x007C, 0x0000, 0x0110,    'Z', 0x010C,    'C',    'V',
    /* x30 - x37 */       'B',    'N',    'M', 0x002C, 0x002E, 0x002D, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x002C, 0x0000, 0x0000, 0x017D, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x002E, 0x0000,
};

static const Keylayout::KeyLayout_t capslock_shift = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0021, 0x0022, 0x0023, 0x00A4, 0x0025, 0x0026,
    /* x08 - x0F */    0x002F, 0x0028, 0x0029, 0x003D, 0x003F, 0x0060, 0x0008, 0x0009,
    /* x10 - x17 */    0x00E1, 0x0161,    'e',    'r',    't', 0x0167,    'u',    'i',
    /* x18 - x1F */       'o',    'p', 0x00E5, 0x014B, 0x000D, 0x0000,    'a',    's',
    /* x20 - x27 */       'd',    'f',    'g',    'h',    'j',    'k',    'l', 0x00F8,
    /* x28 - x2F */    0x00E6, 0x00A7, 0x0000, 0x0111,    'z', 0x010D,    'c',    'v',
    /* x30 - x37 */       'b',    'n',    'm', 0x003B, 0x003A, 0x005F, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x002C, 0x0000, 0x0000, 0x017E, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x002E, 0x0000,
};

static const Keylayout::KeyLayout_t capslock_altGr = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0000, 0x0040, 0x00A3, 0x0024, 0x20AC, 0x0000,
    /* x08 - x0F */    0x007B, 0x005B, 0x005D, 0x007D, 0x0000, 0x00B4, 0x0008, 0x0009,
    /* x10 - x17 */       'Q',    'W', 0x20AC, 0x0000, 0x0000,    'Y', 0x0000, 0x00CF,
    /* x18 - x1F */    0x00D5, 0x0000, 0x00A8, 0x007E, 0x000D, 0x0000, 0x00C2, 0x0000,
    /* x20 - x27 */    0x0000, 0x0000, 0x01E6, 0x01E4, 0x0000, 0x01E8, 0x0000, 0x00D6,
    /* x28 - x2F */    0x00C4, 0x0000, 0x0000, 0x0027, 0x01B7,    'X', 0x0000, 0x0000,
    /* x30 - x37 */    0x0000, 0x0000, 0x00B5, 0x003C, 0x003E, 0x0000, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01EE, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const Keylayout::KeyLayout_t capslock_shiftAltGr = {
    /* x00 - x07 */    0x0000, 0x001B, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x08 - x0F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0008, 0x0009,
    /* x10 - x17 */       'q',    'w', 0x0000, 0x0000, 0x0000,    'y', 0x0000, 0x00EF,
    /* x18 - x1F */    0x00F5, 0x0000, 0x005E, 0x02C7, 0x000D, 0x0000, 0x00E2, 0x0000,
    /* x20 - x27 */    0x0000, 0x0000, 0x01E7, 0x01E5, 0x0000, 0x01E9, 0x0000, 0x00F6,
    /* x28 - x2F */    0x00E4, 0x0000, 0x0000, 0x002A, 0x0292,    'x', 0x0000, 0x0000,
    /* x30 - x37 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,    '*',
    /* x38 - x3F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x40 - x47 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x48 - x4F */    0x0000, 0x0000,    '-', 0x0000, 0x0000, 0x0000,    '+', 0x0000,
    /* x50 - x57 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01EF, 0x0000,
    /* x58 - x5F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x60 - x67 */    0x0000, 0x0000,    '/', 0x0000, 0x000D, 0x0000, 0x0000, 0x0000,
    /* x68 - x6F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x70 - x77 */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    /* x78 - x7F */    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const Keylayout::dkey_t deadkeys[] = {
    { 0x0060, 0x0d, 15, { {0x006f, 0x00f2}  // 'o' = 'ò'
                        , {0x0049, 0x00cc}  // 'I' = 'Ì'
                        , {0x0059, 0x1ef2}  // 'Y' = 'Ỳ'
                        , {0x0041, 0x00c0}  // 'A' = 'À'
                        , {0x0057, 0x1e80}  // 'W' = 'Ẁ'
                        , {0x0045, 0x00c8}  // 'E' = 'È'
                        , {0x0055, 0x00d9}  // 'U' = 'Ù'
                        , {0x0020, 0x0060}  // ' ' = '`'
                        , {0x0065, 0x00e8}  // 'e' = 'è'
                        , {0x0075, 0x00f9}  // 'u' = 'ù'
                        , {0x0077, 0x1e81}  // 'w' = 'ẁ'
                        , {0x0061, 0x00e0}  // 'a' = 'à'
                        , {0x0079, 0x1ef3}  // 'y' = 'ỳ'
                        , {0x004f, 0x00d2}  // 'O' = 'Ò'
                        , {0x0069, 0x00ec}  // 'i' = 'ì'
                        }
    },
    { 0x00b4, 0x0d, 33, { {0x0059, 0x00dd}  // 'Y' = 'Ý'
                        , {0x0053, 0x015a}  // 'S' = 'Ś'
                        , {0x0052, 0x0154}  // 'R' = 'Ŕ'
                        , {0x0057, 0x1e82}  // 'W' = 'Ẃ'
                        , {0x0055, 0x00da}  // 'U' = 'Ú'
                        , {0x004c, 0x0139}  // 'L' = 'Ĺ'
                        , {0x004e, 0x0143}  // 'N' = 'Ń'
                        , {0x004f, 0x00d3}  // 'O' = 'Ó'
                        , {0x007a, 0x017a}  // 'z' = 'ź'
                        , {0x00f8, 0x01ff}  // 'ø' = 'ǿ'
                        , {0x00d8, 0x01fe}  // 'Ø' = 'Ǿ'
                        , {0x0065, 0x00e9}  // 'e' = 'é'
                        , {0x0063, 0x0107}  // 'c' = 'ć'
                        , {0x0061, 0x00e1}  // 'a' = 'á'
                        , {0x0069, 0x00ed}  // 'i' = 'í'
                        , {0x0049, 0x00cd}  // 'I' = 'Í'
                        , {0x0020, 0x00b4}  // ' ' = '´'
                        , {0x0041, 0x00c1}  // 'A' = 'Á'
                        , {0x0043, 0x0106}  // 'C' = 'Ć'
                        , {0x0045, 0x00c9}  // 'E' = 'É'
                        , {0x0079, 0x00fd}  // 'y' = 'ý'
                        , {0x005a, 0x0179}  // 'Z' = 'Ź'
                        , {0x006f, 0x00f3}  // 'o' = 'ó'
                        , {0x006e, 0x0144}  // 'n' = 'ń'
                        , {0x006c, 0x013a}  // 'l' = 'ĺ'
                        , {0x0073, 0x015b}  // 's' = 'ś'
                        , {0x0072, 0x0155}  // 'r' = 'ŕ'
                        , {0x0075, 0x00fa}  // 'u' = 'ú'
                        , {0x0077, 0x1e83}  // 'w' = 'ẃ'
                        , {0x00c5, 0x01fa}  // 'Å' = 'Ǻ'
                        , {0x00c6, 0x01fc}  // 'Æ' = 'Ǽ'
                        , {0x00e6, 0x01fd}  // 'æ' = 'ǽ'
                        , {0x00e5, 0x01fb}  // 'å' = 'ǻ'
                        }
    },
    { 0x00a8, 0x1a, 15, { {0x006f, 0x00f6}  // 'o' = 'ö'
                        , {0x0049, 0x00cf}  // 'I' = 'Ï'
                        , {0x0059, 0x0178}  // 'Y' = 'Ÿ'
                        , {0x0041, 0x00c4}  // 'A' = 'Ä'
                        , {0x0057, 0x1e84}  // 'W' = 'Ẅ'
                        , {0x0045, 0x00cb}  // 'E' = 'Ë'
                        , {0x0055, 0x00dc}  // 'U' = 'Ü'
                        , {0x0020, 0x00a8}  // ' ' = '¨'
                        , {0x0065, 0x00eb}  // 'e' = 'ë'
                        , {0x0075, 0x00fc}  // 'u' = 'ü'
                        , {0x0077, 0x1e85}  // 'w' = 'ẅ'
                        , {0x0061, 0x00e4}  // 'a' = 'ä'
                        , {0x0079, 0x00ff}  // 'y' = 'ÿ'
                        , {0x004f, 0x00d6}  // 'O' = 'Ö'
                        , {0x0069, 0x00ef}  // 'i' = 'ï'
                        }
    },
    { 0x005e, 0x1a, 25, { {0x0059, 0x0176}  // 'Y' = 'Ŷ'
                        , {0x0053, 0x015c}  // 'S' = 'Ŝ'
                        , {0x0057, 0x0174}  // 'W' = 'Ŵ'
                        , {0x0055, 0x00db}  // 'U' = 'Û'
                        , {0x004a, 0x0134}  // 'J' = 'Ĵ'
                        , {0x004f, 0x00d4}  // 'O' = 'Ô'
                        , {0x0067, 0x011d}  // 'g' = 'ĝ'
                        , {0x0065, 0x00ea}  // 'e' = 'ê'
                        , {0x0063, 0x0109}  // 'c' = 'ĉ'
                        , {0x0061, 0x00e2}  // 'a' = 'â'
                        , {0x0068, 0x0125}  // 'h' = 'ĥ'
                        , {0x0069, 0x00ee}  // 'i' = 'î'
                        , {0x0048, 0x0124}  // 'H' = 'Ĥ'
                        , {0x0049, 0x00ce}  // 'I' = 'Î'
                        , {0x0020, 0x005e}  // ' ' = '^'
                        , {0x0041, 0x00c2}  // 'A' = 'Â'
                        , {0x0043, 0x0108}  // 'C' = 'Ĉ'
                        , {0x0045, 0x00ca}  // 'E' = 'Ê'
                        , {0x0047, 0x011c}  // 'G' = 'Ĝ'
                        , {0x006f, 0x00f4}  // 'o' = 'ô'
                        , {0x006a, 0x0135}  // 'j' = 'ĵ'
                        , {0x0073, 0x015d}  // 's' = 'ŝ'
                        , {0x0075, 0x00fb}  // 'u' = 'û'
                        , {0x0077, 0x0175}  // 'w' = 'ŵ'
                        , {0x0079, 0x0177}  // 'y' = 'ŷ'
                        }
    },
    { 0x007e, 0x1b, 11, { {0x006f, 0x00f5}  // 'o' = 'õ'
                        , {0x0049, 0x0128}  // 'I' = 'Ĩ'
                        , {0x0020, 0x007e}  // ' ' = '~'
                        , {0x006e, 0x00f1}  // 'n' = 'ñ'
                        , {0x0041, 0x00c3}  // 'A' = 'Ã'
                        , {0x0055, 0x0168}  // 'U' = 'Ũ'
                        , {0x0075, 0x0169}  // 'u' = 'ũ'
                        , {0x0061, 0x00e3}  // 'a' = 'ã'
                        , {0x004e, 0x00d1}  // 'N' = 'Ñ'
                        , {0x004f, 0x00d5}  // 'O' = 'Õ'
                        , {0x0069, 0x0129}  // 'i' = 'ĩ'
                        }
    },
    { 0x02c7, 0x1b, 27, { {0x0053, 0x0160}  // 'S' = 'Š'
                        , {0x0052, 0x0158}  // 'R' = 'Ř'
                        , {0x0054, 0x0164}  // 'T' = 'Ť'
                        , {0x004b, 0x01e8}  // 'K' = 'Ǩ'
                        , {0x004c, 0x013d}  // 'L' = 'Ľ'
                        , {0x004e, 0x0147}  // 'N' = 'Ň'
                        , {0x007a, 0x017e}  // 'z' = 'ž'
                        , {0x01b7, 0x01ee}  // 'Ʒ' = 'Ǯ'
                        , {0x0067, 0x01e7}  // 'g' = 'ǧ'
                        , {0x0064, 0x010f}  // 'd' = 'ď'
                        , {0x0065, 0x011b}  // 'e' = 'ě'
                        , {0x0063, 0x010d}  // 'c' = 'č'
                        , {0x0068, 0x021f}  // 'h' = 'ȟ'
                        , {0x0048, 0x021e}  // 'H' = 'Ȟ'
                        , {0x0020, 0x02c7}  // ' ' = 'ˇ'
                        , {0x0043, 0x010c}  // 'C' = 'Č'
                        , {0x0044, 0x010e}  // 'D' = 'Ď'
                        , {0x0045, 0x011a}  // 'E' = 'Ě'
                        , {0x0047, 0x01e6}  // 'G' = 'Ǧ'
                        , {0x005a, 0x017d}  // 'Z' = 'Ž'
                        , {0x006e, 0x0148}  // 'n' = 'ň'
                        , {0x006b, 0x01e9}  // 'k' = 'ǩ'
                        , {0x006c, 0x013e}  // 'l' = 'ľ'
                        , {0x0292, 0x01ef}  // 'ʒ' = 'ǯ'
                        , {0x0073, 0x0161}  // 's' = 'š'
                        , {0x0072, 0x0159}  // 'r' = 'ř'
                        , {0x0074, 0x0165}  // 't' = 'ť'
                        }
    },
};

const static uint8_t nbDeadkeys = 6;

} // END NAMESPACE - x0001043b

static const Keylayout keylayout_x0001043b( x0001043b::LCID
                                          , x0001043b::locale_name
                                          , x0001043b::noMod
                                          , x0001043b::shift
                                          , x0001043b::altGr
                                          , x0001043b::shiftAltGr
                                          , x0001043b::ctrl
                                          , x0001043b::capslock_noMod
                                          , x0001043b::capslock_shift
                                          , x0001043b::capslock_altGr
                                          , x0001043b::capslock_shiftAltGr
                                          , x0001043b::deadkeys
                                          , x0001043b::nbDeadkeys
);


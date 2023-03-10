//Copyright 2023 Leonel Alejandro Aguilera Correia
/*
This file is part of Desensamblador AVR.

Desensamblador AVR is free software: you can redistribute it and/or
modify it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or any later version.

Desensamblador AVR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public
License along with Foobar. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __UNITCONVERSIONS_H__
#define __UNITCONVERSIONS_H__

#include <stdint.h>
#include <stdio.h>

typedef uint8_t byte;
typedef uint16_t word;

byte hexToByte(const char* hexStr);
word hexToWord(const char* hexStr, bool SwapBytes = true);
byte singleHexCharToBin(const char hexChar);

bool singleHexCharToBin_UnitTest();
bool hexToByte_UnitTest();
bool hexToWord_UnitTest();

void run_unitConversions_unit_tests();
#endif // !__UNITCONVERSIONS_H__
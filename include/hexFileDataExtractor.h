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

#ifndef __HexFileDataExtractor_H__
#define	__HexFileDataExtractor_H__

#include "hexline.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

DataLinkedList getDataFromHex(FILE* fp);

#endif // !__HexFileDataExtractor_H__

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

#ifndef __HEXLINE_H__
#define __HEXLINE_H__

#include "dataLinkedList.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

class HexLine {
private:
	byte byteCount;
	word address;
	byte recordType;
	DataLinkedList dataHead;
	byte checksum;

public:
	HexLine* nextLine;

	//HexLine();
	HexLine(FILE* fp);
	~HexLine();

	byte getByteCount();
	word getAddress();
	byte getRecordType();
	DataLinkedList getDataHead();
	byte getChecksum();

	void printData();
	bool isEOF();
};
#endif // !__HEXLINE_H__

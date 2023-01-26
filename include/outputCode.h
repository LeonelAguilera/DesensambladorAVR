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
License along with Desensamblador AVR. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __OUTPUTCODE_H__
#define __OUTPUTCODE_H__

#include "instruction.h"

class OutputCodeLine {
public:
	char _codeLine[INSTRUCTION_MAX_LENGTH];
	OutputCodeLine* next;

public:
	OutputCodeLine() : _codeLine(""), next(0) {};
	OutputCodeLine(const char* codeLine) : next(0) {
		strncpy_s(this->_codeLine, INSTRUCTION_MAX_LENGTH, codeLine, INSTRUCTION_MAX_LENGTH);
	}
};

class OutputCode {
public:
	OutputCodeLine* head;
	OutputCodeLine* tail;
	size_t numberOfLines;

public:
	OutputCode() : head(0), tail(0), numberOfLines(0) {};
	void addLine(const char* codeLine);
	const char* turnIntoString();
};

#endif //__OUTPUTCODE_H__

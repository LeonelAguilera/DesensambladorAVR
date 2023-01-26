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

#include "outputCode.h"

void OutputCode::addLine(const char* codeLine)
{
	if (this->head == 0) {
		this->head = (OutputCodeLine*)malloc(sizeof(OutputCodeLine));
		if (this->head == 0) {
			perror("Error output code");
			return;
		}
		*this->head = OutputCodeLine(codeLine);
		this->tail = this->head;
	}
	else {
		this->tail->next = (OutputCodeLine*)malloc(sizeof(OutputCodeLine));
		if (this->tail->next == 0) {
			perror("Error adding code line");
			return;
		}
		*this->tail->next = OutputCodeLine(codeLine);
		this->tail = this->tail->next;
	}
	this->numberOfLines++;
}

const char* OutputCode::turnIntoString()
{
	size_t numberOfBytes = (this->numberOfLines)*INSTRUCTION_MAX_LENGTH*sizeof(char);
	char* outputString = (char*)malloc(numberOfBytes);
	if (outputString == nullptr) {
		return nullptr;
	}
	OutputCodeLine* currentLine = this->head;
	while (currentLine != 0) {
		strcat_s(outputString, numberOfBytes, currentLine->_codeLine);
	}
	return outputString;
}

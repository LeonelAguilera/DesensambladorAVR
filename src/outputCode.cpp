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

void OutputCode::addLine(const char* codeLine, size_t currentLine)
{
	if (this->head == 0) {
		this->head = (OutputCodeLine*)malloc(sizeof(OutputCodeLine));
		if (this->head == 0) {
			perror("Error output code");
			return;
		}
		*this->head = OutputCodeLine(codeLine, currentLine);
		this->tail = this->head;
	}
	else {
		this->tail->next = (OutputCodeLine*)malloc(sizeof(OutputCodeLine));
		if (this->tail->next == 0) {
			perror("Error adding code line");
			return;
		}
		*this->tail->next = OutputCodeLine(codeLine, currentLine);
		this->tail = this->tail->next;
	}
	this->numberOfLines++;
}

const char* OutputCode::turnIntoString(const LabelLinkedList* codeLabels)
{
	size_t numberOfChars = (this->numberOfLines+codeLabels->getNumberOfLabels())*INSTRUCTION_MAX_LENGTH;
	char* outputString = (char*)calloc(numberOfChars, sizeof(char));
	if (outputString == nullptr) {
		perror("Error turning label list into string");
		return nullptr;
	}
	LabelNode* label = codeLabels->getSortedArray();

	OutputCodeLine* currentLine = this->head;
	size_t currentLabelIndex = 0;
	while (currentLine != 0) {
		if (currentLine->line == label[currentLabelIndex].line) {
			char label_text[INSTRUCTION_MAX_LENGTH];
			label[currentLabelIndex].print(label_text);
			strcat_s(outputString, numberOfChars * sizeof(char), label_text);
			currentLabelIndex++;
			if (currentLabelIndex >= codeLabels->getNumberOfLabels())
				currentLabelIndex = 0;
		}
		strcat_s(outputString, numberOfChars* sizeof(char), currentLine->_codeLine);
		currentLine = currentLine->next;
	}
	return outputString;
}

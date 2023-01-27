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

#ifndef __CODE_MARKS_H__
#define __CODE_MARKS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "unitConversions.h"

#define MAX_LABEL_NAME_LENGTH 20
#define NUMBER_OF_INTERRUPTIONS 26

class LabelNode {
public:
	char* labelName;
	word line;
	LabelNode* next;

public:
	LabelNode() : labelName(0), line(0), next(0) {};
	LabelNode(const char* name, word line);
	LabelNode(word currentLine, word destinationLine, size_t numberOfLabels);

	LabelNode(const LabelNode& base);
//#ifdef __INSTRUCTION_H__
	void print(char* printedText);
//#endif // __INSTRUCTION_H__

private:
	void init(const char* name, word line);
};

class LabelLinkedList {
public:
	LabelNode* tail;
	LabelNode* head;

public:
	LabelLinkedList() :tail(0), head(0), numberOfLabels(0) {};
	LabelLinkedList(const LabelLinkedList& base);
	static const char interruptNames[NUMBER_OF_INTERRUPTIONS][MAX_LABEL_NAME_LENGTH];

	void getLabelName(word currentLine, word destinationLine, char* labelName);
	bool addNode(word currentLine, word destinationLine);
	bool isDestinationAlreadyAdded(word destinationLine, char* destinationLabelName);
	size_t getNumberOfLabels() const { return this->numberOfLabels; };

	LabelNode* toArray() const ;
	LabelNode* getSortedArray() const;

#ifdef _DEBUG
	void testSortedArray();
#endif // _DEBUG

private:
	size_t numberOfLabels;
};

#endif // !__CODE_MARKS_H__
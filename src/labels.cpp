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

#include "labels.h"

void LabelLinkedList::getLabelName(word currentLine, word destinationLine, char* labelName)
{
	if (this->isDestinationAlreadyAdded(destinationLine, labelName)) {
		return;
	}

	if (this->addNode(currentLine, destinationLine)) {
		strncpy_s(labelName, MAX_LABEL_NAME_LENGTH, this->tail->labelName, MAX_LABEL_NAME_LENGTH);
	}
	else {
		sprintf_s(labelName, MAX_LABEL_NAME_LENGTH, "0x%04X", destinationLine);
	}
}

bool LabelLinkedList::addNode(word currentLine, word destinationLine) {
	if (this->head == 0) {
		this->head = (LabelNode*)malloc(sizeof(LabelNode));
		if (this->head == 0) {
			perror("Error creating label list");
			return false;
		}
		*this->head = LabelNode(currentLine, destinationLine, &this->numberOfUnnamedLabels);
		this->tail = this->head;
	}
	else {
		this->tail->next = (LabelNode*)malloc(sizeof(LabelNode));
		if (this->tail->next == 0) {
			perror("Error adding label to list");
			return false;
		}
		*this->tail->next = LabelNode(currentLine, destinationLine, &this->numberOfUnnamedLabels);
		this->tail = this->tail->next;
	}
	return true;
}

bool LabelLinkedList::isDestinationAlreadyAdded(word destinationLine, char* destinationLabelName) {
	LabelNode* temp = this->head;
	while (temp != 0) {
		if (temp->line == destinationLine) {
			strncpy_s(destinationLabelName, MAX_LABEL_NAME_LENGTH, temp->labelName, MAX_LABEL_NAME_LENGTH);
			return true;
		}
		else {
			temp = temp->next;
		}
	}
	return false;
}

LabelNode::LabelNode(const char* name, word line) {
	this->init(name, line);
}

LabelNode::LabelNode(word currentLine, word destinationLine, uint32_t* numberOfUnnamedLabels) {
	char newName[MAX_LABEL_NAME_LENGTH];
	bool isPartOfTheInterruptVector = (currentLine / 2) < NUMBER_OF_INTERRUPTIONS;

	if (isPartOfTheInterruptVector) {
		strcpy_s(newName, MAX_LABEL_NAME_LENGTH, LabelLinkedList::interruptNames[currentLine / 2]);
	}
	else {
		sprintf_s(newName, MAX_LABEL_NAME_LENGTH, "MARK_%d", *numberOfUnnamedLabels);
		(* numberOfUnnamedLabels)++;
	}
	this->init(newName, destinationLine);
}

LabelNode::LabelNode(const LabelNode& base) {
	this->init(base.labelName, base.line);
	if (base.next != 0) {
		this->next = (LabelNode*)malloc(sizeof(LabelNode));
		if (this->next != 0) {
			*this->next = LabelNode(*base.next);
		}
		else {
			perror("Error copiando LabelLinkedList");
		}
	}
	else {
		this->next = 0;
	}
}

void LabelNode::init(const char* name, word line) {
	this->next = 0;
	this->line = line;
	this->labelName = (char*)malloc(sizeof(const char) * MAX_LABEL_NAME_LENGTH);
	if (this->labelName != 0) {
		strcpy_s(this->labelName, MAX_LABEL_NAME_LENGTH, name);
	}
	else {
		perror("Error creating LabelLinkedList element");
	}
}

const char LabelLinkedList::interruptNames[NUMBER_OF_INTERRUPTIONS][MAX_LABEL_NAME_LENGTH] = { "RESET", "INT0", "INT1", "PCINT0", "PCINT1", "PCINT2", "WDT", "TIMER2 COMPA", "TIMER2 COMPB", "TIMER2 OVF", "TIMER1 CAPT", "TIMER1 COMPA", "TIMER1 COMPB", "TIMER1 OVF", "TIMER0 COMPA", "TIMER0 COMPB", "TIMER0 OVF", "SPI, STC", "USART, RX", "USART, UDRE", "USART, TX", "ADC", "EE READY", "ANALOG COMP", "TWI", "SPM READY" };
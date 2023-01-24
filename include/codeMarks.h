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

#ifndef __CODE_MARKS_H__
#define __CODE_MARKS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "unitConversions.h"

#define MAX_MARK_NAME_LENGTH 20
#define INTERRUPTION_NUMBER 26

class CodeMarkLinkedList {
public:
	char* markName;
	word line;
	CodeMarkLinkedList* next;
public:
	CodeMarkLinkedList(): markName(0), line(0), next(0) {};

	CodeMarkLinkedList(const char* name, word line){
		this->init(name, line);
	}

	CodeMarkLinkedList(word currentLine, word destinationLine){
		const char interruptNames[INTERRUPTION_NUMBER][MAX_MARK_NAME_LENGTH] = { "RESET", "INT0", "INT1", "PCINT0", "PCINT1", "PCINT2", "WDT", "TIMER2 COMPA", "TIMER2 COMPB", "TIMER2 OVF", "TIMER1 CAPT", "TIMER1 COMPA", "TIMER1 COMPB", "TIMER1 OVF", "TIMER0 COMPA", "TIMER0 COMPB", "TIMER0 OVF", "SPI, STC", "USART, RX", "USART, UDRE", "USART, TX", "ADC", "EE READY", "ANALOG COMP", "TWI", "SPM READY" };
		char newName[MAX_MARK_NAME_LENGTH];
		if (this->markName != 0) {
			if ((currentLine / 2) < INTERRUPTION_NUMBER) {
				strcpy_s(newName, MAX_MARK_NAME_LENGTH, interruptNames[line/2]);
			}
			else {
				sprintf_s(newName, MAX_MARK_NAME_LENGTH, "MARK_%d",this->marknum);
				this->marknum++;
			}
		}
		else {
			perror("Error creating CodeMarkLinkedList element");
		}
		this->init(newName, destinationLine);
	}

	CodeMarkLinkedList(const CodeMarkLinkedList& base) {
		this->init(base.markName, base.line);
		if (base.next != 0) {
			this->next = (CodeMarkLinkedList*)malloc(sizeof(CodeMarkLinkedList));
			if (this->next != 0) {
				*this->next = CodeMarkLinkedList(*base.next);
			}
			else {
				perror("Error copiando CodeMarkLinkedList");
			}
		}
		else {
			this->next = 0;
		}
	}

	CodeMarkLinkedList initializeInterrupts() {
		*this = CodeMarkLinkedList(interruptNames[0], 0);
		CodeMarkLinkedList temp = *this;
		for (uint8_t i = 1; i < INTERRUPTION_NUMBER; i++) {
			temp.next = (CodeMarkLinkedList*)malloc(sizeof(CodeMarkLinkedList));
			if (temp.next != 0) {
				*temp.next = CodeMarkLinkedList(interruptNames[i], 2 * i);
				temp = *temp.next;
			}
			else {
				perror("Error initializing CodeMarkLinkedList");
			}
		}
		return temp;
	}
private:
	static uint32_t marknum;
	static const char interruptNames[INTERRUPTION_NUMBER][MAX_MARK_NAME_LENGTH];
	void init(const char* name, word line) {
		this->next = 0;
		this->line = line;
		this->markName = (char*)malloc(sizeof(const char) * MAX_MARK_NAME_LENGTH);
		if (this->markName != 0) {
			strcpy_s(this->markName, MAX_MARK_NAME_LENGTH, name);
		}
		else {
			perror("Error creating CodeMarkLinkedList element");
		}
	}
};

uint32_t CodeMarkLinkedList::marknum = 0;
const char CodeMarkLinkedList::interruptNames[INTERRUPTION_NUMBER][MAX_MARK_NAME_LENGTH] = { "RESET", "INT0", "INT1", "PCINT0", "PCINT1", "PCINT2", "WDT", "TIMER2 COMPA", "TIMER2 COMPB", "TIMER2 OVF", "TIMER1 CAPT", "TIMER1 COMPA", "TIMER1 COMPB", "TIMER1 OVF", "TIMER0 COMPA", "TIMER0 COMPB", "TIMER0 OVF", "SPI, STC", "USART, RX", "USART, UDRE", "USART, TX", "ADC", "EE READY", "ANALOG COMP", "TWI", "SPM READY" };

#endif // !__CODE_MARKS_H__
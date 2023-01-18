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

#ifndef __DATA_LINKED_LIST_H__
#define __DATA_LINKED_LIST_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef uint8_t byte;
typedef uint16_t word;


class DataLinkedList {
public:
	word data;
	DataLinkedList* next;
public:
	DataLinkedList() : data(0), next(0) {};
	DataLinkedList(word data) : data(data), next(0) {};
	DataLinkedList(const DataLinkedList& base) {
		this->data = base.data;
		if(base.next != 0) {
			/*
			temp_data->next = (DataLinkedList*)malloc(sizeof(DataLinkedList));
			if (temp_data->next != 0) {
				*temp_data->next = DataLinkedList(temp_line.getDataHead());
			}*/
			this->next = (DataLinkedList*)malloc(sizeof(DataLinkedList));
			if (this->next != 0) {
				*this->next = DataLinkedList(*base.next);
			}
			else {
				perror("Error copiando DataLinkedList");
			}
		}
		else {
			this->next = 0;
		}
	};
	DataLinkedList getNext() {
		if (this->next != 0){
			return *this->next;
		}
		else {
			return *this;
		}
	};

	void print() {
		printf("0x%04X\n", this->data);
		if (this->next != 0) {
			this->next->print();
		}
	}
};


byte hexToByte(const char* hexStr);
word hexToWord(const char* hexStr);
byte singleHexCharToBin(const char hexChar);

bool singleHexCharToBin_UnitTest();
bool hexToByte_UnitTest();
bool hexToWord_UnitTest();

void run_unit_tests();
#endif // !__DATA_LINKED_LIST_H__

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

#include "hexline.h"
#include "dataLinkedList.h"
#include <stdio.h>
#include <stdlib.h>

HexLine::HexLine(FILE* fp) {
	while (fgetc(fp) != ':') {
		if (feof(fp)) {
			this->byteCount = 0;
			this->address = 0;
			this->recordType = 0;
			//this->dataHead = 0; //�How did this compiled in the first place?
			this->dataHead = DataLinkedList(); //Temporary fix
			this->checksum = 0;
			this->nextLine = 0;
			return;
		}
	}
	char buffer[5]{};
	fgets(buffer, 3, fp);
	this->byteCount = hexToByte(buffer);
	fgets(buffer, 5, fp);
	this->address = hexToWord(buffer, false)/2;
	fgets(buffer, 3, fp);
	this->recordType = hexToByte(buffer);

	DataLinkedList* temp = &this->dataHead;
	if (this->byteCount / 2 > 0) {
		fgets(buffer, 5, fp);
		temp->data = hexToWord(buffer);
		temp->line = this->address;
		for (uint8_t i = 1; i < this->byteCount / 2; i++) {
			fgets(buffer, 5, fp);
			temp->next = new DataLinkedList(hexToWord(buffer), this->address+i);
			temp = temp->next;
		}
	}
	fgets(buffer, 3, fp);
	this->checksum = hexToByte(buffer);
	this->nextLine = 0;
}

byte HexLine::getByteCount() {
	return this->byteCount;
}
word HexLine::getAddress() {
	return this->address;
}
byte HexLine::getRecordType() {
	return this->recordType;
}
DataLinkedList HexLine::getDataHead() {
	return this->dataHead;
}
byte HexLine::getChecksum() {
	return this->checksum;
}

void HexLine::printData() {
	DataLinkedList temp = this->dataHead;
	for (uint8_t i = 0; i < this->byteCount / 2; i++) {
		printf("0x%04X\n", temp.data);
		if (temp.next != 0) {
			temp = temp.getNext();
		}
	}
}

bool HexLine::isEOF() {
	return this->recordType == 0x01;
}

HexLine::~HexLine() {
	//TODO
}


byte hexToByte(const char* hexStr) {
	if (hexStr[2] != '\0') {
		return 0; //Error
	}

	return singleHexCharToBin(hexStr[0]) << 4 | singleHexCharToBin(hexStr[1]);
}
word hexToWord(const char* hexStr, bool SwapBytes) {
	if (hexStr[4] != '\0') {
		return 0; //Error
	}

	if (SwapBytes) {
		return ((singleHexCharToBin(hexStr[2]) << 4 | singleHexCharToBin(hexStr[3])) << 8) | (singleHexCharToBin(hexStr[0]) << 4 | singleHexCharToBin(hexStr[1]));
	}
	else {
		return ((singleHexCharToBin(hexStr[0]) << 4 | singleHexCharToBin(hexStr[1])) << 8) | (singleHexCharToBin(hexStr[2]) << 4 | singleHexCharToBin(hexStr[3]));
	}
}

byte singleHexCharToBin(const char hexChar) {
	switch (hexChar) {
	case '0':
		return 0b0000;
		break;
	case '1':
		return 0b0001;
		break;
	case '2':
		return 0b0010;
		break;
	case '3':
		return 0b0011;
		break;
	case '4':
		return 0b0100;
		break;
	case '5':
		return 0b0101;
		break;
	case '6':
		return 0b0110;
		break;
	case '7':
		return 0b0111;
		break;
	case '8':
		return 0b1000;
		break;
	case '9':
		return 0b1001;
		break;
	case 'A':
		return 0b1010;
		break;
	case 'B':
		return 0b1011;
		break;
	case 'C':
		return 0b1100;
		break;
	case 'D':
		return 0b1101;
		break;
	case 'E':
		return 0b1110;
		break;
	case 'F':
		return 0b1111;
		break;
	default:
		return 0;
		break;
	}
}

bool singleHexCharToBin_UnitTest() {
	const char test_values[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	for (uint8_t i = 0; i < 16; i++) {
		if (singleHexCharToBin(test_values[i]) != i) {
			return false;
		}
	}
	return true;
}

bool hexToByte_UnitTest() {
	const char test_values[5][3] = {
		{'0','C','\0'},
		{'1','0','\0'},
		{'F','F','\0'},
		{'4','6','\0'},
		{'9','4','\0'}
	};
	const byte test_answers[5] = {0x0C, 0x10, 0xFF, 0x46, 0x94};

	for (uint8_t i = 0; i < 5; i++) {
		if (hexToByte(test_values[i]) != test_answers[i]) {
			return false;
		}
	}
	return true;
}

bool hexToWord_UnitTest() {
	const char test_values[5][5] = {
		{'0', 'C', '9', '4', '\0'},
		{'3', '4', '0', '0', '\0'},
		{'8', 'B', 'B', '9', '\0'},
		{'F', 'F', 'F', 'F', '\0'},
		{'0', '0', 'E', '0', '\0'}
	};
	const word test_answers[5] = { 0x940C, 0x0034, 0xB98B, 0xFFFF, 0xE000 };

	for (uint8_t i = 0; i < 5; i++) {
		if (hexToWord(test_values[i]) != test_answers[i]) {
			return false;
		}
	}
	return true;
}

void run_unit_tests() {
	bool (*unit_tests[3])(void) = {singleHexCharToBin_UnitTest, hexToByte_UnitTest, hexToWord_UnitTest};
	for (int i = 0; i < 3; i++) {
		if (unit_tests[i]()) {
			printf("Unit test %d: Passed\n", i);
		}
		else {
			printf("Unit test %d: Failed\n", i);
		}
	}
}
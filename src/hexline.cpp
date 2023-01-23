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

HexLine::HexLine(FILE* fp) {
	while (fgetc(fp) != ':') {
		if (feof(fp)) {
			this->byteCount = 0;
			this->address = 0;
			this->recordType = 0;
			//this->dataHead = 0; //¿How did this compiled in the first place?
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
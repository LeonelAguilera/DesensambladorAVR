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
			this->next = new DataLinkedList(*base.next);
		}
		else {
			this->next = 0;
		}
	};
	DataLinkedList getNext() { return *this->next; };

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

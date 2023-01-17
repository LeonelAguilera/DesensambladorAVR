#ifndef __HEXLINE_H__
#define __HEXLINE_H__

#include "dataLinkedList.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

class HexLine {
private:
	byte byteCount;
	word address;
	byte recordType;
	DataLinkedList dataHead;
	byte checksum;

public:
	HexLine* nextLine;

	//HexLine();
	HexLine(FILE* fp);
	~HexLine();

	byte getByteCount();
	word getAddress();
	byte getRecordType();
	DataLinkedList getDataHead();
	byte getChecksum();

	void printData();
	bool isEOF();
};
#endif // !__HEXLINE_H__

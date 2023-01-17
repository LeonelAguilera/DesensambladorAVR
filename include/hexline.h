#ifndef __HEXLINE_H__
#define __HEXLINE_H__
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef uint8_t byte;
typedef uint16_t word;

class HexLine {
public:
	byte byteCount;
	word address;
	byte recordType;
	word* data;
	byte checksum;
public:
	HexLine(FILE* fp);

	void printData();
};

byte hexToByte(const char* hexStr);
word hexToWord(const char* hexStr);
byte singleHexCharToBin(const char hexChar);

bool singleHexCharToBin_UnitTest();
bool hexToByte_UnitTest();
bool hexToWord_UnitTest();

void run_unit_tests();
#endif // !__HEXLINE_H__

#ifndef __HEXLINE_H__
#define __HEXLINE_H__
#include <stdint.h>


typedef uint8_t byte;
typedef uint16_t word;

class HexLine {
public:
	byte byteCount;
	word address;
	byte recordType;
	word* data;
	byte checksum;
};

byte hexToByte(const char* hexStr);
word hexToWord(const char* hexStr);
byte singleHexCharToBin(const char hexChar);

bool singleHexCharToBin_UnitTest();
bool hexToByte_UnitTest();
bool hexToWord_UnitTest();
#endif // !__HEXLINE_H__

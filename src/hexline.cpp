#include "hexline.h"

byte hexToByte(const char* hexStr) {
	if (hexStr[2] != '\0') {
		return 0; //Error
	}

	return singleHexCharToBin(hexStr[0]) << 4 | singleHexCharToBin(hexStr[1]);
}
word hexToWord(const char* hexStr) {
	if (hexStr[4] != '\0') {
		return 0; //Error
	}

	return ((singleHexCharToBin(hexStr[2]) << 4 | singleHexCharToBin(hexStr[3]))<<8) | (singleHexCharToBin(hexStr[0]) << 4 | singleHexCharToBin(hexStr[1]));
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

bool hexToByte_UnitTest(const char* hexStr) {
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
}

bool hexToWord_UnitTest(const char* hexStr) {
	const char test_values[5][5] = {
		{'0', 'C', '9', '4', '\0'},
		{'3', '4', '0', '0', '\0'},
		{'8', 'B', 'B', '9', '\0'},
		{'F', 'F', 'F', 'F', '\0'},
		{'0', '0', 'E', '0', '\0'}
	};
	const word test_answers[5] = { 0x940C, 0x0034, 0xB98B, 0xFFFF, 0xE000 };

	for (uint8_t i = 0; i < 5; i++) {
		if (hexToByte(test_values[i]) != test_answers[i]) {
			return false;
		}
	}
}
//Copyright 2023 Leonel Alejandro Aguilera Correia
/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public
License along with this program.
If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "hexFileDataExtractor.h"
#include "dataLinkedList.h"
#include "instruction.h"
#include "outputCode.h"

#define DEBUG

typedef enum{SYSTEM_ERROR = -2, USER_ERROR, HEXTOASM, ASMTOHEX, HELP} INPUT_TYPE;

INPUT_TYPE whatToDo(int argc, char* argv[]);

void HandleSystemError();
void HandleUserError(int argc, char* argv[]);
bool HandleHexToAsmConversion(const char* hexFileName, const char* asmFileName);
bool HandleAsmToHexConversion(const char* asmFileName, const char* hexFileName);
void HandleHelp();

DataLinkedList getHexCodeFromNamedFile(const char* fileName);
bool writeASMFileFromHexcode(DataLinkedList hexCode, const char* destinationFile);

void getFileFormat(const char* fileName, char* fileFormat);

#ifdef _DEBUG
int main() {
	const char* firstParameter = "DesensambladorAVR";
	const char* secondParameter = "Blink.hex";
	const char* thirdParameter = "file.asm";

	int argc = 3;
	char* argv[3]{};

	argv[0] = (char*)malloc((strlen(firstParameter) + 1) * sizeof(char));
	argv[1] = (char*)malloc((strlen(secondParameter) + 1) * sizeof(char));
	argv[2] = (char*)malloc((strlen("file.asm") + 1) * sizeof(char));

	if (argv[0] == 0 || argv[1] == 0 || argv[2] == 0)
		return -1;

	strncpy_s(argv[0], strlen(firstParameter) + 1, firstParameter, strlen(firstParameter));
	strncpy_s(argv[1], strlen(secondParameter) + 1, secondParameter, strlen(secondParameter));
	strncpy_s(argv[2], strlen(thirdParameter) + 1, thirdParameter, strlen(thirdParameter));
#else
int main(int argc, char* argv[]) {
#endif // _DEBUG


	INPUT_TYPE inputType = whatToDo(argc, argv);

	switch (inputType)
	{
	case SYSTEM_ERROR:
		HandleSystemError();
		return -1;
		break;
	case USER_ERROR:
		HandleUserError(argc, argv);
		break;
	case HEXTOASM:
		if (HandleHexToAsmConversion(argv[1], argv[2])) {
			return 0;
		}
		else {
			HandleSystemError();
			return -1;
		}
		break;
	case ASMTOHEX:
		if (HandleAsmToHexConversion(argv[1], argv[2])) {
			return 0;
		}
		else {
			HandleSystemError();
			return -1;
		}
		break;
	case HELP:
		HandleHelp();
		break;
	default:
		HandleSystemError();
		return -1;
		break;
	}
	return 0;
}

INPUT_TYPE whatToDo(int argc, char* argv[]) {
	switch (argc) {
	case 1: {
		return HELP;
		break;
	}
	case 2: {
		bool isHelp = strcmp(argv[1], "help") == 0;
		if (isHelp) {
			return HELP;
		}
		else {
			return USER_ERROR;
		}
		break;
	}
	case 3: {
		char* fileFormat = (char*)malloc(5);
		if (fileFormat == 0) {
			return SYSTEM_ERROR;
		}
		getFileFormat(argv[1], fileFormat);

		bool isHex = strcmp(fileFormat, ".hex") == 0;
		bool isAsm = strcmp(fileFormat, ".asm") == 0;
		if (isHex) {
			getFileFormat(argv[2], fileFormat);
			isAsm = strcmp(fileFormat, ".asm") == 0;
			free(fileFormat);
			if (isAsm) {
				return HEXTOASM;
			}
			else {
				return USER_ERROR;
			}
		}
		else if (isAsm) {
			getFileFormat(argv[2], fileFormat);
			isHex = strcmp(fileFormat, ".hex") == 0;
			free(fileFormat);
			if (isHex) {
				return ASMTOHEX;
			}
			else {
				return USER_ERROR;
			}
		}
		else {
			free(fileFormat);
			return USER_ERROR;
		}
		break;
	}
	default:
		return USER_ERROR;
	}
}

void HandleSystemError() {
	perror("Something went wrong, please free some RAM and try again");
}
void HandleUserError(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Program expected 2 inputs, but you introduced: %d", argc - 1);
	}
	else {
		char* FirstfileFormat = (char*)malloc(5);
		char* SecondfileFormat = (char*)malloc(5);
		if (FirstfileFormat == 0 || SecondfileFormat == 0) {
			HandleSystemError();
			return;
		}
		getFileFormat(argv[1], FirstfileFormat);
		getFileFormat(argv[2], SecondfileFormat);
		printf("Expected a .hex file and a .asm file, but instead got a %s and a %s", FirstfileFormat, SecondfileFormat);
	}
}
bool HandleHexToAsmConversion(const char* hexFileName, const char* asmFileName) {
	DataLinkedList hexCode = getHexCodeFromNamedFile(hexFileName);

	return writeASMFileFromHexcode(hexCode, asmFileName);
}
bool HandleAsmToHexConversion(const char* asmFileName, const char* hexFileName) {
	printf("This program only disassembles, it cannot be used to assemble");
	return true;
}
void HandleHelp() {
	printf("This code is an AVR disassembler\nTo use it input the following comand line:\n\n\t$:DesensambladorAVR \"[sourceFile_Name].hex\" \"[destinationFile_Name].asm\"");
}


DataLinkedList getHexCodeFromNamedFile(const char* fileName) {
	FILE* hexFile;
	DataLinkedList hexCode;

	fopen_s(&hexFile, fileName, "r");
	if (hexFile == 0) {
		perror("Hex file opening failed");
		exit(-1);
	}

	hexCode = getDataFromHex(hexFile);

	fclose(hexFile);

	return hexCode;
}

bool writeASMFileFromHexcode(DataLinkedList hexCode, const char* destinationFile) {
	FILE* asmFile;

	fopen_s(&asmFile, destinationFile, "w");
	if (asmFile == 0) {
		perror("ASM file creation failed");
		return false;
	}

	Instruction* listaDeInstrucciones[NUM_INSTRUCCIONES];
	inicializador(listaDeInstrucciones);

	OutputCode outputCode;

	while (hexCode.next != 0) {
		char ASMLine[INSTRUCTION_MAX_LENGTH]{};
		bool failedFlag = true;
		for (int i = 0; i < NUM_INSTRUCCIONES; i++) {
			if (listaDeInstrucciones[i]->codeLine(&hexCode, ASMLine)) {
				//puts(ASMLine);
				//fputs(ASMLine, asmFile);
				//outputCode.addLine(ASMLine);
				failedFlag = false;
				break;
			}
		}
		if (failedFlag) {
			Instruction::handleUnknownInstruction(&hexCode, ASMLine);
			//outputCode.addLine(ASMLine);
			//fputs(ASMLine, asmFile);
			//perror("Instruccion no reconocida");
		}
		outputCode.addLine(ASMLine);
	}

	fputs(outputCode.turnIntoString(), asmFile);

	fclose(asmFile);

	return true;
}

void getFileFormat(const char* fileName, char* fileFormat) {
	strncpy_s(fileFormat, 5, fileName + strlen(fileName) - 4, 4);
}
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

#include "hexFileDataExtractor.h"
#include "dataLinkedList.h"
#include "instruction.h"

int main() {
	FILE* hexFile;

	fopen_s(&hexFile, "code.hex", "r");
	if (hexFile == 0) {
		perror("Hex file opening failed");
		return 0;
	}

	DataLinkedList hexCode = getDataFromHex(hexFile);

	fclose(hexFile);

	FILE* asmFile;

	fopen_s(&asmFile, "asmFile.asm", "w");
	if (asmFile == 0) {
		perror("ASM file creation failed");
		return 0;
	}

	Instruction* listaDeInstrucciones[NUM_INSTRUCCIONES];
	inicializador(listaDeInstrucciones);
	
	while (hexCode.next != 0) {
		char ASMCode[INSTRUCTION_MAX_LENGTH]{};
		bool failedFlag = true;
		for (int i = 0; i < NUM_INSTRUCCIONES; i++) {
			if (listaDeInstrucciones[i]->codeLine(&hexCode, ASMCode)) {
				//puts(ASMCode);
				fputs(ASMCode, asmFile);
				failedFlag = false;
				break;
			}
		}
		if (failedFlag) {
			perror("Instruccion no reconocida");
		}
	}

	fclose(asmFile);

	return 0;
}
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

#include "instruction.h"
#include <malloc.h>

bool Instruction::isThisInstruction(word OPcode) {
	return (OPcode & this->_mask) == this->_OPcode;
}

bool Instruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte Rr = OPCode->data & 0x000F | (OPCode->data & 0x0200) >> 5;
	byte Rd = (OPCode->data >> 4) & 0x001F;
	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d,r%d\n", this->_mnemonic, Rd, Rr);

	if (OPCode->next != 0) {
		DataLinkedList* temp = OPCode->next;
		*OPCode = OPCode->getNext();
		free(temp);
	}

	//OPCode = OPCode->next;
	return true;
}

bool BranchInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data) || this->_man == 13 || this->_man == 14) {
		return 0;
	}

	word k = OPCode->data & !this->_mask;
	if (OPCode->data & 0x200) {
		k = k >> 3;
	}
	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d\n", this->_mnemonic, k); //Pendiente controlar signos negativos
	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool InmediateWordInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte K = OPCode->data & 0x000F | (OPCode->data & 0x00C0)>>2;
	byte d = (OPCode->data & 0x0030) >> 4;
	d = 24 + 2 * d;
	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d:r%d,%d\n", this->_mnemonic, d+1,d,K); //Pendiente controlar signos negativos

	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool InmediateByteInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte K = OPCode->data & 0x000F | (OPCode->data & 0x0F00) >> 4;
	byte d = (OPCode->data & 0x00F0) >> 4;
	d += 16;
	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, %d\n", this->_mnemonic, d, K); //Pendiente controlar signos negativos
	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool SingleRegisterInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte d = (OPCode->data & 0x01F0) >> 4;
	if ((this->_mask & 0x0001) == 0) { //Se trata de BLD o BST
		byte b = (OPCode->data & 0x0007);
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d,%d\n", this->_mnemonic, d, b);
	}
	else {
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d\n", this->_mnemonic, d);
	}
	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool LoadInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte d = (OPCode->data & 0x01F0) >> 4;
	byte q = (OPCode->data & 0x0007) >> 0 | (OPCode->data & 0x0C00) >> 7 | (OPCode->data & 0x2000) >> 8;

	switch (this->_OPcode) {
	case 0b1001000000000110:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z\n", this->_mnemonic, d);
		break;
	case 0b1001000000000111:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z+\n", this->_mnemonic, d);
		break;
	case 0b1001000000001100:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, X\n", this->_mnemonic, d);
		break;
	case 0b1001000000001101:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, X+\n", this->_mnemonic, d);
		break;
	case 0b1001000000001110:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, -X\n", this->_mnemonic, d);
		break;
	case 0b1000000000001000:
		if (q == 0) {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Y\n", this->_mnemonic, d);
		}
		else {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Y+%d\n", this->_mnemonic, d, q);
		}
		break;
	case 0b1001000000001001:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Y+\n", this->_mnemonic, d);
		break;
	case 0b1001000000001010:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, -Y\n", this->_mnemonic, d);
		break;
	case 0b1000000000000000:
		if (q == 0) {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z\n", this->_mnemonic, d);
		}
		else {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z+%d\n", this->_mnemonic, d, q);
		}
		break;
	case 0b1001000000000001:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z+\n", this->_mnemonic, d);
		break;
	case 0b1001000000000010:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, -Z\n", this->_mnemonic, d);
		break;
	case 0b1001000000000100:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z\n", this->_mnemonic, d);
		break;
	case 0b1001000000000101:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z+\n", this->_mnemonic, d);
		break;
	default:
		return 0;
	}
	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool StoreInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte d = (OPCode->data & 0x01F0) >> 4;
	byte q = (OPCode->data & 0x0007) >> 0 | (OPCode->data & 0x0C00) >> 7 | (OPCode->data & 0x2000) >> 8;

	switch (this->_OPcode) {
	case 0b1001001000000110:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z, r%d\n", this->_mnemonic, d);
		break;
	case 0b1001001000000101:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z, r%d\n", this->_mnemonic, d);
		break;
	case 0b1001001000000111:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z, r%d\n", this->_mnemonic, d);
		break;
	case 0b1111110000000000:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, %d\n", this->_mnemonic, d, q & 0x07);
		break;
	case 0b1111111000000000:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, %d\n", this->_mnemonic, d, q & 0x07);
		break;
	case 0b1001001000001100:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s X, r%d\n", this->_mnemonic, d);
		break;
	case 0b1001001000001101:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s X+, r%d\n", this->_mnemonic, d);
		break;
	case 0b1001001000001110:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s -X, r%d\n", this->_mnemonic, d);
		break;
	case 0b1000001000001000:
		if (q == 0) {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Y, r%d\n", this->_mnemonic, d);
		}
		else {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Y+%d, r%d\n", this->_mnemonic, d, q);
		}
		break;
	case 0b1001001000001001:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Y+, r%d\n", this->_mnemonic, d);
		break;
	case 0b1001001000001010:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s -Y, r%d\n", this->_mnemonic, d);
		break;
	case 0b1000001000000000:
		if (q == 0) {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z, r%d\n", this->_mnemonic, d);
		}
		else {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z+%d, r%d\n", this->_mnemonic, d, q);
		}
		break;
	case 0b1001001000000001:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z+, r%d\n", this->_mnemonic, d);
		break;
	case 0b1001001000000010:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s -Z, r%d\n", this->_mnemonic, d);
		break;
	case 0b1001001000000100:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z, r%d\n", this->_mnemonic, d);
		break;
	default:
		return 0;
	}
	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool SREGInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte s = (OPCode->data >> 4) & 0x0007;
	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s %d\n", this->_mnemonic, s);
	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool NoParameterInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s\n", this->_mnemonic);
	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool SingleBitInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte A = (OPCode->data >> 3) & 0x001F;
	byte b = (OPCode->data >> 0) & 0x0007;

	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s %d, %d\n", this->_mnemonic, A, b);
	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool DoubleSingleRegisterInstruction::isThisInstruction(word OPcode) {
	byte Rr = OPcode & 0x000F | (OPcode & 0x0200) >> 5;
	byte Rd = (OPcode >> 4) & 0x001F;
	return (Rd == Rr) && ((OPcode & this->_mask) == this->_OPcode);
}

bool DoubleSingleRegisterInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte d = (OPCode->data >> 4) & 0x001F;
	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d\n", this->_mnemonic, d);
	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool FMULInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte d = (OPCode->data >> 4) & 0x0007;
	byte r = (OPCode->data >> 0) & 0x0007;

	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, r%d\n", this->_mnemonic, d, r);
	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool IOInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte A = (OPCode->data & 0x0600) >> 5 | (OPCode->data & 0x000F);
	byte dr = (OPCode->data >> 4) & 0x001F;

	if (this->_OPcode & 0x0800) {
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s %d, r%d\n", this->_mnemonic, A, dr);
	}
	else {
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, %d\n", this->_mnemonic, dr, A);
	}
	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool ExtraInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte d = (OPCode->data >> 4) & 0x000F;
	byte k = ((OPCode->data >> 0) & 0x000F) | ((OPCode->data >> 4) & 0x0070);

	switch (this->_OPcode) {
	case 0b1001010000001011:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s 0x%04X\n", this->_mnemonic, d);
		break;
	case 0b1010000000000000:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, %d\n", this->_mnemonic, d + 16, k);
		break;
	case 0b0000000100000000:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d:%d, r%d:%d\n", this->_mnemonic, (d*2)+1, d*2, ((k&0x0F)*2)+1, (k & 0x0F) * 2);
		break;
	case 0b0000001000000000:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, r%d\n", this->_mnemonic, d + 16, (k & 0x0F) + 16);
		break;
	case 0b1010100000000000:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s %d, r%d\n", this->_mnemonic, k, d + 16);
		break;
	default:
		return 0;
		break;
	}
	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

bool ThirtyTwoBitsInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return 0;
	}

	byte k = ((OPCode->data >> 0) & 0x000F) | ((OPCode->data >> 4) & 0x0070);

	if (this->_mask & 0x0001) {
		byte d = (OPCode->data >> 4) & 0x001F;
		DataLinkedList* temp = OPCode->next;
		*OPCode = OPCode->getNext();
		free(temp);

		//OPCode = OPCode->next;
		word k = OPCode->data;
		if (this->_OPcode & 0x0200) { //STS
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s %d, r%d\n", this->_mnemonic, k, d);
		}
		else {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, %d\n", this->_mnemonic, d, k);
		}
	}
	else {
		uint32_t k = (OPCode->data & 0x0001) | ((OPCode->data & 0x01F0) >> 3);
		k = k << 16;
		DataLinkedList* temp = OPCode->next;
		*OPCode = OPCode->getNext();
		free(temp);

		//OPCode = OPCode->next;
		k |= OPCode->data;

		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s 0x%06X\n", this->_mnemonic, k);
	}

	DataLinkedList* temp = OPCode->next;
	*OPCode = OPCode->getNext();
	free(temp);

	//OPCode = OPCode->next;
	return true;
}

void inicializador(Instruction** ListaDe144Instrucciones) {
	ListaDe144Instrucciones[0] = new ThirtyTwoBitsInstruction(0b1001001000000000, 0b1111111000001111, "STS", 121);
	ListaDe144Instrucciones[1] = new ThirtyTwoBitsInstruction(0b1001000000000000, 0b1111111000001111, "LDS", 74);
	ListaDe144Instrucciones[2] = new ThirtyTwoBitsInstruction(0b1001010000001100, 0b1111111000001110, "JMP", 66);
	ListaDe144Instrucciones[3] = new ThirtyTwoBitsInstruction(0b1001010000001110, 0b1111111000001110, "CALL", 36);
	ListaDe144Instrucciones[4] = new ExtraInstruction(0b1010100000000000, 0b1111100000000000, "STS", 122);
	ListaDe144Instrucciones[5] = new ExtraInstruction(0b0000001000000000, 0b1111111100000000, "MULS", 82);
	ListaDe144Instrucciones[6] = new ExtraInstruction(0b0000000100000000, 0b1111111100000000, "MOVW", 80);
	ListaDe144Instrucciones[7] = new ExtraInstruction(0b1010000000000000, 0b1111100000000000, "LDS", 75);
	ListaDe144Instrucciones[8] = new ExtraInstruction(0b1001010000001011, 0b1111111100001111, "DES", 54);
	ListaDe144Instrucciones[9] = new IOInstruction(0b1011100000000000, 0b1111100000000000, "OUT", 88);
	ListaDe144Instrucciones[10] = new IOInstruction(0b1011000000000000, 0b1111100000000000, "IN", 64);
	ListaDe144Instrucciones[11] = new FMULInstruction(0b0000001100000000, 0b1111111110001000, "MULSU", 83);
	ListaDe144Instrucciones[12] = new FMULInstruction(0b0000001110001000, 0b1111111110001000, "FMULSU", 61);
	ListaDe144Instrucciones[13] = new FMULInstruction(0b0000001110001000, 0b1111111110001000, "FMULS", 60);
	ListaDe144Instrucciones[14] = new FMULInstruction(0b0000001100001000, 0b1111111110001000, "FMUL", 59);
	ListaDe144Instrucciones[15] = new DoubleSingleRegisterInstruction(0b0010000000000000, 0b1111110000000000, "TST", 126);
	ListaDe144Instrucciones[16] = new DoubleSingleRegisterInstruction(0b0001110000000000, 0b1111110000000000, "ROL", 95);
	ListaDe144Instrucciones[17] = new DoubleSingleRegisterInstruction(0b0000110000000000, 0b1111110000000000, "LSL", 77);
	ListaDe144Instrucciones[18] = new DoubleSingleRegisterInstruction(0b0010010000000000, 0b1111110000000000, "CLR", 43);
	ListaDe144Instrucciones[19] = new SingleBitInstruction(0b1001101100000000, 0b1111111100000000, "SBIS", 101);
	ListaDe144Instrucciones[20] = new SingleBitInstruction(0b1001100100000000, 0b1111111100000000, "SBIC", 100);
	ListaDe144Instrucciones[21] = new SingleBitInstruction(0b1001101000000000, 0b1111111100000000, "SBI", 99);
	ListaDe144Instrucciones[22] = new SingleBitInstruction(0b1001100000000000, 0b1111111100000000, "CBI", 37);
	ListaDe144Instrucciones[23] = new NoParameterInstruction(0b1001010110101000, 0b1111111111111111, "WDR", 127);
	ListaDe144Instrucciones[24] = new NoParameterInstruction(0b1001010111111000, 0b1111111111111111, "SPM", 117);
	ListaDe144Instrucciones[25] = new NoParameterInstruction(0b1001010111101000, 0b1111111111111111, "SPM", 117);
	ListaDe144Instrucciones[26] = new NoParameterInstruction(0b1001010111101000, 0b1111111111111111, "SPM", 116);
	ListaDe144Instrucciones[27] = new NoParameterInstruction(0b1001010110001000, 0b1111111111111111, "SLEEP", 115);
	ListaDe144Instrucciones[28] = new NoParameterInstruction(0b1001010000011000, 0b1111111111111111, "SEZ", 114);
	ListaDe144Instrucciones[29] = new NoParameterInstruction(0b1001010000111000, 0b1111111111111111, "SEV", 113);
	ListaDe144Instrucciones[30] = new NoParameterInstruction(0b1001010001101000, 0b1111111111111111, "SET", 112);
	ListaDe144Instrucciones[31] = new NoParameterInstruction(0b1001010001001000, 0b1111111111111111, "SES", 111);
	ListaDe144Instrucciones[32] = new NoParameterInstruction(0b1001010000101000, 0b1111111111111111, "SEN", 109);
	ListaDe144Instrucciones[33] = new NoParameterInstruction(0b1001010001111000, 0b1111111111111111, "SEI", 108);
	ListaDe144Instrucciones[34] = new NoParameterInstruction(0b1001010001011000, 0b1111111111111111, "SEH", 107);
	ListaDe144Instrucciones[35] = new NoParameterInstruction(0b1001010000001000, 0b1111111111111111, "SEC", 106);
	ListaDe144Instrucciones[36] = new NoParameterInstruction(0b1001010100011000, 0b1111111111111111, "RETI", 93);
	ListaDe144Instrucciones[37] = new NoParameterInstruction(0b1001010100001000, 0b1111111111111111, "RET", 92);
	ListaDe144Instrucciones[38] = new NoParameterInstruction(0b0000000000000000, 0b1111111111111111, "NOP", 85);
	ListaDe144Instrucciones[39] = new NoParameterInstruction(0b1001010111001000, 0b1111111111111111, "LPM", 76);
	ListaDe144Instrucciones[40] = new NoParameterInstruction(0b1001010000001001, 0b1111111111111111, "IJMP", 63);
	ListaDe144Instrucciones[41] = new NoParameterInstruction(0b1001010100001001, 0b1111111111111111, "ICALL", 62);
	ListaDe144Instrucciones[42] = new NoParameterInstruction(0b1001010111011000, 0b1111111111111111, "ELPM", 57);
	ListaDe144Instrucciones[43] = new NoParameterInstruction(0b1001010000011001, 0b1111111111111111, "EIJMP", 56);
	ListaDe144Instrucciones[44] = new NoParameterInstruction(0b1001010100011001, 0b1111111111111111, "EICALL", 55);
	ListaDe144Instrucciones[45] = new NoParameterInstruction(0b1001010010011000, 0b1111111111111111, "CLZ", 47);
	ListaDe144Instrucciones[46] = new NoParameterInstruction(0b1001010010111000, 0b1111111111111111, "CLV", 46);
	ListaDe144Instrucciones[47] = new NoParameterInstruction(0b1001010011101000, 0b1111111111111111, "CLT", 45);
	ListaDe144Instrucciones[48] = new NoParameterInstruction(0b1001010011001000, 0b1111111111111111, "CLS", 44);
	ListaDe144Instrucciones[49] = new NoParameterInstruction(0b1001010010101000, 0b1111111111111111, "CLN", 42);
	ListaDe144Instrucciones[50] = new NoParameterInstruction(0b1001010011111000, 0b1111111111111111, "CLI", 41);
	ListaDe144Instrucciones[51] = new NoParameterInstruction(0b1001010011011000, 0b1111111111111111, "CLH", 40);
	ListaDe144Instrucciones[52] = new NoParameterInstruction(0b1001010010001000, 0b1111111111111111, "CLC", 39);
	ListaDe144Instrucciones[53] = new NoParameterInstruction(0b1001010110011000, 0b1111111111111111, "BREAK", 17);
	ListaDe144Instrucciones[54] = new SREGInstruction(0b1001010000001000, 0b1111111110001111, "BSET", 34);
	ListaDe144Instrucciones[55] = new SREGInstruction(0b1001010010001000, 0b1111111110001111, "BCLR", 11);
	ListaDe144Instrucciones[56] = new StoreInstruction(0b1001001000000100, 0b1111111000001111, "XCH", 128);
	ListaDe144Instrucciones[57] = new StoreInstruction(0b1000001000000000, 0b1101001000001000, "STD", 120);
	ListaDe144Instrucciones[58] = new StoreInstruction(0b1001001000000010, 0b1111111000001111, "ST", 120);
	ListaDe144Instrucciones[59] = new StoreInstruction(0b1001001000000001, 0b1111111000001111, "ST", 120);
	ListaDe144Instrucciones[60] = new StoreInstruction(0b1000001000000000, 0b1111111000001111, "ST", 120);
	ListaDe144Instrucciones[61] = new StoreInstruction(0b1000001000001000, 0b1101001000001000, "STD", 119);
	ListaDe144Instrucciones[62] = new StoreInstruction(0b1001001000001010, 0b1111111000001111, "ST", 119);
	ListaDe144Instrucciones[63] = new StoreInstruction(0b1001001000001001, 0b1111111000001111, "ST", 119);
	ListaDe144Instrucciones[64] = new StoreInstruction(0b1000001000001000, 0b1111111000001111, "ST", 119);
	ListaDe144Instrucciones[65] = new StoreInstruction(0b1001001000001110, 0b1111111000001111, "ST", 118);
	ListaDe144Instrucciones[66] = new StoreInstruction(0b1001001000001101, 0b1111111000001111, "ST", 118);
	ListaDe144Instrucciones[67] = new StoreInstruction(0b1001001000001100, 0b1111111000001111, "ST", 118);
	ListaDe144Instrucciones[68] = new StoreInstruction(0b1111111000000000, 0b1111111000001000, "SBRS", 105);
	ListaDe144Instrucciones[69] = new StoreInstruction(0b1111110000000000, 0b1111111000001000, "SBRC", 104);
	ListaDe144Instrucciones[70] = new StoreInstruction(0b1001001000000111, 0b1111111000001111, "LAT", 69);
	ListaDe144Instrucciones[71] = new StoreInstruction(0b1001001000000101, 0b1111111000001111, "LAS", 68);
	ListaDe144Instrucciones[72] = new StoreInstruction(0b1001001000000110, 0b1111111000001111, "LAC", 67);
	ListaDe144Instrucciones[73] = new LoadInstruction(0b1001000000000101, 0b1111111000001111, "LPM", 76);
	ListaDe144Instrucciones[74] = new LoadInstruction(0b1001000000000100, 0b1111111000001111, "LPM", 76);
	ListaDe144Instrucciones[75] = new LoadInstruction(0b1000000000000000, 0b1101001000001000, "LDD", 72);
	ListaDe144Instrucciones[76] = new LoadInstruction(0b1001000000000010, 0b1111111000001111, "LD", 72);
	ListaDe144Instrucciones[77] = new LoadInstruction(0b1001000000000001, 0b1111111000001111, "LD", 72);
	ListaDe144Instrucciones[78] = new LoadInstruction(0b1000000000000000, 0b1111111000001111, "LD", 72);
	ListaDe144Instrucciones[79] = new LoadInstruction(0b1000000000001000, 0b1101001000001000, "LDD", 71);
	ListaDe144Instrucciones[80] = new LoadInstruction(0b1001000000001010, 0b1111111000001111, "LD", 71);
	ListaDe144Instrucciones[81] = new LoadInstruction(0b1001000000001001, 0b1111111000001111, "LD", 71);
	ListaDe144Instrucciones[82] = new LoadInstruction(0b1000000000001000, 0b1111111000001111, "LD", 71);
	ListaDe144Instrucciones[83] = new LoadInstruction(0b1001000000001110, 0b1111111000001111, "LD", 70);
	ListaDe144Instrucciones[84] = new LoadInstruction(0b1001000000001101, 0b1111111000001111, "LD", 70);
	ListaDe144Instrucciones[85] = new LoadInstruction(0b1001000000001100, 0b1111111000001111, "LD", 70);
	ListaDe144Instrucciones[86] = new LoadInstruction(0b1001000000000111, 0b1111111000001111, "ELPM", 57);
	ListaDe144Instrucciones[87] = new LoadInstruction(0b1001000000000110, 0b1111111000001111, "ELPM", 57);
	ListaDe144Instrucciones[88] = new SingleRegisterInstruction(0b1001010000000010, 0b1111111000001111, "SWAP", 125);
	ListaDe144Instrucciones[89] = new SingleRegisterInstruction(0b1110111100001111, 0b1111111100001111, "SER", 110);
	ListaDe144Instrucciones[90] = new SingleRegisterInstruction(0b1001010000000111, 0b1111111000001111, "ROR", 96);
	ListaDe144Instrucciones[91] = new SingleRegisterInstruction(0b1001001000001111, 0b1111111000001111, "PUSH", 90);
	ListaDe144Instrucciones[92] = new SingleRegisterInstruction(0b1001000000001111, 0b1111111000001111, "POP", 89);
	ListaDe144Instrucciones[93] = new SingleRegisterInstruction(0b1001010000000001, 0b1111111000001111, "NEG", 84);
	ListaDe144Instrucciones[94] = new SingleRegisterInstruction(0b1001010000000110, 0b1111111000001111, "LSR", 78);
	ListaDe144Instrucciones[95] = new SingleRegisterInstruction(0b1001010000000011, 0b1111111000001111, "INC", 65);
	ListaDe144Instrucciones[96] = new SingleRegisterInstruction(0b1001010000001010, 0b1111111000001111, "DEC", 53);
	ListaDe144Instrucciones[97] = new SingleRegisterInstruction(0b1001010000000000, 0b1111111000001111, "COM", 48);
	ListaDe144Instrucciones[98] = new SingleRegisterInstruction(0b1111101000000000, 0b1111111000001000, "BST", 35);
	ListaDe144Instrucciones[99] = new SingleRegisterInstruction(0b1111100000000000, 0b1111111000001000, "BLD", 12);
	ListaDe144Instrucciones[100] = new SingleRegisterInstruction(0b1001010000000101, 0b1111111000001111, "ASR", 10);
	ListaDe144Instrucciones[101] = new InmediateByteInstruction(0b0101000000000000, 0b1111000000000000, "SUBI", 124);
	ListaDe144Instrucciones[102] = new InmediateByteInstruction(0b0110000000000000, 0b1111000000000000, "SBR", 103);
	ListaDe144Instrucciones[103] = new InmediateByteInstruction(0b0100000000000000, 0b1111000000000000, "SBCI", 98);
	ListaDe144Instrucciones[104] = new InmediateByteInstruction(0b0110000000000000, 0b1111000000000000, "ORI", 87);
	ListaDe144Instrucciones[105] = new InmediateByteInstruction(0b1110000000000000, 0b1111000000000000, "LDI", 73);
	ListaDe144Instrucciones[106] = new InmediateByteInstruction(0b0011000000000000, 0b1111000000000000, "CPI", 51);
	ListaDe144Instrucciones[107] = new InmediateByteInstruction(0b0111000000000000, 0b1111000000000000, "ANDI", 9);
	ListaDe144Instrucciones[108] = new InmediateWordInstruction(0b1001011100000000, 0b1111111100000000, "SBIW", 102);
	ListaDe144Instrucciones[109] = new InmediateWordInstruction(0b1001011000000000, 0b1111111100000000, "ADIW", 7);
	ListaDe144Instrucciones[110] = new BranchInstruction(0b1100000000000000, 0b1111000000000000, "RJMP", 94);
	ListaDe144Instrucciones[111] = new BranchInstruction(0b1101000000000000, 0b1111000000000000, "RCALL", 91);
	ListaDe144Instrucciones[112] = new BranchInstruction(0b1111000000000011, 0b1111110000000111, "BRVS", 33);
	ListaDe144Instrucciones[113] = new BranchInstruction(0b1111010000000011, 0b1111110000000111, "BRVC", 32);
	ListaDe144Instrucciones[114] = new BranchInstruction(0b1111000000000110, 0b1111110000000111, "BRTS", 31);
	ListaDe144Instrucciones[115] = new BranchInstruction(0b1111010000000110, 0b1111110000000111, "BRTC", 30);
	ListaDe144Instrucciones[116] = new BranchInstruction(0b1111010000000000, 0b1111110000000111, "BRSH", 29);
	ListaDe144Instrucciones[117] = new BranchInstruction(0b1111010000000010, 0b1111110000000111, "BRPL", 28);
	ListaDe144Instrucciones[118] = new BranchInstruction(0b1111010000000001, 0b1111110000000111, "BRNE", 27);
	ListaDe144Instrucciones[119] = new BranchInstruction(0b1111000000000010, 0b1111110000000111, "BRMI", 26);
	ListaDe144Instrucciones[120] = new BranchInstruction(0b1111000000000100, 0b1111110000000111, "BRLT", 25);
	ListaDe144Instrucciones[121] = new BranchInstruction(0b1111000000000000, 0b1111110000000111, "BRLO", 24);
	ListaDe144Instrucciones[122] = new BranchInstruction(0b1111000000000111, 0b1111110000000111, "BRIE", 23);
	ListaDe144Instrucciones[123] = new BranchInstruction(0b1111010000000111, 0b1111110000000111, "BRID", 22);
	ListaDe144Instrucciones[124] = new BranchInstruction(0b1111000000000101, 0b1111110000000111, "BRHS", 21);
	ListaDe144Instrucciones[125] = new BranchInstruction(0b1111010000000101, 0b1111110000000111, "BRHC", 20);
	ListaDe144Instrucciones[126] = new BranchInstruction(0b1111010000000100, 0b1111110000000111, "BRGE", 19);
	ListaDe144Instrucciones[127] = new BranchInstruction(0b1111000000000001, 0b1111110000000111, "BREQ", 18);
	ListaDe144Instrucciones[128] = new BranchInstruction(0b1111000000000000, 0b1111110000000111, "BRCS", 16);
	ListaDe144Instrucciones[129] = new BranchInstruction(0b1111010000000000, 0b1111110000000111, "BRCC", 15);
	ListaDe144Instrucciones[130] = new BranchInstruction(0b1111000000000000, 0b1111110000000000, "BRBS", 14);
	ListaDe144Instrucciones[131] = new BranchInstruction(0b1111010000000000, 0b1111110000000000, "BRBC", 13);
	ListaDe144Instrucciones[132] = new Instruction(0b0001100000000000, 0b1111110000000000, "SUB", 123);
	ListaDe144Instrucciones[133] = new Instruction(0b0000100000000000, 0b1111110000000000, "SBC", 97);
	ListaDe144Instrucciones[134] = new Instruction(0b0010100000000000, 0b1111110000000000, "OR", 86);
	ListaDe144Instrucciones[135] = new Instruction(0b1001110000000000, 0b1111110000000000, "MUL", 81);
	ListaDe144Instrucciones[136] = new Instruction(0b0010110000000000, 0b1111110000000000, "MOV", 79);
	ListaDe144Instrucciones[137] = new Instruction(0b0010010000000000, 0b1111110000000000, "EOR", 58);
	ListaDe144Instrucciones[138] = new Instruction(0b0001000000000000, 0b1111110000000000, "CPSE", 52);
	ListaDe144Instrucciones[139] = new Instruction(0b0000010000000000, 0b1111110000000000, "CPC", 50);
	ListaDe144Instrucciones[140] = new Instruction(0b0001010000000000, 0b1111110000000000, "CP", 49);
	ListaDe144Instrucciones[141] = new Instruction(0b0010000000000000, 0b1111110000000000, "AND", 8);
	ListaDe144Instrucciones[142] = new Instruction(0b0000110000000000, 0b1111110000000000, "ADD", 6);
	ListaDe144Instrucciones[143] = new Instruction(0b0001110000000000, 0b1111110000000000, "ADC", 5);
	return;
}
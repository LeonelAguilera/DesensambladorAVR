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

bool Instruction::isThisInstruction(word OPcode) {
	return (OPcode & this->_mask) == this->_OPcode;
}

bool Instruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte Rr = OPCode->data & 0x000F | (OPCode->data & 0x0200) >> 5;
	byte Rd = (OPCode->data >> 4) & 0x001F;
	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d,r%d", this->_mnemonic, Rd, Rr);
	OPCode = OPCode->next;
	return true;
}

bool BranchInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data) || this->_man == 13 || this->_man == 14) {
		return false;
	}

	word k = OPCode->data & !this->_mask;
	if (OPCode->data & 0x200) {
		k = k >> 3;
	}
	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d", this->_mnemonic, k); //Pendiente controlar signos negativos
	OPCode = OPCode->next;
	return true;
}

bool InmediateWordInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte K = OPCode->data & 0x000F | (OPCode->data & 0x00C0)>>2;
	byte d = (OPCode->data & 0x0030) >> 4;
	d = 24 + 2 * d;
	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d:r%d,%d", this->_mnemonic, d+1,d,K); //Pendiente controlar signos negativos
	OPCode = OPCode->next;
	return true;
}

bool InmediateByteInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte K = OPCode->data & 0x000F | (OPCode->data & 0x0F00) >> 4;
	byte d = (OPCode->data & 0x00F0) >> 4;
	d += 16;
	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d,%d", this->_mnemonic, d, K); //Pendiente controlar signos negativos
	OPCode = OPCode->next;
	return true;
}

bool SingleRegisterInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte d = (OPCode->data & 0x01F0) >> 4;
	if ((this->_mask & 0x0001) == 0) { //Se trata de BLD o BST
		byte b = (OPCode->data & 0x0007);
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d,%d", this->_mnemonic, d, b);
	}
	else {
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d", this->_mnemonic, d);
	}
	OPCode = OPCode->next;
	return true;
}

bool LoadInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte d = (OPCode->data & 0x01F0) >> 4;
	byte q = (OPCode->data & 0x0007) >> 0 | (OPCode->data & 0x0C00) >> 7 | (OPCode->data & 0x2000) >> 8;

	switch (this->_OPcode) {
	case 0b1001000000000110:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z", this->_mnemonic, d);
		break;
	case 0b1001000000000111:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z+", this->_mnemonic, d);
		break;
	case 0b1001000000001100:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, X", this->_mnemonic, d);
		break;
	case 0b1001000000001101:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, X+", this->_mnemonic, d);
		break;
	case 0b1001000000001110:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, -X", this->_mnemonic, d);
		break;
	case 0b1000000000001000:
		if (q == 0) {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Y", this->_mnemonic, d);
		}
		else {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Y+%d", this->_mnemonic, d, q);
		}
		break;
	case 0b1001000000001001:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Y+", this->_mnemonic, d);
		break;
	case 0b1001000000001010:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, -Y", this->_mnemonic, d);
		break;
	case 0b1000000000000000:
		if (q == 0) {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z", this->_mnemonic, d);
		}
		else {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z+%d", this->_mnemonic, d, q);
		}
		break;
	case 0b1001000000000001:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z+", this->_mnemonic, d);
		break;
	case 0b1001000000000010:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, -Z", this->_mnemonic, d);
		break;
	case 0b1001000000000100:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z", this->_mnemonic, d);
		break;
	case 0b1001000000000101:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, Z+", this->_mnemonic, d);
		break;
	default:
		return false;
	}
	OPCode = OPCode->next;
	return true;
}

bool StoreInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte d = (OPCode->data & 0x01F0) >> 4;
	byte q = (OPCode->data & 0x0007) >> 0 | (OPCode->data & 0x0C00) >> 7 | (OPCode->data & 0x2000) >> 8;

	switch (this->_OPcode) {
	case 0b1001001000000110:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z, r%d", this->_mnemonic, d);
		break;
	case 0b1001001000000101:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z, r%d", this->_mnemonic, d);
		break;
	case 0b1001001000000111:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z, r%d", this->_mnemonic, d);
		break;
	case 0b1111110000000000:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, %d", this->_mnemonic, d, q & 0x07);
		break;
	case 0b1111111000000000:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, %d", this->_mnemonic, d, q & 0x07);
		break;
	case 0b1001001000001100:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s X, r%d", this->_mnemonic, d);
		break;
	case 0b1001001000001101:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s X+, r%d", this->_mnemonic, d);
		break;
	case 0b1001001000001110:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s -X, r%d", this->_mnemonic, d);
		break;
	case 0b1000001000001000:
		if (q == 0) {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Y, r%d", this->_mnemonic, d);
		}
		else {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Y+%d, r%d", this->_mnemonic, d, q);
		}
		break;
	case 0b1001001000001001:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Y+, r%d", this->_mnemonic, d);
		break;
	case 0b1001001000001010:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s -Y, r%d", this->_mnemonic, d);
		break;
	case 0b1000001000000000:
		if (q == 0) {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z, r%d", this->_mnemonic, d);
		}
		else {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z+%d, r%d", this->_mnemonic, d, q);
		}
		break;
	case 0b1001001000000001:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z+, r%d", this->_mnemonic, d);
		break;
	case 0b1001001000000010:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s -Z, r%d", this->_mnemonic, d);
		break;
	case 0b1001001000000100:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s Z, r%d", this->_mnemonic, d);
		break;
	default:
		return false;
	}
	OPCode = OPCode->next;
	return true;
}

bool SREGInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte s = (OPCode->data >> 4) & 0x0007;
	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s %d", this->_mnemonic, s);
	OPCode = OPCode->next;
	return true;
}

bool NoParameterInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s", this->_mnemonic);
	OPCode = OPCode->next;
	return true;
}

bool SingleBitInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte A = (OPCode->data >> 3) & 0x001F;
	byte b = (OPCode->data >> 0) & 0x0007;

	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s %d, %d", this->_mnemonic, A, b);
	OPCode = OPCode->next;
	return true;
}

bool DoubleSingleRegisterInstruction::isThisInstruction(word OPcode) {
	OPcode &= 0x03FF;
	return (OPcode == (OPcode >> 5)) && ((OPcode & this->_mask) == this->_OPcode);
}

bool DoubleSingleRegisterInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte d = (OPCode->data >> 0) & 0x001F;
	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s %d", this->_mnemonic, d);
	OPCode = OPCode->next;
	return true;
}

bool FMULInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte d = (OPCode->data >> 4) & 0x0007;
	byte r = (OPCode->data >> 0) & 0x0007;

	sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, r%d", this->_mnemonic, d, r);
	OPCode = OPCode->next;
	return true;
}

bool IOInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte A = (OPCode->data & 0x0600) >> 5 | (OPCode->data & 0x000F);
	byte dr = (OPCode->data >> 4) & 0x001F;

	if (this->_OPcode & 0x0800) {
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s %d, r%d", this->_mnemonic, A, dr);
	}
	else {
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, %d", this->_mnemonic, dr, A);
	}
	OPCode = OPCode->next;
	return true;
}

bool ExtraInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte d = (OPCode->data >> 4) & 0x000F;
	byte k = ((OPCode->data >> 0) & 0x000F) | ((OPCode->data >> 4) & 0x0070);

	switch (this->_OPcode) {
	case 0b1001010000001011:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s 0x%04X", this->_mnemonic, d);
		break;
	case 0b1010000000000000:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, %d", this->_mnemonic, d + 16, k);
		break;
	case 0b0000000100000000:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d:%d, r%d:%d", this->_mnemonic, (d*2)+1, d*2, ((k&0x0F)*2)+1, (k & 0x0F) * 2);
		break;
	case 0b0000001000000000:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, r%d", this->_mnemonic, d + 16, (k & 0x0F) + 16);
		break;
	case 0b1010100000000000:
		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s %d, r%d", this->_mnemonic, k, d + 16);
		break;
	default:
		return false;
		break;
	}
	OPCode = OPCode->next;
	return true;
}

bool ThirtyTwoBitsInstruction::codeLine(DataLinkedList* OPCode, char* ASMCode) {
	if (!this->isThisInstruction(OPCode->data)) {
		return false;
	}

	byte k = ((OPCode->data >> 0) & 0x000F) | ((OPCode->data >> 4) & 0x0070);

	if (this->_mask & 0x0001) {
		byte d = (OPCode->data >> 4) & 0x001F;
		OPCode = OPCode->next;
		word k = OPCode->data;
		if (this->_OPcode & 0x0200) { //STS
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s %d, r%d", this->_mnemonic, k, d);
		}
		else {
			sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s r%d, %d", this->_mnemonic, d, k);
		}
	}
	else {
		uint32_t k = (OPCode->data & 0x0001) | ((OPCode->data & 0x01F0) >> 3);
		k = k << 16;
		OPCode = OPCode->next;
		k |= OPCode->data;

		sprintf_s(ASMCode, INSTRUCTION_MAX_LENGTH, "%s 0x%06X", this->_mnemonic, k);
	}

	OPCode = OPCode->next;
	return true;
}

void inicializador(Instruction* ListaDe123Instrucciones) {
	return;
}
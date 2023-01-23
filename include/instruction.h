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

#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include "dataLinkedList.h"
#include <malloc.h>
#include <string.h>

#define INSTRUCTION_MAX_LENGTH 50
#define NUM_INSTRUCCIONES 144

class Instruction {
protected:
	const word _OPcode;
	const word _mask;
	const char* _mnemonic;
	const byte _man;
	const static char memLocations[256][7];
public:
	Instruction(word OPcode, word mask, const char* mnemonic, byte man) :_OPcode(OPcode), _mask(mask), _mnemonic(mnemonic), _man(man) {};

	virtual bool isThisInstruction(word OPcode);
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class BranchInstruction :public Instruction {
public:
	BranchInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class InmediateWordInstruction :public Instruction {
public:
	InmediateWordInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class InmediateByteInstruction :public Instruction {
public:
	InmediateByteInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class SingleRegisterInstruction :public Instruction {
public:
	SingleRegisterInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class LoadInstruction :public Instruction {
public:
	LoadInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class StoreInstruction :public Instruction {
public:
	StoreInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class SREGInstruction :public Instruction {
public:
	SREGInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class NoParameterInstruction :public Instruction {
public:
	NoParameterInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class SingleBitInstruction :public Instruction {
public:
	SingleBitInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class DoubleSingleRegisterInstruction :public Instruction {
public:
	DoubleSingleRegisterInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool isThisInstruction(word OPcode);
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class FMULInstruction :public Instruction {
public:
	FMULInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class IOInstruction :public Instruction {
public:
	IOInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class ExtraInstruction :public Instruction {
public:
	ExtraInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

class ThirtyTwoBitsInstruction :public Instruction {
public:
	ThirtyTwoBitsInstruction(word OPcode, word mask, const char* mnemonic, byte man) :Instruction(OPcode, mask, mnemonic, man) {};
	virtual bool codeLine(DataLinkedList* OPCode, char* ASMCode);
};

void inicializador(Instruction** ListaDe144Instrucciones);

#endif // !__INSTRUCTION_H__

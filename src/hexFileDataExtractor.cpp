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

#include "hexFileDataExtractor.h"
#include "hexline.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

DataLinkedList getDataFromHex(FILE* fp) {
	HexLine temp_line = HexLine(fp);
	DataLinkedList instructionsHead = DataLinkedList(temp_line.getDataHead());
	DataLinkedList* temp_data = &instructionsHead;

	while (temp_line.isEOF() == false) {
		while (temp_data->next != 0) {
			//printf("0x%04X\n", temp_data->data);
			temp_data = temp_data->next;
		}
		temp_line = HexLine(fp);
		temp_data->next = (DataLinkedList*)malloc(sizeof(DataLinkedList));
		if (temp_data->next != 0) {
			*temp_data->next = DataLinkedList(temp_line.getDataHead());
		}
		else {
			perror("Error encadenando DataLinkedList de varias lineas");
		}
	}
	//printf("0x%04X\n", temp_data->data);
	return instructionsHead;
}
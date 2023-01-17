#include "hexFileDataExtractor.h"
#include "hexline.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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
		temp_data->next = new DataLinkedList(temp_line.getDataHead());
	}
	//printf("0x%04X\n", temp_data->data);
	return instructionsHead;
}
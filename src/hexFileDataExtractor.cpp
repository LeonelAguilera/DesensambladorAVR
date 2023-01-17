#include "hexFileDataExtractor.h"
#include "hexline.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

HexFileDataExtractor::HexFileDataExtractor(FILE* fp) {
	HexLine temp_line = HexLine(fp);
	this->instructionsHead = DataLinkedList(temp_line.getDataHead());
	DataLinkedList temp_data = this->instructionsHead;

	while (temp_line.isEOF() == false) {
		while (temp_data.next != 0) {
			printf("0x%04X\n", temp_data.data);
			temp_data = temp_data.getNext();
		}
		temp_line = HexLine(fp);
		temp_data.next = new DataLinkedList(temp_line.getDataHead());
	}
	printf("0x%04X\n", temp_data.data);
}
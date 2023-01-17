#ifndef __HexFileDataExtractor_H__
#define	__HexFileDataExtractor_H__

#include "hexline.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

class HexFileDataExtractor {
public:
	DataLinkedList instructionsHead;
public:
	HexFileDataExtractor(FILE* fp);
};

#endif // !__HexFileDataExtractor_H__

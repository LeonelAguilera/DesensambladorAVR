#ifndef __HexFileDataExtractor_H__
#define	__HexFileDataExtractor_H__

#include "hexline.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

DataLinkedList getDataFromHex(FILE* fp);

#endif // !__HexFileDataExtractor_H__

//Copyright 2023 Leonel Alejandro Aguilera Correia
/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or any
later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>

//void fileReadTest();

int main() {

	return 0;
}

/*
void fileReadTest() {
	FILE* fp;

	fopen_s(&fp, "code.hex", "r");
	if (fp == 0) {
		perror("File opening failed");
		return 0;
	}
	char str[50]{};
	while (fgets(str, 10, fp) != NULL) {
		printf("%s", str);
	}

	fclose(fp);
}*/
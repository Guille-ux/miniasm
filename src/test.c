#include "../include/test.h"
#include "../include/miniasm.h"
#include "../include/link.h"
#include <stdio.h>

int main() {
	ByteStream stream;
	LinkerTable *table = (LinkerTable*)malloc(sizeof(LinkerTable)+sizeof(Symbol)*4);	

	assemble_text(&stream, "db 0b00000101", table);

	printf("Data: %d", (uint8_t)*stream.data);

	return 0;
}

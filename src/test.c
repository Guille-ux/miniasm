#include "../include/test.h"
#include "../include/miniasm.h"
#include <stdio.h>

int main() {
	ByteStream stream;
	
	assemble_text(&stream, "db b00000101");

	printf("Data: %d", (uint8_t)*stream.data);

	return 0;
}

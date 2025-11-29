#include "../include/data_asm.h"
#include "../include/compat.h"
#include "../include/stream.h"
#include "../include/directives.h"
#include "../include/utils.h"


void assemble_dd(size_t value, ByteStream *stream) {
	streamAppendByte(stream, ((uint8_t)value) & 0xFF);
	streamAppendByte(stream, ((uint8_t)value >> 8) & 0xFF);
	streamAppendByte(stream, ((uint8_t)value >> 16) & 0xFF);
	streamAppendByte(stream, ((uint8_t)value >> 24) & 0xFF);
}
void assemble_dw(size_t value, ByteStream *stream) {
	streamAppendByte(stream, ((uint8_t)value) & 0xFF);
	streamAppendByte(stream, ((uint8_t)value >> 8) & 0xFF);
}
void assemble_db(size_t value, ByteStream *stream) {
	streamAppendByte(stream, ((uint8_t)value) & 0xFF);
}

void handle_data_directive(ByteStream *stream, directive dir, const char *data, int *pos) {	
	size_t val=0;

	val = getNum(data, pos);
	
	switch (dir) {
		case DIRECTIVE_DB: assemble_db(val, stream); break;
		case DIRECTIVE_DW: assemble_dw(val, stream); break;
		case DIRECTIVE_DD: assemble_dd(val, stream); break;
	}
}

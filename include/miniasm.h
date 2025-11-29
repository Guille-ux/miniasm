#ifndef MINIASM_MINIASM_H
#define MINIASM_MINIASM_H

#include <stdbool.h>
#include "stream.h"

bool assemble_text(ByteStream *outStream, const char *text);
uint8_t assemble_reg(const char *text, int *pos, char *arch);
void skipUntilThese(const char *text, int *pos, const char *list);
void skipThese(const char *text, int *pos, const char *list);

/*
 * Mapeo de Registros para x86-32
 *
 * EAX: 000
 * ECX: 001
 * EDX: 010
 * EBX: 011
 * ESP: 100
 * EBP: 101
 * ESI: 110
 * EDI: 111
 *
 */

#endif

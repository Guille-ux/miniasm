#ifndef MINIASM_MINIASM_H
#define MINIASM_MINIASM_H

#include <stdbool.h>
#include "stream.h"

bool assembly_text(ByteStream *outStream, const char *text);

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

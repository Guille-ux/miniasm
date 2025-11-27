#include "../include/miniasm.h"
#include "../include/compat.h"
#include "../include/stream.h"
#include "../include/utils.h"
#include "../include/data_asm.h"

bool assemble_text(ByteStream *outStream, const char *text) {
	if (outStream == NULL) return false;

	/*
	 *
	 * Aqui iran cosas importantes, pero de momento no
	 *
	 */

	if(initStream(outStream)!=0) return false;

	return true;
}

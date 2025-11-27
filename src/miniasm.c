#include "../include/miniasm.h"
#include "../include/compat.h"
#include "../include/stream.h"
#include "../include/utils.h"
#include "../include/data_asm.h"
#include "../include/directives.h"

bool assemble_text(ByteStream *outStream, const char *text) {
	if (outStream == NULL) return false;

	int pos=0;

	/*
	 *
	 * Aqui iran cosas importantes, pero de momento no
	 *
	 */

	if(initStream(outStream)!=0) return false;

	directive cdir;

	while (1) {
		if (text[pos]=='\n') {
			pos++;
			continue;
		}
		if (text[pos]=='\0') {
			break;
		}

		cdir = WtfDirectiveIs(&text[pos], &pos);

		switch (cdir) {
			case DIRECTIVE_DB:
			case DIRECTIVE_DW:
			case DIRECTIVE_DD:
				handle_data_directive(outStream, cdir, &text[pos], &pos);
				break;
			default: break;
		}

		pos++;
	}

	return true;
}

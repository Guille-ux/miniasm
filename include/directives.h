#ifndef MINIASM_DIRECTIVES_H
#define MINIASM_DIRECTIVES_H

#include <stdbool.h>

typedef enum {
	DIRECTIVE_DB,
	DIRECTIVE_DW,
	DIRECTIVE_DD,
	DIRECTIVE_NULL,
} directive;

directive WtfDirectiveIs(const char *string, int *pos);
bool isPattern(const char *string, const char *pattern);

#endif

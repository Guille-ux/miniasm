#ifndef MINIASM_DIRECTIVES_H
#define MINIASM_DIRECTIVES_H

#include <stdbool.h>

typedef enum {
	DIRECTIVE_DB,
	DIRECTIVE_DW,
	DIRECTIVE_DD,
	DIRECTIVE_MOVI,
	DIRECTIVE_NULL,
	DIRECTIVE_TIMES_SIZE,
	DIRECTIVE_TIMES,
	DIRECTIVE_LABEL,
	DIRECTIVE_LABELDEF,
	DIRECTIVE_ADD,
	DIRECTIVE_SUB,
	DIRECTIVE_OR,
	DIRECTIVE_AND,
	DIRECTIVE_ADC,
	DIRECTIVE_SBB,
	DIRECTIVE_XOR,
} directive;

directive WtfDirectiveIs(const char *string, size_t *pos);
bool isPattern(const char *string, const char *pattern);

#endif

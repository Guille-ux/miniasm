#include "../include/directives.h"
#include "../include/compat.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

bool isPattern(const char *string, const char *pattern) {
	for (int i=0;pattern[i]!='\0';i++) {
		if (pattern[i]!=string[i]) return false;
	}
	return true;
}

directive WtfDirectiveIs(const char *string, size_t *pos) {
	if (string==NULL || pos == NULL) return DIRECTIVE_NULL;

	if (isPattern(string, "db ")) {
		*pos+=3;
		return DIRECTIVE_DB;
	} else if (isPattern(string, "dw ")) {
		*pos+=3;
		return DIRECTIVE_DW;
	} else if (isPattern(string, "dd ")) {
		*pos+=3;
		return DIRECTIVE_DD;
	} else if (isPattern(string, "mov ")) {
		*pos+=4;
		return DIRECTIVE_MOVI;
	} else if (isPattern(string, ".label ")) {
		*pos+=7;
		return DIRECTIVE_LABEL;
	} else if (isPattern(string, ".labeldef ")) {
		*pos+=10;
		return DIRECTIVE_LABELDEF;
	} else if (isPattern(string, "%times size ")) {
		*pos+=12;
		return DIRECTIVE_TIMES_SIZE;
	} else if (isPattern(string, "%times ")) {
		*pos+=7;
		return DIRECTIVE_TIMES;
	} else if (isPattern(string, "add ")) {
		*pos+=4;
		return DIRECTIVE_ADD;
	} else if (isPattern(string, "sub ")) {
		*pos+=4;
		return DIRECTIVE_SUB;
	} else if (isPattern(string, "or ")) {
		*pos+=3;
		return DIRECTIVE_OR;
	} else if (isPattern(string, "and ")) {
		*pos+=4;
		return DIRECTIVE_AND;
	} else if (isPattern(string, "adc ")) {
		*pos+=4;
		return DIRECTIVE_ADC;
	} else if (isPattern(string, "sbb ")) {
		*pos+=4;
		return DIRECTIVE_SBB;
	} else if (isPattern(string, "xor ")) {
		*pos+=4;
		return DIRECTIVE_XOR;
	}

	return DIRECTIVE_NULL;
}

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

directive WtfDirectiveIs(const char *string, int *pos) {
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
	} else if (isPattern(string, "x86mov ")) {
		*pos+=7;
		return DIRECTIVE_MOVI;
	} else if (isPattern(string, ".label ")) {
		*pos+=7;
		return DIRECTIVE_LABEL;
	} else if (isPattern(string, "%times size ")) {
		*pos+=12;
		return DIRECTIVE_TIMES_SIZE;
	} else if (isPattern(string, "%times ")) {
		*pos+=7;
		return DIRECTIVE_TIMES;
	}

	return DIRECTIVE_NULL;
}

#include "../include/utils.h"
#include <stdint.h>
#include <stddef.h>
#include "../include/directives.h"


size_t atob(const char *string, size_t *pos) {
	size_t bin = 0;
	char *ch = string;
	int z=0;
	while (*ch=='0' || *ch=='1') {
		if (*ch++=='1') bin += 1;
		bin <<= 1;
		z++;
	}
	bin >>= 1;
	*pos+=z;
	return bin;
}

size_t getNum(const char *string, size_t *pos) {
	*pos+=2;
	size_t val;
	if (isPattern(string, "0b")) {
		val = atob(&string[*pos], pos);
	}

	return val;
}

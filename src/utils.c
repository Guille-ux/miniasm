#include "../include/utils.h"
#include <stdint.h>
#include <stddef.h>

size_t atob(const char *string, int *pos) {
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

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
	} else if (isPattern(string, "0d")) {
		val = atol(&string[*pos], pos);
	} else if (isPattern(string, "0x")) {
		val = atoh(&string[*pos], pos);
	} else if (isPattern(string, "'")) {
		if (string[*pos]=='\') {
			*pos++;
			if (string[*pos]=='n') {
				val = '\n';
				
			} else if (string[*pos]=='t') {
				val = '\t';
			} else if (string[*pos]=='\') {
				val = '\';
			}
		} else {
			val = string[*pos];
		}
		*pos++;
		if (string[*pos]!="'") {
			// Error!
			return (size_t)-1;
		}
	}

	return val;
}

size_t atol(const char *string, size_t *pos) {
	size_t val=0;
	int i=0;
	while (string[i] <= '9' && string[i] >= '0') {
		val += string[i] - '0';
		val *=10;
		i++;
		*pos++;
	}
	val /= 10;
	return val;
}

size_t atoh(const char *string, size_t *pos) {
	size_t val=0;
	int i=0;
	while ((string[i] <= '9' && string[i] >= '0') || (string[i] <= 'F' && string[i] >= 'A')) {
		val += (string[i]>'9') ? string[i]-'0' :  string[i] - 'A' + 10;
		val *=16;
		i++;
		*pos++;
	}
	val /= 16;
	return val;
}

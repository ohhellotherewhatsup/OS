#include <stdint.h>

#include "bool.h"

BOOL util_strcmp(char *str1, char *str2) {
	while (*str1) {
		if (*str1++ != *str2++)
			return FALSE;
	}

	return (!(*str1) && !(*str2));
}

void util_memset(void *mem, char c, int len) {
	while (len--)
		*(char *)mem++ = c;
}

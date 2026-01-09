#include "../kernel.h"

void *memset(void *s, int c, size_t n) {
	char *s1 = s;
	size_t i = 0;
	while (i < n)
		s1[i++] = c;
	return s1;
}

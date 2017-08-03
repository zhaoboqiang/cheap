#include "strip_extension.h"
#include <string.h>

errno_t strip_extension(char* filepath) {
	errno_t r = -1;
	char* delimiter;

	delimiter = strrchr(filepath, '.');
	if (delimiter == NULL)
		goto LABEL_ERROR;

	*delimiter = 0;

	r = 0;
LABEL_ERROR:
	return r;
}

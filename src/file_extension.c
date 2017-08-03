/*
** Born to code, die for bugs! 
*/
#include "file_extension.h"
#include <string.h>

errno_t strip_file_extension(char* filepath) {
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

errno_t substitute_file_extension(char* filepath, size_t length, char const* extension) {
	errno_t r = -1;

	if (strip_file_extension(filepath))
		goto LABEL_ERROR;

	if (strcat_s(filepath, length, extension))
		goto LABEL_ERROR;

	r = 0;
LABEL_ERROR:
	return r;
}

/*
** MIT License
**
** Copyright (c) 2012 Zhao Boqiang
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
*/


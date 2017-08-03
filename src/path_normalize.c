/*
** Born to code, die for bugs!
*/
#include "path_normalize.h"
#include <assert.h>

int is_path_normalize(char const* path) {
	char c;

	if (path == NULL)
		return 0;

	for (;;) {
		c = *path;

		if (c == 0)
			break;

		if (c == '\\')
			return 0;

		++path;
	}

	return 1;
}

void normalize_path(char* path, size_t count) {
	size_t index;
	char c;

	assert(path);

	for (index = 0; index < count; ++index) {
		c = path[index];

		if (c == '\0') {
			break;
		}
		if (c == '\\') {
			path[index] = '/';
		}
	}

	if (index > 0 && path[index - 1] != '/') {
		assert(index + 1 < count);

		path[index] = '/';
		path[index + 1] = '\0';
	}
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


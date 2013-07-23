/*
** Born to code, die for bugs! 
*/

#include "./split_text.h"
#include <assert.h>

void split_line(size_t* split_length, size_t* delimiter_length, char* text, size_t size, char delimiter) {
	size_t offset;
	char c;

	for (offset = 0; offset < size; ++offset) {
		c = text[offset];

		/* escape sequences */
		if (c == '\\') {
			++offset;
			assert(offset < size);
			continue;
		}

		if (c == '\n') {
			text[offset] = delimiter;
			*split_length = offset + 1;
			*delimiter_length = 1;
			return;
		}

		if (c == '\r') {
			if (offset + 1 < size && text[offset + 1] == '\n') {
				text[offset] = delimiter;
				*split_length = offset + 2;
				*delimiter_length = 2;
				return;
			} else {
				text[offset] = delimiter;
				*split_length = offset + 1;
				*delimiter_length = 1;
				return;
			}
		}
	}

	*split_length = size;
	*delimiter_length = 0;
}

void split_text(size_t* split_length, size_t* delimiter_length, char* text, size_t size, char delimiter) {
	size_t offset;
	char c;

	for (offset = 0; offset < size; ++offset) {
		c = text[offset];

		/* escape sequences */
		if (c == '\\') {
			++offset;
			assert(offset < size);
			continue;
		}

		if (c == delimiter) {
			text[offset] = '\0';
			*split_length = offset + 1;
			*delimiter_length = 1;
			return;
		}
		assert(c != '\n');
		assert(c != delimiter);
	}

	*split_length = size;
	*delimiter_length = 0;
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


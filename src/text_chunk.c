/*
** Born to code, die for bugs! 
*/

#include "text_chunk.h"
#include "safe_free.h"
#include <assert.h>
#include <string.h>

errno_t alloc_text_chunk(struct text_chunk_t* text_chunk, uint32_t size) {
	errno_t r = -1;
	char* text = NULL;

	text = malloc(size);
	if (text == NULL)
		goto LABEL_ERROR;

	text_chunk->size = size;
	text_chunk->offset = 0;
	text_chunk->text = text;
	text = NULL;

	r = 0;
LABEL_ERROR:
	SAFE_FREE(text);
	return r;
}

void free_text_chunk(struct text_chunk_t* text_chunk) {
	SAFE_FREE(text_chunk->text);
}

uint32_t put_text(struct text_chunk_t* text_chunk, char const* text) {
	int offset;
	uint32_t size;

	assert(text_chunk);
	assert(text);

	offset = get_text_offset(text_chunk, text);
	if (offset != -1) {
		return (uint32_t)offset;
	} else {
		size = (uint32_t)strlen(text) + 1;

		assert(text_chunk->offset + size < text_chunk->size);

		strcpy_s(text_chunk->text + text_chunk->offset, text_chunk->size - text_chunk->offset, text);

		offset = (int)text_chunk->offset;

		text_chunk->offset += size;

		return (uint32_t)offset;
	}
}

int get_text_offset(struct text_chunk_t* text_chunk, char const* text) {
	uint32_t size;
	uint32_t size_iterator;
	char const* text_iterator;

	assert(text_chunk);
	assert(text);

	size = (uint32_t)strlen(text);

	assert(size > 0);

	text_iterator = text_chunk->text;

	do {
		size_iterator = (uint32_t)strlen(text_iterator);
		if (size == size_iterator && strcmp(text, text_iterator) == 0) {
			return (int)(text_iterator - text_chunk->text);
		}
		text_iterator += size_iterator + 1;
	} while (size_iterator > 0);
	
	return -1;
}

char const* get_text(struct text_chunk_t* text_chunk, size_t offset) {
	assert(text_chunk);

	return text_chunk->text + offset;
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


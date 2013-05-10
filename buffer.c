/*
** Born to code, die for bugs! 
** http://code.google.com/p/cheap/
*/

#include "./buffer.h"
#include "./file_size.h"
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>

errno_t alloc_buffer(struct buffer_t* buffer) {
    errno_t r = -1;
    void* data = NULL;
    
    assert(buffer);
    assert(buffer->data == NULL);
    assert(buffer->size > 0);

    data = malloc(buffer->size);
    if (data == NULL) {
        assert(0);
        goto LABEL_ERROR;
    }

	buffer->init_size = buffer->size;
	buffer->init_data = data;
    buffer->data = data;
    data = NULL;

    r = 0;
LABEL_ERROR:
    if (data) {
        free(data);
    }
    return r;
}

void free_buffer(struct buffer_t* buffer) {
    assert(buffer);
    assert(buffer->init_data);

    free(buffer->init_data);

	buffer->init_data = NULL;
	buffer->init_size = 0;

    buffer->data = NULL;
    buffer->size = 0;
}

void* seek_buffer(struct buffer_t* buffer, size_t offset) {
    void* data = NULL;

    assert(buffer);
    assert(buffer->data);
    assert(buffer->size >= offset);

	if (buffer->size < offset) {
		return NULL;
	}

    data = buffer->data;

    buffer->data = (unsigned char*)buffer->data + offset;
    buffer->size -= offset;

    return data;
}

errno_t printf_buffer(struct buffer_t* buffer, char const* fmt, ...) {
	errno_t r = -1;
    va_list args;
    int size;

    va_start(args, fmt);
    
	size = _vscprintf(fmt, args) + 1;
	if ((int)buffer->size < size) {
		goto LABEL_ERROR;
	}

	size = vsprintf_s(buffer->data, buffer->size, fmt, args);

	seek_buffer(buffer, size);

	va_end(args);

	r = 0;
LABEL_ERROR:
	return r;
}

errno_t load_buffer(struct buffer_t* buffer, char const* filepath) {
    errno_t r = -1;
	errno_t alloc_buffer_errno = -1;
    FILE* fp = NULL;
    size_t read_count;
    size_t size;
    
    assert(buffer);
    assert(buffer->data == NULL);
    assert(buffer->size == 0);
    assert(buffer->init_data == NULL);
    assert(buffer->init_size == 0);

	if (fopen_s(&fp, filepath, "rb")) {
        assert(0);
        goto LABEL_ERROR;
    }

    size = get_file_size(fp);
    if (size == 0) {
        assert(0);
        goto LABEL_ERROR;
    }

	buffer->size = size;

	alloc_buffer_errno = alloc_buffer(buffer);
	if (alloc_buffer_errno) {
		assert(0);
		goto LABEL_ERROR;
	}

    read_count = fread_s(buffer->data, size, size, 1, fp);
    if (read_count != 1) {
        assert(0);
        goto LABEL_ERROR;
    }

    r = 0;
LABEL_ERROR:
	if (r) {
		if (alloc_buffer_errno == 0) {
			free_buffer(buffer);
		}
	}
    if (fp) {
        fclose(fp);
    }
    return r;
}

errno_t save_buffer(struct buffer_t* buffer, char const* filepath) {
    errno_t r = -1;
    FILE* fp;
    
    if (fopen_s(&fp, filepath, "wb")) {
		assert(0);
		goto LABEL_ERROR;
	}

	fwrite(buffer->init_data, buffer->init_size - buffer->size, 1, fp);
	fclose(fp);

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


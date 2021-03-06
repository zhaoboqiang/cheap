/*
** Born to code, die for bugs! 
*/

#include "stack_alloc.h"
#include <assert.h>
#include <stdio.h>

void* stack_alloc(struct stack_alloc_t* stack_alloc, size_t size) {
	void* data = NULL;
	size = (size + 0x0F) & ~0x0F;

	if (stack_alloc->offset + size > stack_alloc->size) {
		printf("[stack pool] alloc failed %d + %d > %d\n", stack_alloc->offset, size, stack_alloc->size);
		assert(0);
		goto LABEL_ERROR;
	}

	data = (unsigned char*)stack_alloc->buffer + stack_alloc->offset;
	stack_alloc->offset += size;

LABEL_ERROR:
	return data;
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


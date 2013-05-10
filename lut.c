/*
** Born to code, die for bugs! 
** http://code.google.com/p/cheap/
*/

#include "./lut.h"
#include <assert.h>
#include <search.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>

errno_t init_lut(struct lut_t* lut) {
	errno_t r = -1;
	struct lut_opt_t* opt;
	size_t keys_size;
	size_t objs_size;
	size_t buffer_size;
	void* buffer = NULL;

	opt = lut->opt;

	assert(opt->key_size > 0);
	assert(lut->obj_size > 0);
	assert(lut->count > 0);

	keys_size = lut->count * (opt->key_size + sizeof(int));
	objs_size = lut->count * lut->obj_size;

	buffer_size = keys_size + objs_size;

	buffer = malloc(buffer_size);
	if (buffer == NULL) {
		printf("[lut] malloc(%d) failed.\n", buffer_size);
		goto LABEL_ERROR;
	}
	
	lut->buffer = buffer;
	buffer = NULL;

	r = 0;
LABEL_ERROR:
	if (buffer) { 
		free(buffer);
	}
	return r;
}

void exit_lut(struct lut_t* lut) {
	if (lut->buffer) {
		free(lut->buffer);
	}
}

void* get_objs(struct lut_t* lut) {
	struct lut_opt_t* opt;
	unsigned char* keys;
	unsigned char* objs;

	opt = lut->opt;

	keys = (unsigned char*)lut->buffer;
	objs = keys + lut->count * (opt->key_size + sizeof(int));

	return objs;
}

void sort_table(struct lut_t* lut) {
	struct lut_opt_t* opt;
	size_t index;
	size_t count;
	unsigned char* key_index;
	unsigned char* obj_key;

	opt = lut->opt;
	count = lut->count;

	key_index = (unsigned char*)lut->buffer;
	obj_key = key_index + lut->count * (opt->key_size + sizeof(int)) + lut->key_offset;

	for (index = 0; index < count; ++index) {
		memcpy(key_index, obj_key, opt->key_size);
		*((int*)(key_index + opt->key_size)) = index;
		key_index += opt->key_size + sizeof(int);
		obj_key += lut->obj_size;
	}

	qsort(lut->buffer, count, opt->key_size + sizeof(int), opt->key_cmp);
}

void* lookup_table(struct lut_t* lut, void* key) {
	struct lut_opt_t* opt;
	unsigned char* keys;
	unsigned char* objs;
	void* result;
	int index;

	opt = lut->opt;

	result = bsearch(key, lut->buffer, lut->count, opt->key_size + sizeof(int), opt->key_cmp);
	if (result == NULL) {
		return NULL;
	}

	keys = (unsigned char*)lut->buffer;
	objs = keys + lut->count * (opt->key_size + sizeof(int));

	index = *(int*)((unsigned char*)result + opt->key_size);

	assert(index >= 0 && index < (int)lut->count);

	return objs + index * lut->obj_size;
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


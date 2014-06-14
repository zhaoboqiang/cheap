/*
** Born to code, die for bugs! 
*/

#include "./chunk.h"
#include "./rbuffer.h"
#include <assert.h>
#include <search.h>

static int chunk_compare(void const* l, void const* r) {
	struct chunk_t const* chunk_l;
	struct chunk_t const* chunk_r;

	chunk_l = l;
	chunk_r = r;

	return (int)chunk_l->identity - (int)chunk_r->identity;
}

struct chunk_t* seek_chunk(unsigned long identity, struct chunk_t* chunk_table, size_t chunk_count) {
	struct chunk_t chunk_key;

	assert(chunk_table);
	assert(chunk_count > 0);

	chunk_key.identity = identity;

	return (struct chunk_t*)bsearch(&chunk_key, chunk_table, chunk_count, sizeof(struct chunk_t), chunk_compare);
}

void init_buffer(struct rbuffer_t* rbuffer, struct chunk_t* chunk, void* base, size_t size) {
	assert(rbuffer);
	assert(chunk);
	assert(base);
	assert(size > 0);

	assert(chunk->size <= size);

	rbuffer->data = (unsigned char*)base + chunk->offset;
	rbuffer->size = chunk->size;
}

void sort_chunk_table(struct chunk_t* chunk_table, size_t chunk_count) {
	assert(chunk_table);
	assert(chunk_count > 0);

	qsort(chunk_table, chunk_count, sizeof(struct chunk_t), chunk_compare);
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


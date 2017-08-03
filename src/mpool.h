/*
** Born to code, die for bugs! 
*/

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
	MPOOL_FIXED,
	MPOOL_RESIZE,
};

struct mpool_opt_t {
	char const* name;
	size_t chunk_object_count;
	size_t object_size;
};

struct mpool_t {
	struct mpool_opt_t* opt;

	struct mpool_chunk_t* chunk_head;
	struct mpool_chunk_t* chunk_tail;
	struct mpool_object_t* free_object_list;
};

errno_t init_mpool(struct mpool_t* mpool);
void free_mpool(struct mpool_t* mpool);

void purge_mpool(struct mpool_t* mpool);

void* mpool_alloc(struct mpool_t* mpool, int type);
void mpool_free(struct mpool_t* mpool, void* p);

int mpool_index(struct mpool_t* mpool, void* p);
void* mpool_object(struct mpool_t* mpool, int index);

#ifdef __cplusplus
}
#endif

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


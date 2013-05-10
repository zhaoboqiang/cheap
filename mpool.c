/*
** Born to code, die for bugs! 
** http://code.google.com/p/cheap/
*/

#include "./mpool.h"
#include <assert.h>
#include <stdio.h>
#include <malloc.h>

struct mpool_chunk_t {
	struct mpool_chunk_t* next;
	char data[1];
};

struct mpool_object_t {
	union {
		struct mpool_object_t* next;
		void* data;
	};
};

static void mpool_alloc_chunk(struct mpool_t* mpool) {
	struct mpool_chunk_t* chunk;
	struct mpool_object_t* object;
	size_t i;

	assert(mpool->opt->object_size > 0);
	assert(mpool->opt->chunk_object_count > 0);

	chunk = malloc(sizeof(struct mpool_chunk_t*) + mpool->opt->chunk_object_count * mpool->opt->object_size);
	if (chunk == NULL) {
		printf("[mem pool] alloc failed %d %d %s\n", mpool->opt->chunk_object_count, mpool->opt->object_size, mpool->opt->name);
		return;
	}

	/* push back */
	chunk->next = NULL;

	if (mpool->chunk_head == NULL) {
		mpool->chunk_head = chunk;
	} else {
		mpool->chunk_tail->next = chunk;  
	}

	mpool->chunk_tail = chunk;

	/* append to free object list */
	for (i = 0; i < mpool->opt->chunk_object_count; ++i) {
		object = (struct mpool_object_t*)(mpool->chunk_tail->data + i * mpool->opt->object_size);
		object->next = mpool->free_object_list;
		mpool->free_object_list = object;
	}
}

errno_t init_mpool(struct mpool_t* mpool) {
	errno_t r = -1;

	if (mpool->opt->object_size == 0) {
		goto LABEL_ERROR;
	}

	assert(mpool->free_object_list == NULL);
	assert(mpool->chunk_head == NULL);
	assert(mpool->chunk_tail == NULL);
	assert(mpool->opt->object_size >= sizeof(struct mpool_object_t*));

	mpool->opt->object_size = (mpool->opt->object_size + 0x0F) & ~0x0F;
	mpool->opt->chunk_object_count = mpool->opt->chunk_object_count;

	mpool_alloc_chunk(mpool);

	r = 0;
LABEL_ERROR:
	return r;
}

void exit_mpool(struct mpool_t* mpool) {
	size_t alloc_object_count = 0;
	size_t free_object_count = 0;
	struct mpool_chunk_t* chunk;
	struct mpool_object_t* object;

	for (chunk = mpool->chunk_head; chunk; chunk = chunk->next) {
		alloc_object_count += mpool->opt->chunk_object_count;
	}

	for (object = mpool->free_object_list; object; object = object->next) {
		++free_object_count;
	}

	if (alloc_object_count > free_object_count) {
		printf("[mem pool] leak %d %s\n", alloc_object_count - free_object_count, mpool->opt->name);
	}

	purge_mpool(mpool);
}

void purge_mpool(struct mpool_t* mpool) {
	struct mpool_chunk_t* chunk;

	while (mpool->chunk_head) {
		chunk = mpool->chunk_head;
		mpool->chunk_head = mpool->chunk_head->next;
		free(chunk);
	}

	mpool->chunk_head = NULL;
	mpool->chunk_tail = NULL;
	mpool->free_object_list = NULL;
}

void* mpool_alloc(struct mpool_t* mpool, int type) {
	struct mpool_object_t* object;

	if (mpool->free_object_list == NULL && type == MPOOL_RESIZE) {
		mpool_alloc_chunk(mpool);
	}

	object = mpool->free_object_list;

	if (mpool->free_object_list) {
		mpool->free_object_list = mpool->free_object_list->next;
	}

	return object;
}

void mpool_free(struct mpool_t* mpool, void* p) {
	struct mpool_object_t* object;

	assert(p);

	object = p;

	object->next = mpool->free_object_list;
	mpool->free_object_list = object;
}

int mpool_index(struct mpool_t* mpool, void* p) {
	struct mpool_chunk_t* chunk;
	size_t chunk_size;
	int object_count;

	chunk_size = mpool->opt->chunk_object_count * mpool->opt->object_size;
	object_count = 0;
	
	for (chunk = mpool->chunk_head; chunk; chunk = chunk->next) {
		if ((char*)p >= chunk->data && (char*)p < chunk->data + chunk_size) {
			object_count += ((char*)p - chunk->data) / mpool->opt->object_size;
			return object_count;
		}
		object_count += mpool->opt->chunk_object_count;
	}

	return -1;
}

void* mpool_object(struct mpool_t* mpool, int index) {
	struct mpool_chunk_t* chunk;

	for (chunk = mpool->chunk_head; chunk; chunk = chunk->next) {

		assert(index >= 0);

		if (index < (int)mpool->opt->chunk_object_count) {
			return chunk->data + index * mpool->opt->object_size;
		}

		index -= mpool->opt->chunk_object_count;
	}

	return NULL;
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


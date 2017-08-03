/*
** Born to code, die for bugs! 
*/

#include "batch_alloc.h"
#include <assert.h>
#include <malloc.h>

struct batch_alloc_node_t {
	struct batch_alloc_node_t* next;
};

void* batch_alloc(struct batch_alloc_t* batch_alloc, size_t size) {
	void* data = NULL;
	struct batch_alloc_node_t* node;
	struct batch_alloc_node_t* head;

	node = (struct batch_alloc_node_t*)malloc(sizeof(struct batch_alloc_node_t) + size);
	if (node == NULL)
		goto LABEL_ERROR;

	head = (struct batch_alloc_node_t*)batch_alloc->free_list;
	if (head) {
		node->next = head->next;
	} else {
		node->next = NULL;
	}
	
	batch_alloc->free_list = node;

	data = (void*)(node + 1);
LABEL_ERROR:
	return data;
}

void batch_free(struct batch_alloc_t* batch_alloc) {
	struct batch_alloc_node_t* node;
	struct batch_alloc_node_t* free_node;

	node = (struct batch_alloc_node_t*)batch_alloc->free_list;

	while (node) {
		free_node = node;
		node = node->next;

		free(free_node);
	}

	batch_alloc->free_list = NULL;
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


/*
** Born to code, die for bugs! 
*/

#pragma once

#include "./mpool.h"
#include "./bbox2.h"

#ifdef __cplusplus
extern "C" {
#endif

struct quadtree_data_t {
	struct quadtree_data_t* next;

	void* value;
};

enum QUADTREE_CHILD {
	QUADTREE_CHILD_NORTH_EAST,
	QUADTREE_CHILD_NORTH_WEST,
	QUADTREE_CHILD_SOUTH_WEST,
	QUADTREE_CHILD_SOUTH_EAST,
	
	QUADTREE_CHILD_COUNT,
};

struct quadtree_node_t {
	struct bbox2_t bbox;

	struct quadtree_node_t* children[QUADTREE_CHILD_COUNT];

	struct quadtree_data_t* data;
};

struct quadtree_opt_t {
	struct mpool_opt_t node_pool_opt;
	struct mpool_opt_t data_pool_opt;

	struct bbox2_t bbox;

	int node_pool_alloc_type;
	int min_size;
	int aligned_size;
};

struct quadtree_t {
	struct quadtree_opt_t* opt;

	struct quadtree_node_t* root;

	struct mpool_t node_pool;
	struct mpool_t data_pool;
};

errno_t init_quadtree(struct quadtree_t* quadtree);
void free_quadtree(struct quadtree_t* quadtree);

void purge_quadtree(struct quadtree_t* quadtree);

int insert_quadtree(struct quadtree_t* quadtree, struct bbox2_t* bbox, void* data);
int delete_quadtree(struct quadtree_t* quadtree, struct bbox2_t* bbox, void* data);

struct quadtree_data_t* query_quadtree(struct quadtree_t* quadtree, struct vector2_t* position);

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


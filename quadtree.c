/*
** Born to code, die for bugs! 
** http://code.google.com/p/cheap/
*/

#include "./quadtree.h"
#include "./nextpow2.h"
#include "./vector2.h"
#include <assert.h>
#include <memory.h>

static struct quadtree_node_t* alloc_quadtree_node(struct mpool_t* node_pool, int node_pool_alloc_type) {
	struct quadtree_node_t* node;

	node = mpool_alloc(node_pool, node_pool_alloc_type);
	if (node == NULL) {
		goto LABEL_ERROR;
	}

	memset(node, 0, sizeof(struct quadtree_node_t));

LABEL_ERROR:
	return node;
}

static void get_child_bbox(struct bbox2_t* child_bbox, struct bbox2_t* parent_bbox, int child) {
	switch (child) {
	case QUADTREE_CHILD_NORTH_EAST:
		child_bbox->x_min = parent_bbox->x_min;
		child_bbox->x_max = (parent_bbox->x_min + parent_bbox->x_max) / 2;
		child_bbox->y_min = parent_bbox->y_min;
		child_bbox->y_max = (parent_bbox->y_min + parent_bbox->y_max) / 2;
		break;
	case QUADTREE_CHILD_NORTH_WEST:
		child_bbox->x_min = (parent_bbox->x_min + parent_bbox->x_max) / 2;
		child_bbox->x_max = parent_bbox->x_max;
		child_bbox->y_min = (parent_bbox->y_min + parent_bbox->y_max) / 2;
		child_bbox->y_max = parent_bbox->y_max;
		break;
	case QUADTREE_CHILD_SOUTH_WEST:
		child_bbox->x_min = parent_bbox->x_min;
		child_bbox->x_max = (parent_bbox->x_min + parent_bbox->x_max) / 2;
		child_bbox->y_min = (parent_bbox->y_min + parent_bbox->y_max) / 2;
		child_bbox->y_max = parent_bbox->y_max;
		break;
	case QUADTREE_CHILD_SOUTH_EAST:
		child_bbox->x_min = (parent_bbox->x_min + parent_bbox->x_max) / 2;
		child_bbox->x_max = parent_bbox->x_max;
		child_bbox->y_min = (parent_bbox->y_min + parent_bbox->y_max) / 2;
		child_bbox->y_max = parent_bbox->y_max;
		break;
	default:
		assert(0);
		break;
	}
}

static errno_t alloc_quadtree_child_node(struct quadtree_t* quadtree, struct quadtree_node_t* node, int child) {
	errno_t r = -1;
	struct quadtree_node_t* child_node;

	assert(child >= 0 && child < QUADTREE_CHILD_COUNT);
	
	child_node = alloc_quadtree_node(&quadtree->node_pool, quadtree->opt->node_pool_alloc_type);
	if (child_node == NULL) {
		goto LABEL_ERROR;
	}

	get_child_bbox(&child_node->bbox, &node->bbox, child);
	node->children[child] = child_node;

	r = 0;
LABEL_ERROR:
	return r;
}

errno_t init_root_bbox(struct quadtree_t* quadtree) {
	errno_t r = -1;
	struct quadtree_opt_t* opt;
	struct quadtree_node_t* node;
	struct bbox2_t bbox;
	int cx;
	int cy;
	int edge;
	int aligned_size;

	opt = quadtree->opt;

	aligned_size = opt->aligned_size;

	bbox.x_min = (opt->bbox.x_min - (aligned_size - 1)) / aligned_size * aligned_size;
	bbox.y_min = (opt->bbox.y_min - (aligned_size - 1)) / aligned_size * aligned_size;
	bbox.x_max = (opt->bbox.x_max - (aligned_size - 1)) / aligned_size * aligned_size;
	bbox.y_max = (opt->bbox.y_max - (aligned_size - 1)) / aligned_size * aligned_size;

	cx = bbox.x_max - bbox.x_min;
	cy = bbox.y_max - bbox.y_min;

	if (cx >= cy) {
		edge = cx;
	} else {
		edge = cy;
	}

	edge = nextpow2(edge);

	bbox.x_max = bbox.x_min + edge;
	bbox.y_max = bbox.y_min + edge;

	assert(quadtree->root == NULL);

	node = alloc_quadtree_node(&quadtree->node_pool, quadtree->opt->node_pool_alloc_type);
	if (node == NULL) {
		goto LABEL_ERROR;
	}
	
	node->bbox = bbox;

	quadtree->root = node;

	r = 0;
LABEL_ERROR:
	return r;
}

errno_t init_quadtree(struct quadtree_t* quadtree) {
	errno_t r = -1;
	errno_t init_node_pool_errno = -1;
	errno_t init_data_pool_errno = -1;

	quadtree->opt->node_pool_opt.name = "quadtree_node";
	quadtree->opt->node_pool_opt.object_size = sizeof(struct quadtree_node_t);

	quadtree->opt->data_pool_opt.name = "quadtree_data";
	quadtree->opt->data_pool_opt.object_size = sizeof(struct quadtree_data_t);

	quadtree->node_pool.opt = &quadtree->opt->node_pool_opt;
	quadtree->data_pool.opt = &quadtree->opt->data_pool_opt;

	init_node_pool_errno = init_mpool(&quadtree->node_pool);
	if (init_node_pool_errno) {
		goto LABEL_ERROR;
	}

	init_data_pool_errno = init_mpool(&quadtree->data_pool);
	if (init_data_pool_errno) {
		goto LABEL_ERROR;
	}

	r = 0;
LABEL_ERROR:
	if (r) {
		if (init_data_pool_errno == 0) {
			exit_mpool(&quadtree->data_pool);
		}
		if (init_node_pool_errno == 0) {
			exit_mpool(&quadtree->node_pool);
		}
	}
	return r;
}

void exit_quadtree(struct quadtree_t* quadtree) {
	purge_quadtree(quadtree);

	exit_mpool(&quadtree->data_pool);
	exit_mpool(&quadtree->node_pool);
}

static void purge_quadtree_data(struct quadtree_t* quadtree, struct quadtree_node_t* node) {
	struct quadtree_data_t* data;
	struct quadtree_data_t* prev;

	data = node->data;

	if (data) {
		while (data) {
			prev = data;
			data = data->next;

			mpool_free(&quadtree->data_pool, prev);
		}

		node->data = NULL;
	}
}

static int delete_quadtree_data(struct quadtree_t* quadtree, struct quadtree_node_t* node, void* value) {
	int delete_count;
	struct quadtree_data_t* data;
	struct quadtree_data_t* prev;

	delete_count = 0;

	if (node->data == NULL) {
		goto LABEL_ERROR;
	}

	data = node->data;
	if (data->value == value) {
		node->data = data->next;
		mpool_free(&quadtree->data_pool, data);
		++delete_count;
	} else {
		for (prev = node->data; prev->next; prev = prev->next) {
			data = prev->next;
			if (data->value == value) {
				prev->next = data->next;
				mpool_free(&quadtree->data_pool, data);
				++delete_count;
				break;
			}
		}
	}

LABEL_ERROR:
	return delete_count;
}

static errno_t insert_quadtree_data(struct quadtree_t* quadtree, struct quadtree_node_t* node, void* value) {
	errno_t r = -1;
	struct quadtree_data_t* data;

	data = mpool_alloc(&quadtree->data_pool, MPOOL_RESIZE);
	if (data == NULL) {
		goto LABEL_ERROR;
	}

	data->value = value;

	data->next = node->data;
	node->data = data;

	r = 0;
LABEL_ERROR:
	return r;
}

static int insert_quadtree_node(struct quadtree_t* quadtree, struct quadtree_node_t* node, struct bbox2_t* bbox, void* data) {
	struct bbox2_t child_bbox;
	struct quadtree_node_t* child_node;
	int child;
	int insert_count;

	assert(node);

	insert_count = 0;

	if (node->bbox.x_max - node->bbox.x_min < quadtree->opt->min_size ||
		node->bbox.y_max - node->bbox.y_min < quadtree->opt->min_size)
	{
		/* the deepest level */

		if (insert_quadtree_data(quadtree, node, data)) {
			goto LABEL_ERROR;
		}

		++insert_count;
	} else {
		/* intersect */

		if (intersect_bbox2(bbox, &node->bbox)) {
			for (child = 0; child < QUADTREE_CHILD_COUNT; ++child) {
				child_node = node->children[child];

				if (child_node == NULL) {
					get_child_bbox(&child_bbox, &node->bbox, child);

					if (intersect_bbox2(bbox, &child_bbox)) {
						child_node = alloc_quadtree_node(&quadtree->node_pool, quadtree->opt->node_pool_alloc_type);
						if (child_node) {
							child_node->bbox = child_bbox;

							insert_count += insert_quadtree_node(quadtree, child_node, bbox, data);

							node->children[child] = child_node;
						}
					}
				} else {
					insert_count += insert_quadtree_node(quadtree, child_node, bbox, data);
				}
			}

			if (insert_count == 0) {
				/* not enought memory to alloc node */
				
				if (insert_quadtree_data(quadtree, node, data)) {
					goto LABEL_ERROR;
				}
		
				++insert_count;
			}
		}
	}

LABEL_ERROR:
	return insert_count;
}

static int child_node_count(struct quadtree_node_t* node) {
	int child;
	int count;

	count = 0;

	for (child = 0; child < QUADTREE_CHILD_COUNT; ++child) {
		if (node->children[child]) {
			++count;
		}
	}

	return count;
}

static int delete_quadtree_node(struct quadtree_t* quadtree, struct quadtree_node_t** node, struct bbox2_t* bbox, void* data) {
	int child;
	int delete_count;

	assert(node);

	delete_count = 0;

	if (intersect_bbox2(bbox, &(*node)->bbox)) {
		for (child = 0; child < QUADTREE_CHILD_COUNT; ++child) {
			if ((*node)->children[child] == NULL) {
				continue;
			}
			delete_count += delete_quadtree_node(quadtree, (*node)->children + child, bbox, data);
		}
	}

	delete_count += delete_quadtree_data(quadtree, *node, data);

	if (delete_count > 0) {
		if ((*node)->data == NULL && child_node_count(*node) == 0) {
			mpool_free(&quadtree->node_pool, *node);
			*node = NULL;
		}
	}

	return delete_count;
}

static void purge_quadtree_node(struct quadtree_t* quadtree, struct quadtree_node_t** node) {
	int child;

	assert(node);
	assert(*node != NULL);

	purge_quadtree_data(quadtree, *node);

	for (child = 0; child < QUADTREE_CHILD_COUNT; ++child) {
		if ((*node)->children[child] == NULL) {
			continue;
		}
		purge_quadtree_node(quadtree, (*node)->children + child);
	}

	assert((*node)->data == NULL);
	assert(child_node_count(*node) == 0);

	mpool_free(&quadtree->node_pool, *node);
	*node = NULL;
}

void purge_quadtree(struct quadtree_t* quadtree) {
	purge_quadtree_node(quadtree, &quadtree->root);
}

int insert_quadtree(struct quadtree_t* quadtree, struct bbox2_t* bbox, void* data) {
	int insert_count;

	insert_count = 0;

	if (quadtree->root == NULL) {
		if (init_root_bbox(quadtree)) {
			goto LABEL_ERROR;
		}

		++insert_count;
	}

	insert_count += insert_quadtree_node(quadtree, quadtree->root, bbox, data);

LABEL_ERROR:
	return insert_count;
}

int delete_quadtree(struct quadtree_t* quadtree, struct bbox2_t* bbox, void* data) {
	return delete_quadtree_node(quadtree, &quadtree->root, bbox, data);
}

struct quadtree_data_t* query_quadtree_node(struct quadtree_t* quadtree, struct quadtree_node_t* node, struct vector2_t* position) {
	struct quadtree_data_t* data;
	errno_t r = -1;
	struct quadtree_node_t* child_node;
	int child;

	if (node == NULL) {
		return NULL;
	}

	if (vector2_in_bbox2(position, &node->bbox)) {
		for (child = 0; child < QUADTREE_CHILD_COUNT; ++child) {
			child_node = node->children[child];
			if (child_node == NULL) {
				continue;
			}

			data = query_quadtree_node(quadtree, child_node, position);
			if (data == NULL) {
				continue;
			}
			
			return data;
		}

		return node->data;
	}

	return NULL;
}

struct quadtree_data_t* query_quadtree(struct quadtree_t* quadtree, struct vector2_t* position) {
	return query_quadtree_node(quadtree, quadtree->root, position);
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


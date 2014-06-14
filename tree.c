/*
** Born to code, die for bugs! 
*/
#include "tree.h"
#include "safe_free.h"

void insert_tree(struct tree_t* parent_tree, struct tree_t* tree) {
	struct tree_t* sibling;

	if (tree->parent) {
		remove_tree(tree->parent, tree);
	}

	if (parent_tree->first_child) {
		sibling = parent_tree->first_child;
		while (sibling->sibling != NULL) {
			sibling = sibling->sibling;
		}
		sibling->sibling = tree;
		tree->parent = sibling->parent;
	} else {
		parent_tree->first_child = tree;
		tree->parent = parent_tree;
	}
}

void remove_tree(struct tree_t* parent_tree, struct tree_t* tree) {
	struct tree_t* sibling;

	if (parent_tree->first_child == NULL)
		goto LABEL_ERROR;

	if (parent_tree->first_child == tree) {
		parent_tree->first_child = parent_tree->first_child->sibling;
	} else {
		sibling = parent_tree->first_child;
		while (sibling->sibling != NULL) {
			if (sibling->sibling == tree) {
				sibling->sibling = sibling->sibling->sibling;
				break;
			}
			sibling = sibling->sibling;
		}
	}

	tree->parent = NULL;
	tree->sibling = NULL;

LABEL_ERROR:
	return;
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


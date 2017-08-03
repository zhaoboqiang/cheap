/*
** Born to code, die for bugs!
*/
#include "path_stack.h"
#include <string.h>
#include <assert.h>

struct path_stack_t g_path_stack;

errno_t change_root_path(struct path_stack_t* path_stack, char const* root_path) {
	errno_t r = -1;
	size_t length;
	
	path_stack->level = 0;

	length = strlen(root_path);
	if (length >= PATH_BUFFER_LENGTH)
		goto LABEL_ERROR;

	path_stack->length[path_stack->level] = length;

	strcpy(path_stack->path, root_path);

	r = 0;
LABEL_ERROR:
	return r;
}

errno_t push_path(struct path_stack_t* path_stack, char const* dir) {
	errno_t r = -1;
	size_t up_level_length;
	size_t length;

	assert(path_stack->level < PATH_STACK_LEVEL);
	assert(strchr(dir, '/') == NULL);
	assert(strchr(dir, '\\') == NULL);

	length = strlen(dir);
	
	up_level_length = path_stack->length[path_stack->level];

	length += up_level_length + 1;
	if (length > PATH_BUFFER_LENGTH)
		goto LABEL_ERROR;

	strcpy(path_stack->path + up_level_length, dir);
	
	path_stack->path[up_level_length] = '/';
	path_stack->path[up_level_length + 1] = '\0';

	path_stack->length[++path_stack->level] = length;

	r = 0;
LABEL_ERROR:
	return r;
}

void pop_path(struct path_stack_t* path_stack) {
	assert(path_stack->level > 0);

	path_stack->path[path_stack->length[path_stack->level--]] = '\0';
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


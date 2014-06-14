/*
** Born to code, die for bugs!
*/

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

	enum { PATH_STACK_LEVEL = 4 };
	enum { PATH_BUFFER_LENGTH = 256 };

	struct path_stack_t {
		char path[PATH_BUFFER_LENGTH];

		size_t length[PATH_STACK_LEVEL];
		int level;
	};

	extern struct path_stack_t g_path_stack;

	errno_t change_root_path(struct path_stack_t* path_stack, char const* root_path);

	errno_t push_path(struct path_stack_t* path_stack, char const* dir);
	void pop_path(struct path_stack_t* path_stack);

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

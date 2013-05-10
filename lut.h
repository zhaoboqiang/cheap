/*
** Born to code, die for bugs! 
*/

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct lut_opt_t {
	size_t key_size;
	int (*key_cmp)(void const* lhs, void const* rhs);
};

struct lut_t {
	struct lut_opt_t* opt;

	size_t key_offset;
	size_t obj_size;
	size_t count;

	void* buffer;
};

errno_t init_lut(struct lut_t* lut);
void free_lut(struct lut_t* lut);

void* get_objs(struct lut_t* lut);

void sort_table(struct lut_t* lut);
void* lookup_table(struct lut_t* lut, void* obj);

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


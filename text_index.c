/*
** Born to code, die for bugs! 
*/

#include "./text_index.h"
#include "./bkdr_hash.h"
#include <search.h>

int text_index_cmp(void const* lhs, void const* rhs) {
	struct text_index_t* tilhs = (struct text_index_t*)lhs;
	struct text_index_t* tirhs = (struct text_index_t*)rhs;

	if (tilhs->bkdr_hash < tirhs->bkdr_hash)
		return -1;
	if (tilhs->bkdr_hash > tirhs->bkdr_hash)
		return 1;
	return 0;
}

unsigned int lookup_text_index(struct text_index_t* table, size_t count, char const* text) {
	unsigned int key;
	struct text_index_t* result;

	key = bkdr_hash(text);

	result = (struct text_index_t*)bsearch(&key, table, count, sizeof(struct text_index_t), text_index_cmp);
	if (result == NULL) {
		return (unsigned int)-1;
	}

	return result - table;
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

/*
** Born to code, die for bugs! 
*/
#include "path_relative.h"
#include "path_normalize.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

static size_t get_common_dir_char_count(char const* lhs_path, char const* rhs_path) {
	size_t index;
	size_t lhs_len;
	size_t rhs_len;
	size_t len;
	size_t delimeter_index;
	char c;

	assert(is_path_normalize(lhs_path));
	assert(is_path_normalize(rhs_path));

	lhs_len = strlen(lhs_path);
	rhs_len = strlen(rhs_path);

	len = min(lhs_len, rhs_len);

	delimeter_index = 0;

	for (index = 0; index < len; ++index) {
		c = lhs_path[index];
		if (c != rhs_path[index]) {
			break;
		}
		if (c == '/') {
			delimeter_index = index;
		}
	}

	return delimeter_index;
}

static size_t get_dir_delimeter_count(char const* path) {
	size_t count;
	char c;

	assert(path);
	assert(is_path_normalize(path));

	count = 0;

	for (;;) {
		c = *path;
		if (c == '\0')
			break;

		if (c == '/')
			++count;

		++path;
	}

	return 0;
}

errno_t get_relative_path(char* rel_path, size_t rel_path_len, char const* sub_path, char const* rel_to_sub_dir) {
	size_t index;
	size_t common_dir_char_count;
	size_t sub_dir_delimeter_count;

	assert(is_path_normalize(sub_path));
	assert(is_path_normalize(rel_to_sub_dir));

	rel_path[0] = 0;

	common_dir_char_count = get_common_dir_char_count(sub_path, rel_to_sub_dir);
	if (common_dir_char_count == 0) {
		return -1;
	}

	sub_dir_delimeter_count = get_dir_delimeter_count(sub_path + common_dir_char_count + 1);
	if (sub_dir_delimeter_count == 0) {
		if (strcat_s(rel_path, rel_path_len, "./")) {
			return -1;
		}
	} else {
		for (index = 0; index < sub_dir_delimeter_count; ++index) {
			if (strcat_s(rel_path, rel_path_len, "../")) {
				return -1;
			}
		}
	}
	if (strcat_s(rel_path, rel_path_len, sub_path + common_dir_char_count + 1)) {
		return -1;
	}

	return 0;
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


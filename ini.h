/*
** Born to code, die for bugs! 
** http://code.google.com/p/cheap/
** one pass scan, no string copy, two malloc, one file read
*/

#pragma once

#include "./buffer.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ini_kv_t {
	struct ini_kv_t* next;
	
	char const* key;
	char const* value;
};

struct ini_section_t {
	struct ini_section_t* next;

	char const* name;
	struct ini_kv_t* kv;
};

struct ini_t {
	struct ini_section_t* section;

	struct buffer_t rbuffer;
	struct buffer_t wbuffer;
};

errno_t init_ini(struct ini_t* ini);
void exit_ini(struct ini_t* ini);

errno_t load_ini(struct ini_t* ini, char const* filepath);

char const* get_value(struct ini_t* ini, char const* section, char const* key);

errno_t get_integer(struct ini_t* ini, char const* section, char const* key, int* value);
errno_t get_float(struct ini_t* ini, char const* section, char const* key, float* value);

errno_t get_multi_integer(struct ini_t* ini, char const* section, char const* key, int* value, int count);
errno_t get_multi_float(struct ini_t* ini, char const* section, char const* key, float* value, int count);

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


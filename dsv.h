/*
** Born to code, die for bugs! 
**
** delimiter-separated values
** two pass scan, no string copy, two malloc, one file read
** little endian
** title format: "name:type"
**	type:
**		name	desc		c memory layout
**		d		int32
**		u		uint32
**		f		float
**		date	yyyymmdd	[y16m8d8]
**		time	hh:mm:ss	[x8h8m8s8]
**		rgba	rrggbbaa	[b8g8r8a8]
**		path	p/p/f.e
**		url
**	others are string
*/

#pragma once

#include "./buffer.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

enum DSV_COLUMN_TYPE {
	DSV_COLUMN_TYPE_STRING,
	DSV_COLUMN_TYPE_INT,
	DSV_COLUMN_TYPE_UINT,
	DSV_COLUMN_TYPE_FLOAT,
	DSV_COLUMN_TYPE_TIME,
	DSV_COLUMN_TYPE_DATE,
	DSV_COLUMN_TYPE_RGBA,
	DSV_COLUMN_TYPE_PATH,
	DSV_COLUMN_TYPE_URL,
};

struct dsv_column_t {
	char const* name;
	int type;
};

struct dsv_opt_t {
	char delimiter;
};

struct dsv_t {
	struct dsv_opt_t* opt;

	size_t column;
	size_t row;
	
	struct dsv_column_t* title;
	void* data;

	struct buffer_t rbuffer;
	struct buffer_t wbuffer;
};

errno_t init_dsv(struct dsv_t* dsv);
void free_dsv(struct dsv_t* dsv);

errno_t load_dsv(struct dsv_t* dsv, char const* filepath);

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


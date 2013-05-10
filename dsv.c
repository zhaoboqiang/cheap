/*
** Born to code, die for bugs! 
** http://code.google.com/p/cheap/
*/

#include "./dsv.h"
#include "./split_text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

errno_t init_dsv(struct dsv_t* dsv) {
	assert(dsv->column == 0);
	assert(dsv->row == 0);
	assert(dsv->title == NULL);
	assert(dsv->data == NULL);

	return 0;
}

void exit_dsv(struct dsv_t* dsv) {
	free_buffer(&dsv->wbuffer);
	free_buffer(&dsv->rbuffer);
}

static void get_dsv_extent(size_t* cx_cell, size_t* cy_cell, char* text, size_t size, char delimiter) {
	size_t column = 0;
	size_t row = 0;
	size_t offset;
	char c;

	/* calc table column */
	for (offset = 0; offset < size; ++offset) {
		c = text[offset];

		/* escape sequences */
		if (c == '\\') {
			++offset;
			assert(offset < size);
			continue;
		}

		if (c == '\r' || c == '\n') {
			++column;
			break;
		}

		if (c == delimiter) {
			++column;
		}
	}

	/* calc row */
	for (offset = 0; offset < size;) {
		c = text[offset];

		/* escape sequences */
		if (c == '\\') {
			assert(offset + 1 < size);
			offset += 2;
			continue;
		}

		if (c == '\n' || c == '\0') {
			++row;
			++offset;
		} else if (c == '\r') {
			if (offset + 1 < size && text[offset + 1] == '\n') {
				offset += 2;
			} else {
				++offset;
			}

			++row;
		} else {
			++offset;
		}
	}

	if (size > 0) {
		c = text[size - 1];
		if (c != '\n' && c != '\r' && c != '\0') {
			++row;
		}
	}

	*cx_cell = column;
	*cy_cell = row;
}

static errno_t load_cell(struct dsv_t* dsv, char* text, size_t size, size_t column, size_t row) {
	errno_t r = -1;
	struct dsv_column_t* title;
	char* cell;
	unsigned long value[4];

	title = dsv->title + column;
	cell = (char*)dsv->data + row * dsv->column + column;

	switch (title->type)
	{
	case DSV_COLUMN_TYPE_STRING:
		cell = text;
		break;
	case DSV_COLUMN_TYPE_INT:
		*((int*)cell) = atoi(text);
		break;
	case DSV_COLUMN_TYPE_UINT:
		sscanf_s(text, "%u", cell);
		break;
	case DSV_COLUMN_TYPE_FLOAT:
		*((float*)cell) = (float)atof(text);
		break;
	case DSV_COLUMN_TYPE_TIME:
		assert(size == 9);
		if (sscanf_s(text, "%02u:%02u:%02u", value, value + 1, value + 2) != 3) {
			printf("[dsv] load time cell (r%d, c%d) failed. \"%s\"\n", column, row, text);
			goto LABEL_ERROR;
		}
		if (value[0] >= 24 || value[1] >= 60 || value[2] >= 60) {
			printf("[dsv] load time cell (r%d, c%d) failed. \"%s\"\n", column, row, text);
			goto LABEL_ERROR;
		}
		(*(unsigned long*)cell) = (value[2] | (value[1] << 8) | (value[0] << 16));
		break;
	case DSV_COLUMN_TYPE_DATE:
		assert(size == 9);
		if (sscanf_s(text, "%04u%02u%02u", value, value + 1, value + 2) != 3) {
			printf("[dsv] load date cell (r%d, c%d) failed. \"%s\"\n", column, row, text);
			goto LABEL_ERROR;
		}
		if (value[1] > 12 || value[2] > 31) {
			printf("[dsv] load date cell (r%d, c%d) failed. \"%s\"\n", column, row, text);
			goto LABEL_ERROR;
		}
		(*(unsigned long*)cell) = (value[2] | (value[1] << 8) | (value[0] << 16));
		break;
	case DSV_COLUMN_TYPE_RGBA:
		assert(size == 7 || size == 9);
		if (size == 9) {
			if (sscanf_s(text, "%02x%02x%02x%02x", value, value + 1, value + 2, value + 3) != 4) {
				printf("[dsv] load rgba cell (r%d, c%d) failed. \"%s\"\n", column, row, text);
				goto LABEL_ERROR;
			}
		} else if (size == 7) {
			if (sscanf_s(text, "%02x%02x%02x", value, value + 1, value + 2) != 3) {
				printf("[dsv] load rgba cell (r%d, c%d) failed. \"%s\"\n", column, row, text);
				goto LABEL_ERROR;
			}
			value[3] = 0xFF;
		} else {
			printf("[dsv] load rgba cell (r%d, c%d) failed. \"%s\"\n", column, row, text);
			goto LABEL_ERROR;
		}

		*((unsigned long*)cell) = ((value[0] << 16) | (value[1] << 8) | value[2] | (value[3] << 24));
		break;
	case DSV_COLUMN_TYPE_PATH:
		cell = text;
		break;
	case DSV_COLUMN_TYPE_URL:
		cell = text;
		break;
	default:
		assert(0);
		break;
	}

	r = 0;
LABEL_ERROR:
	return r;
}

static errno_t load_row(struct dsv_t* dsv, char* text, size_t size, size_t row) {
	errno_t r = -1;
	size_t column;
	size_t offset;
	size_t cell_length;
	size_t delimiter_length;

	column = 0;
	offset = 0;

	while (offset < size) {
		split_text(&cell_length, &delimiter_length, text + offset, size - offset, dsv->opt->delimiter);

		if (load_cell(dsv, text + offset, cell_length - delimiter_length + 1, column, row)) {
			goto LABEL_ERROR;
		}

		offset += cell_length;
		++column;
	}

	r = 0;
LABEL_ERROR:
	return r;
}

static errno_t load_title_cell(struct dsv_column_t* title_cell, char* text, size_t size) {
	errno_t r = -1;
	int type;

	if (size == 0) {
		printf("[dsv] does not support empty title.\n");
		goto LABEL_ERROR;
	}

	if (strstr(text, ":time")) {
		type = DSV_COLUMN_TYPE_TIME;
	} else if (strstr(text, ":date")) {
		type = DSV_COLUMN_TYPE_DATE;
	} else if (strstr(text, ":rgba")) {
		type = DSV_COLUMN_TYPE_RGBA;
	} else if (strstr(text, ":path")) {
		type = DSV_COLUMN_TYPE_PATH;
	} else if (strstr(text, ":url")) {
		type = DSV_COLUMN_TYPE_URL;
	} else if (strstr(text, ":d")) {
		type = DSV_COLUMN_TYPE_INT;
	} else if (strstr(text, ":u")) {
		type = DSV_COLUMN_TYPE_UINT;
	} else if (strstr(text, ":f")) {
		type = DSV_COLUMN_TYPE_FLOAT;
	} else {
		type = DSV_COLUMN_TYPE_STRING;
	}

	title_cell->name = text;
	title_cell->type = type;

	r = 0;
LABEL_ERROR:
	return r;
}

static errno_t load_title(struct dsv_t* dsv, char* text, size_t size) {
	errno_t r = -1;
	size_t offset;
	size_t cell_length;
	size_t delimiter_length;
	struct dsv_column_t* title;

	title = dsv->title;

	offset = 0;

	while (offset < size) {
		split_text(&cell_length, &delimiter_length, text + offset, size - offset, dsv->opt->delimiter);

		if (load_title_cell(title++, text + offset, cell_length - delimiter_length + 1)) {
			printf("[dsv] load title cell (c%d) failed. \"%s\"\n", title - dsv->title, text + offset);
			goto LABEL_ERROR;
		}

		offset += cell_length;
	}

	for (; title != dsv->title + dsv->column; ++title) {
		title->name = "";
		title->type = DSV_COLUMN_TYPE_STRING;
	}

	r = 0;
LABEL_ERROR:
	return r;
}

static errno_t load_dsv_text(struct dsv_t* dsv, char* text, size_t size) {
	errno_t r = -1;
	size_t offset;
	size_t delimiter_length;
	size_t row_length;
	size_t row;

	offset = 0;

	/* title */
	split_line(&row_length, &delimiter_length, text, size, dsv->opt->delimiter);

	if (load_title(dsv, text + offset, row_length - delimiter_length + 1)) {
		goto LABEL_ERROR;
	}

	offset += row_length;

	/* data */
	row = 0;

	while (offset < size) {
		split_line(&row_length, &delimiter_length, text + offset, size - offset, dsv->opt->delimiter);

		if (load_row(dsv, text + offset, row_length - delimiter_length + 1, row)) {
			goto LABEL_ERROR;
		}

		offset += row_length;
		++row;
	}

	r = 0;
LABEL_ERROR:
    if (r) {
		dsv->title = NULL;
		dsv->data = NULL;
    }
	return r;
}

errno_t load_dsv(struct dsv_t* dsv, char const* filepath) {
	errno_t r = -1;
	errno_t load_rbuffer_errno = -1;
	errno_t alloc_wbuffer_errno = -1;
	char* text;
	size_t size;

	assert(dsv->rbuffer.data == NULL);
	assert(dsv->rbuffer.size == 0);

	/* prepare */
	load_rbuffer_errno = load_buffer(&dsv->rbuffer, filepath);
	if (load_rbuffer_errno) {
		assert(0);
		goto LABEL_ERROR;
	}

	text = dsv->rbuffer.data;
	size = dsv->rbuffer.size;

	get_dsv_extent(&dsv->column, &dsv->row, text, size, dsv->opt->delimiter);

	assert(dsv->wbuffer.size == 0);
	assert(dsv->wbuffer.data == NULL);
	
	dsv->wbuffer.size = dsv->column * (dsv->row * sizeof(char*) + sizeof(struct dsv_column_t));

	alloc_wbuffer_errno = alloc_buffer(&dsv->wbuffer);
	if (alloc_wbuffer_errno) {
		assert(0);
		goto LABEL_ERROR;
	}

	/* load */
	dsv->title = dsv->wbuffer.data;
	dsv->data = dsv->title + dsv->column;

	if (load_dsv_text(dsv, text, size)) {
		goto LABEL_ERROR;
	}

	r = 0;
LABEL_ERROR:
	if (r) {
		if (load_rbuffer_errno == 0) {
			free_buffer(&dsv->rbuffer);
		}
		if (alloc_wbuffer_errno == 0) {
			free_buffer(&dsv->wbuffer);
		}
	}
	return r;
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


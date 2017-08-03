/*
** Born to code, die for bugs! 
*/

#include "ini.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

errno_t init_ini(struct ini_t* ini) {
	errno_t r = -1;
	errno_t alloc_wbuffer_errno = -1;

	assert(ini->section == NULL);
	assert(ini->rbuffer.size == 0);
	assert(ini->rbuffer.data == NULL);
	assert(ini->wbuffer.data == NULL);

	if (ini->wbuffer.size == 0) {
		goto LABEL_EXIT;
	}

	alloc_wbuffer_errno = alloc_buffer(&ini->wbuffer);
	if (alloc_wbuffer_errno) {
		assert(0);
		goto LABEL_ERROR;
	}

LABEL_EXIT:
	r = 0;
LABEL_ERROR:
	if (r) {
		if (alloc_wbuffer_errno == 0) {
			free_buffer(&ini->wbuffer);
		}
	}
	return r;
}

void free_ini(struct ini_t* ini) {
	free_buffer(&ini->wbuffer);
	free_buffer(&ini->rbuffer);
}

static struct ini_section_t* get_section(struct ini_t* ini, char const* section_name) {
	struct ini_section_t* ini_section;

	for (ini_section = ini->section; ini_section; ini_section = ini_section->next) {
		if (strcmp(ini_section->name, section_name) == 0) {
			return ini_section;
		}
	}

	return NULL;
}

static struct ini_kv_t* get_kv(struct ini_section_t* ini_section, char const* key_name) {
	struct ini_kv_t* ini_kv;

	for (ini_kv = ini_section->kv; ini_kv; ini_kv = ini_kv->next) {
		if (strcmp(ini_kv->key, key_name) == 0) {
			return ini_kv;
		}
	}

	return NULL;
}

static void append_kv(struct ini_section_t* ini_section, struct ini_kv_t* ini_kv) {
	struct ini_kv_t* tail;

	if (ini_section->kv == NULL) {
		ini_section->kv = ini_kv;
	} else {
		tail = ini_section->kv;
		while (tail->next) {
			tail = tail->next;
		}
		tail->next = ini_kv;
	}
}

static errno_t set_kv(struct ini_t* ini, struct ini_section_t* ini_section, char const* key, char const* value) {
	errno_t r = -1;
	struct ini_kv_t* ini_kv;

	ini_kv = get_kv(ini_section, key);
	if (ini_kv == NULL) {
		ini_kv = seek_buffer(&ini->wbuffer, sizeof(struct ini_kv_t));
		if (ini_kv == NULL) {
			goto LABEL_ERROR;
		}
		ini_kv->next = NULL;
		append_kv(ini_section, ini_kv);
	}

	ini_kv->key = key;
	ini_kv->value = value;

	r = 0;
LABEL_ERROR:
	return r;
}

static void append_section(struct ini_t* ini, struct ini_section_t* ini_section) {
	struct ini_section_t* tail;

	if (ini->section == NULL) {
		ini->section = ini_section;
	} else {
		tail = ini->section;
		while (tail->next) {
			tail = tail->next;
		}
		tail->next = ini_section;
	}
}

static struct ini_section_t* set_section(struct ini_t* ini, char const* section_name) {
	struct ini_section_t* ini_section;

	ini_section = get_section(ini, section_name);
	if (ini_section) {
		goto LABEL_EXIT;
	}

	ini_section = seek_buffer(&ini->wbuffer, sizeof(struct ini_section_t));
	if (ini_section == NULL) {
		goto LABEL_ERROR;
	}

	ini_section->next = NULL;
	append_section(ini, ini_section);

	ini_section->name = section_name;
	ini_section->kv = NULL;

LABEL_EXIT:
LABEL_ERROR:
	return ini_section;
}

static int is_key_char(char c) {
	if (c >= 'A' && c <= 'Z' ||
		c >= 'a' && c <= 'z' ||
		c >= '0' && c <= '9' ||
		c == '$' || c == '.' ||
		c == '_' || c == '-')
	{
		return 1;
	}

	return 0;
}

char* split_kv(char* kv) {
	char* delimiter;
	char* value;

	/* split key and value */
	delimiter = kv;
	while (*delimiter) {
		if (*delimiter == '=') {
			*delimiter = '\0';
			break;
		}
		++delimiter;
	}
	
	value = delimiter + 1;

	/* strip comment */
	delimiter = value;
	while (*delimiter) {
		/* escape sequences */
		if (*delimiter == '\\') {
			assert(delimiter[1] != '\0');
			delimiter += 2;
			continue;
		}
		if (*delimiter == ';' || *delimiter == '#') {
			*delimiter = '\0';
			break;
		}
		++delimiter;
	}
	
	return value;
}

static errno_t load_kv(struct ini_t* ini, struct ini_section_t* ini_section, char* text) {
	errno_t r = -1;
	char* value;

	value = split_kv(text);

	if (set_kv(ini, ini_section, text, value)) {
		goto LABEL_ERROR;
	}

	r = 0;
LABEL_ERROR:
	return r;
}

static struct ini_section_t* load_section(struct ini_t* ini, char* head, char* tail) {
	struct ini_section_t* ini_section;

	assert(head[0] == '[');

	++head;

	while (tail != head) {
		if (*tail == ']') {
			*tail = '\0';
			break;
		}

		--tail;
	}

	ini_section = set_section(ini, head);
	if (ini_section == NULL) {
		printf("[ini] add section [%s] error.\n", head);
		goto LABEL_ERROR;
	}

LABEL_ERROR:
	return ini_section;
}

static char* split_line(char* head, char* tail) {
	char* delimiter;

	if (head == tail) {
		return tail;
	}

	for (delimiter = head; delimiter != tail; ++delimiter) {
		if (*delimiter == '\r' || *delimiter == '\n') {
			break;
		}
	}

	if (delimiter != tail) {
		if (*delimiter == '\r') {
			*delimiter++ = '\0';
		} 
	}
	if (delimiter != tail) {
		if (*delimiter == '\n') {
			*delimiter++ = '\0';
		}
	}

	return delimiter;
}

errno_t load_ini(struct ini_t* ini, char const* filepath) {
	errno_t r = -1;
	errno_t load_rbuffer_errno = -1;
	errno_t alloc_wbuffer_errno = -1;
	struct ini_section_t* ini_section = NULL;
	char* head;
	char* tail;
	char* next;
	char c;

	assert(ini->rbuffer.data == NULL);
	assert(ini->rbuffer.size == 0);

	/* prepare */
	load_rbuffer_errno = load_buffer(&ini->rbuffer, filepath);
	if (load_rbuffer_errno) {
		assert(0);
		goto LABEL_ERROR;
	}

	if (ini->wbuffer.size == 0) {
		ini->wbuffer.size = ini->rbuffer.size * 2;

		assert(ini->wbuffer.data == NULL);

		alloc_wbuffer_errno = alloc_buffer(&ini->wbuffer);
		if (alloc_wbuffer_errno) {
			assert(0);
			goto LABEL_ERROR;
		}
	} else {
		assert(ini->wbuffer.size > 0);
		assert(ini->wbuffer.data);
	}

	/* load */
	next = ini->rbuffer.data;
	tail = next + ini->rbuffer.size;

	while (next != tail) {
		head = next;
		next = split_line(next, tail);

		c = *head;

		if (c == '[') {
			ini_section = load_section(ini, head, next);
			if (ini_section == NULL) {
				printf("[ini] load section failed.\n");
				goto LABEL_ERROR;
			}
		} else if (is_key_char(c)) {
			if (ini_section == NULL) {
				printf("[ini] section does not exist.\n");
				goto LABEL_ERROR;
			}

			if (load_kv(ini, ini_section, head)) {
				printf("[ini] load key value pair failed.\n");
				goto LABEL_ERROR;
			}
		}
	}

	r = 0;
LABEL_ERROR:
	if (r) {
		if (load_rbuffer_errno == 0) {
			free_buffer(&ini->rbuffer);
		}
		if (alloc_wbuffer_errno == 0) {
			free_buffer(&ini->wbuffer);
		}
	}
	return r;
}

char const* get_value(struct ini_t* ini, char const* section, char const* key) {
	struct ini_section_t* ini_section;
	struct ini_kv_t* ini_kv;

	ini_section = get_section(ini, section);
	if (ini_section == NULL) {
		return NULL;
	}

	ini_kv = get_kv(ini_section, key);
	if (ini_kv == NULL) {
		return NULL;
	}

	return ini_kv->value;
}

errno_t get_integer(struct ini_t* ini, char const* section, char const* key, int* value) {
	errno_t r = -1;
	char const* value_string;

	value_string = get_value(ini, section, key);
	if (value_string == NULL)
		goto LABEL_ERROR;

	*value = atoi(value_string);

	r = 0;
LABEL_ERROR:
	return r;
}

errno_t get_float(struct ini_t* ini, char const* section, char const* key, float* value) {
	errno_t r = -1;
	char const* value_string;

	value_string = get_value(ini, section, key);
	if (value_string == NULL)
		goto LABEL_ERROR;

	*value = (float)atof(value_string);

	r = 0;
LABEL_ERROR:
	return r;
}

errno_t get_multi_integer(struct ini_t* ini, char const* section, char const* key, int* value, int count) {
	errno_t r = -1;
	char const* value_string;

	if (count == 0) {
		goto LABEL_ERROR;
	}

	value_string = get_value(ini, section, key);
	if (value_string == NULL) {
		goto LABEL_ERROR;
	}

	do {
		*value++ = atoi(value_string);
		--count;

		value_string = strchr(value_string, ',');
		if (value_string) {
			++value_string;
		}
	} while (value_string && count > 0);

	while (count--) {
		*value++ = 0;
	}

	r = 0;
LABEL_ERROR:
	return r;
}

errno_t get_multi_float(struct ini_t* ini, char const* section, char const* key, float* value, int count) {
	errno_t r = -1;
	char const* value_string;

	if (count == 0) {
		goto LABEL_ERROR;
	}

	value_string = get_value(ini, section, key);
	if (value_string == NULL) {
		goto LABEL_ERROR;
	}

	do {
		*value++ = (float)atof(value_string);
		--count;

		value_string = strchr(value_string, ',');
		if (value_string) {
			++value_string;
		}
	} while (value_string && count > 0);

	while (count--) {
		*value++ = 0.0f;
	}

	r = 0;
LABEL_ERROR:
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


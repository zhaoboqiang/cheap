/*
** Born to code, die for bugs! 
*/
#include "./file_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>

void free_file_list(struct file_list_t* file_list) {
	struct file_node_t* item;
	struct file_node_t* free_item;

	for (item = file_list->head; item;) {
		free_item = item;
		item = item->next;
		free(free_item);
	}

	file_list->head = NULL;
	file_list->count = 0;
}

static errno_t push_head_file_list(struct file_list_t* file_list, char const* dir) {
	errno_t r = -1;
	struct file_node_t* item;
	size_t length;

	length = strlen(dir);

	item = malloc(sizeof(struct file_node_t) + length + 1);
	if (item == NULL)
		goto LABEL_ERROR;

	memcpy((char*)item + sizeof(struct file_node_t), dir, length + 1);

	item->next = file_list->head;
	file_list->head = item;
	
	++file_list->count;

	r = 0;
LABEL_ERROR:
	return r;
}

errno_t fill_file_list(struct file_list_t* file_list, char const* dir) {
	errno_t r = -1;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATAA FindFileData;
	char path[MAX_PATH];

	_snprintf_s(path, sizeof(path), _TRUNCATE, "%s\\*", dir);

	hFind = FindFirstFileA(path, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		goto LABEL_ERROR;

	do  {
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			_snprintf_s(path, sizeof(path), _TRUNCATE, "%s\\%s", dir, FindFileData.cFileName);
		
			if (strcmp(FindFileData.cFileName, ".") == 0)
				continue;

			if (strcmp(FindFileData.cFileName, "..") == 0)
				continue;

			if (fill_file_list(file_list, path)) {
				goto LABEL_ERROR;
			}
		} else {
			_snprintf_s(path, sizeof(path), _TRUNCATE, "%s\\%s", dir, FindFileData.cFileName);

			push_head_file_list(file_list, path);
		}
	}  while (FindNextFileA(hFind, &FindFileData));

	r = 0;
LABEL_ERROR:
	if (hFind != INVALID_HANDLE_VALUE) {
		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
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


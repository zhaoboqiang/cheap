/*
** Born to code, die for bugs! 
*/

#include "./mkdir.h"
#include <Shlwapi.h>
#include <string.h>

#pragma comment(lib, "Shlwapi.lib")

errno_t mkdir(wchar_t const* path) {
	errno_t r = -1;
	size_t length;
	wchar_t const* src;
	wchar_t* dst;
	wchar_t* dir = NULL;

	if (PathIsRelativeW(path))
		goto LABEL_ERROR;

	length = wcslen(path);
	if (length < 4)
		goto LABEL_ERROR;

	dir = malloc((length + 1) * sizeof(wchar_t));
	if (dir == NULL)
		goto LABEL_ERROR;

	src = path;
	dst = dir;

	while (*src) {
		if (*src == L'\\' || *src == L'/') {
			if (*(src - 1) != L':') {
				CreateDirectoryW(dir, NULL);
			}
		}
		*dst++ = *src++;
		*dst = L'\0';
	}

	CreateDirectoryW(dir, NULL);

	r = 0;
LABEL_ERROR:
	if (dir)
		free(dir);
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


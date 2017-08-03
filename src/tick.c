/*
** Born to code, die for bugs!
*/

#include "tick.h"

#if defined(__linux) || defined(__APPLE__)
#include <sys/time.h>
#endif

#ifdef _MSC_VER
#include <Windows.h>
#endif

static uint64_t g_startTick;

errno_t init_tick_count() {
	g_startTick = 0;
	g_startTick = get_tick_count();

	return 0;
}

uint32_t get_tick_count() {
#if defined(__linux) || defined(__APPLE__)
	uint64_t tick = 0;
	timeval timeVal;

	if (gettimeofday(&timeVal, NULL))
		goto LABEL_ERROR;

	tick = ((uint64_t)timeVal.tv_sec) * 1000 + timeVal.tv_usec / 1000;

LABEL_ERROR:
	return (uint32_t)tick;
#endif

#ifdef _MSC_VER
	return (uint32_t)(GetTickCount64() - g_startTick);
#endif
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


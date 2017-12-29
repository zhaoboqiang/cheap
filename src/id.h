/*
** Born to code, die for bugs! 
*/

#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define INDEX0_BITS_COUNT (2)
#define INDEX1_BITS_COUNT (12)
#define INDEX64_BITS_COUNT (6)

#define INDEX0_BITS_MASK (0x0001)
#define INDEX1_BITS_MASK (0x0FFF)
#define INDEX64_BITS_MASK (0x003F)

#define INDEX0_COUNT ((1 << INDEX0_BITS_COUNT))
#define INDEX1_COUNT ((1 << INDEX1_BITS_COUNT))
#define INDEX64_COUNT ((1 << INDEX64_BITS_COUNT))
#define INDEX_COUNT (INDEX0_COUNT* INDEX64_COUNT * INDEX1_COUNT * INDEX64_COUNT)

struct id
{
	uint64_t index0[INDEX0_COUNT];
	uint64_t index1[INDEX0_COUNT * INDEX64_COUNT][INDEX1_COUNT];
	uint32_t count1[INDEX0_COUNT * INDEX64_COUNT];
};

int alloc_id(struct id* id);
void free_id(struct id* id, int index);

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


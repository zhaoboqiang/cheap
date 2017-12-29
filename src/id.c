/*
** Born to code, die for bugs! 
*/
#include "id.h"

static int find_empty_index(uint64_t const* index_table, int const count)
{
	for (int i = 0; i < count; ++i)
	{
		uint64_t const index = index_table[i];

		if (index != 0xFFFFFFFFFFFFFFFFull)
		{
			for (int j = 0; j < INDEX64_COUNT; ++j)
			{
				uint64_t const test_mask = 0x0000000000000001ull << j;
				if ((index & test_mask) == 0)
				{
					return i * INDEX64_COUNT + j;
				}
			}
		}
	}

	return -1;
}

static void set(uint64_t* index_table, int const count, int const index)
{
	int const i = index >> INDEX64_BITS_COUNT;
	int const j = index & INDEX64_BITS_MASK;

	uint64_t const test_mask = 0x0000000000000001ull << j;

	assert(i < count);
	assert((index_table[i] & test_mask) == 0);

	index_table[i] |= test_mask;
}

static void reset(uint64_t* index_table, int const count, int const index)
{
	int const i = index >> INDEX64_BITS_COUNT;
	int const j = index & INDEX64_BITS_MASK;

	uint64_t const test_mask = 0x0000000000000001ull << j;

	assert(i < count);
	
	if (index_table[i] & test_mask) {
		index_table[i] &= ~test_mask;
	}
}

int alloc_id(struct id* id)
{
	int const index0 = find_empty_index(id->index0, _countof(id->index0));
	if (index0 == -1)
		return -1;

	int const index1 = find_empty_index(id->index1[index0], _countof(id->index1[index0]));
	
	assert(index1 != -1);

	set(id->index1[index0], INDEX1_COUNT, index1);
	
	if (++id->count1[index0] == INDEX1_COUNT * INDEX64_COUNT)
		set(id->index0, _countof(id->index0), index0);
	
	return index0 << (INDEX1_BITS_COUNT + INDEX64_BITS_COUNT) | index1;
}

void free_id(struct id* id, int index)
{
	assert(index >= 0 && index < INDEX_COUNT);

	int const index0 = (index >> (INDEX1_BITS_COUNT + INDEX64_BITS_COUNT));
	int const index1 = index & ((INDEX1_BITS_MASK << INDEX64_BITS_COUNT) | INDEX64_BITS_MASK);

	reset(id->index1[index0], _countof(id->index1[index0]), index1);
	
	if (--id->count1[index0] == 0) {
		reset(id->index0, _countof(id->index0), index0);
	}
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


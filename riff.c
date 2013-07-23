/*
** Born to code, die for bugs! 
*/

#include "./riff.h"
#include "./rbuffer.h"

errno_t seek_riff_chunk(struct rbuffer_t* riff_rbuffer, struct rbuffer_t const* chunk_rbuffer, unsigned long id) {
	errno_t r = -1;
	struct riff_chunk_t* riff_chunk;
	size_t riff_chunk_size;
	struct rbuffer_t rbuffer;

	rbuffer = *chunk_rbuffer;

    for (;;) {
		if (rbuffer.size < sizeof(struct riff_chunk_t)) {
			goto LABEL_ERROR;
		}

		riff_chunk = seek_rbuffer(&rbuffer, sizeof(struct riff_chunk_t));

		if (riff_chunk->id == id) {
            break;
		}

		riff_chunk_size = riff_chunk->size;
		if (riff_chunk_size & 0x1) {
			++riff_chunk_size;
		}

		if (riff_chunk_size == 0) {
			goto LABEL_ERROR;
		}

		if (rbuffer.size < riff_chunk_size) {
			goto LABEL_ERROR;
		}

		seek_rbuffer(&rbuffer, riff_chunk_size);
    }

	riff_rbuffer->data = rbuffer.data;
	riff_rbuffer->size = riff_chunk->size;

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


/*
** Born to code, die for bugs! 
*/

#include "./mtcall.h"
#include <process.h>
#include <assert.h>

struct mtcall_thread_process_context_t {
	struct mtcall_t* mtcall;
	int index;
};

static unsigned int __stdcall mtcall_thread_process(void* arg) {
	DWORD result;
	int index;
	struct mtcall_thread_process_context_t* mtcall_thread_process_context;
	struct mtcall_t* mtcall;
	struct mtcall_opt_t* opt;

	mtcall_thread_process_context = arg;

	mtcall = mtcall_thread_process_context->mtcall;
	index = mtcall_thread_process_context->index;

	free(mtcall_thread_process_context);

	opt = mtcall->opt;

	while (mtcall->loop) {
		result = WaitForSingleObject(mtcall->event_table[index], INFINITE);
		switch (result)
		{
		case WAIT_OBJECT_0:
			break;
		default:
			assert(0);
			return 0;
		}

		if (mtcall->loop) {
			(*opt->async_call)(opt->userdata, index);

			InterlockedIncrement(&mtcall->done_count);
		}
	}

	return 0;
}

errno_t init_mtcall(struct mtcall_t* mtcall) {
	errno_t r = -1;
	int i = 0;
    unsigned int thread_id;
	struct mtcall_opt_t* opt;
	struct mtcall_thread_process_context_t* mtcall_thread_process_context = NULL;
	HANDLE* thread_table = NULL;
	HANDLE* event_table = NULL;
	HANDLE handle;

	opt = mtcall->opt;
	if (opt == NULL)
		goto LABEL_ERROR;

	thread_table = malloc(opt->core_count * sizeof(HANDLE));
	if (thread_table == NULL)
		goto LABEL_ERROR;

	event_table = malloc(opt->core_count * sizeof(HANDLE));
	if (event_table == NULL)
		goto LABEL_ERROR;

	memset(thread_table, 0, opt->core_count * sizeof(HANDLE));
	memset(event_table, 0, opt->core_count * sizeof(HANDLE));

	mtcall->thread_table = thread_table;
	thread_table = NULL;

	mtcall->event_table = event_table;
	event_table = NULL;

	mtcall->loop = 1;
	mtcall->done_count = 0;

	for (i = 0; i < opt->core_count; ++i) {
		handle = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (handle == NULL) {
			goto LABEL_ERROR;
		}

		mtcall->event_table[i] = handle;

		mtcall_thread_process_context = malloc(sizeof(struct mtcall_thread_process_context_t));
		if (mtcall_thread_process_context == NULL) {
			goto LABEL_ERROR;
		}

		mtcall_thread_process_context->mtcall = mtcall;
		mtcall_thread_process_context->index = i;

		handle = (HANDLE)_beginthreadex(NULL, 256 * 1024, mtcall_thread_process, mtcall_thread_process_context, 0, &thread_id);
		if (handle == NULL) {
			goto LABEL_ERROR;
		}

		mtcall->thread_table[i] = handle;
		mtcall_thread_process_context = NULL;
	}

	r = 0;
LABEL_ERROR:
	if (r) {
		if (mtcall_thread_process_context) {
			free(mtcall_thread_process_context);
		}

		mtcall->loop = 0;

		if (mtcall->thread_table) {
			WaitForMultipleObjects(i, mtcall->thread_table, TRUE, INFINITE);
		}
	
		if (mtcall->event_table) {
			for (i = 0; i < opt->core_count; ++i) {
				CloseHandle(mtcall->event_table[i]);
			}
		}
	}
	if (thread_table) {
		free(thread_table);
	}
	if (event_table) {
		free(event_table);
	}
	return r;
}

void free_mtcall(struct mtcall_t* mtcall) {
	int i;
	struct mtcall_opt_t* opt;

	opt = mtcall->opt;

	mtcall->loop = 0;

	for (i = 0; i < opt->core_count; ++i) {
		SetEvent(mtcall->event_table[i]);
	}

	WaitForMultipleObjects(opt->core_count, mtcall->thread_table, TRUE, INFINITE);

	for (i = 0; i < opt->core_count; ++i) {
		CloseHandle(mtcall->event_table[i]);
	}

	free(mtcall->thread_table);
	free(mtcall->event_table);
}

errno_t mtemit(struct mtcall_t* mtcall) {
	int i;
	struct mtcall_opt_t* opt;

	opt = mtcall->opt;

	assert(mtcall->done_count == 0);

	for (i = 0; i < opt->core_count; ++i) {
		SetEvent(mtcall->event_table[i]);
	}

	return 0;
}

errno_t mtdone(struct mtcall_t* mtcall) {
	errno_t r = -1;
	struct mtcall_opt_t* opt;

	opt = mtcall->opt;

	if (mtcall->done_count != opt->core_count) {
		goto LABEL_ERROR;
	}

	InterlockedExchange(&mtcall->done_count, 0);

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


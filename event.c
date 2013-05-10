/*
** Born to code, die for bugs! 
*/

#include "./event.h"
#include <assert.h>

void hook_event(struct event_source_t* event_source, struct event_receiver_t* receiver) {
#ifndef NDEBUG
	struct event_receiver_t* current;

	for (current = event_source->head; current; current = current->next) {
		assert(current != receiver);
	}
#endif

	receiver->next = event_source->head;
	event_source->head = receiver;
}

void unhook_event(struct event_source_t* event_source, struct event_receiver_t* receiver) {
	struct event_receiver_t* current;
	struct event_receiver_t* previous;

    if (event_source->head == receiver) {
		event_source->head = receiver->next;
        return;
    }

    previous = event_source->head;

    for (current = previous->next; current; current = current->next) {
        if (current == receiver) {
            previous->next = current->next;
            return;
        }
        previous = current;
    }
}

void raise_event(struct event_source_t* event_source, void* source, void* args) {
	struct event_receiver_t* current;

	for (current = event_source->head; current; current = current->next) {
		(*current->handler)(source, current, args);
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


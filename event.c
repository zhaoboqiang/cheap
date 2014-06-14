/*
** Born to code, die for bugs!
*/
#include "event.h"
#include <assert.h>

void hook_event(struct event_t* event, struct event_receiver_t* receiver) {
#ifndef NDEBUG
	struct event_receiver_t* current;

	for (current = event->receiver; current; current = current->next) {
		assert(current != receiver);
	}
#endif

	receiver->next = event->receiver;
	event->receiver = receiver;
}

void unhook_event(struct event_t* event, struct event_receiver_t* receiver) {
	struct event_receiver_t* current;
	struct event_receiver_t* previous;

	if (event->receiver == receiver) {
		event->receiver = receiver->next;
		return;
	}

	previous = event->receiver;

	for (current = previous->next; current; current = current->next) {
		if (current == receiver) {
			previous->next = current->next;
			return;
		}
		previous = current;
	}
}

void raise_event(struct event_t* event, void* source, void* args) {
	struct event_receiver_t* receiver;

	for (receiver = event->receiver; receiver; receiver = receiver->next) {
		(*receiver->handler)(source, receiver, args);
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


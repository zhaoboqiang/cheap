/*
** Born to code, die for bugs! 
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


struct event_receiver_t {
	struct event_receiver_t* next;

	void (*handler)(void* source, struct event_receiver_t* receiver, void* args);
};

struct event_source_t {
	struct event_receiver_t* head;
};

void hook_event(struct event_source_t* event_source, struct event_receiver_t* receiver);
void unhook_event(struct event_source_t* event_source, struct event_receiver_t* receiver);

void raise_event(struct event_source_t* event_source, void* source, void* args);

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


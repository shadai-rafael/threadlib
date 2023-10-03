/*Copyright (c) 2021 Shadai Rafael Lopez Garcia
*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in all
*copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*SOFTWARE.
*/
#ifndef _THREADLIB_
#define _THREADLIB_

#include <pthread.h>
#include <stdbool.h>

#define NAME_SIZE 32

typedef struct thread_{
    char name[NAME_SIZE];
    bool thread_created;
    pthread_t thread;
    void * arg;
    void* (thread_fn)(void*);
    pthread_attr_t attributes;
}thread_t;

thread_t * thread_create(thread_t * thread, char * name);
void thread_run(thread_t * thread, void* (thread_fn)(void*), void * arg);
void thread_set_attribute_joinable_or_detached(thread_t * thread, bool joinable);

#endif //_THREADLIB_
/*Copyright (c) 2023 Shadai Rafael Lopez Garcia
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

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "threadlib.h"

thread_t * thread_create(thread_t * thread, char * name)
{
    if(!thread){
        thread = calloc(1 ,sizeof(thread_t));
    }
    thread->thread_created = false;
    thread->arg = NULL;
    memcpy(thread->name, name, NAME_SIZE);
    pthread_attr_init(&thread->attributes);
    thread->thread_fn = NULL;

    return thread;
}

void thread_run(thread_t * thread, void* (thread_fn)(void*), void * arg)
{
    pthread_create(&thread->thread,&thread->attributes,thread_fn,arg);

}

void thread_set_attribute_joinable_or_detached(thread_t * thread, bool joinable)
{
    pthread_attr_setdetachstate(&thread->attributes, 
        joinable ? PTHREAD_CREATE_JOINABLE:PTHREAD_CREATE_DETACHED);
}
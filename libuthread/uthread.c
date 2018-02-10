
#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"



//GLOBALS
int numThreads;
struct TCB* mainThread;
struct TCB* currentThread;

queue_t tQeeue;
bool init=false;
struct TCB {
    uthread_t tid;
    uthread_ctx_t* context;
    // change bools to running 0, ready 1, block 2, zombie 3;
    int state;
    bool joined;
    int ret;
};
static int find_next_ready(queue_t q, void* data, void* arg);
static int find_tid(queue_t q, void* data, void* arg);

static int find_next_ready(queue_t q, void* data, void* arg)
{
    struct TCB* tcb = (struct TCB*)data;
    if (tcb->state == 1)
        return 1;
    return 0;
}
static int find_tid(queue_t q, void* data, void* arg)
{
    struct TCB* tcb = (struct TCB*)data;
    uthread_t tid = *(uthread_t*)arg;

    if (tcb->tid == tid)
        return tid;
    return -1;
}
//todo change structure accordingly

int initializeMain(struct TCB* tcb, bool running, bool ready, uthread_func_t func, void *arg, bool init, bool* err)
{
    if (tcb == NULL)
        return -1;

    tcb->context = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
    if(getcontext(tcb->context))
    {
        *err = true;
        return -1;
    }
    tcb->state = 0;
    tcb->tid = 0;
    tcb->joined = false;
    tcb->ret = 0;
    return 1;
}

struct TCB* tcb_initialize(struct TCB* tcb, uthread_t id, bool running, bool ready, uthread_func_t func, void *arg, bool init)
{
    if (tcb == NULL)
        return NULL;

    tcb->context = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
    //todo change to case

    void* stack = uthread_ctx_alloc_stack();
    if (uthread_ctx_init(tcb->context, stack, func, arg)) {
        return NULL;
    }

    tcb->state = 1;
    tcb->tid = id;
    tcb->joined = false;
    tcb->ret = 0;
    numThreads++;

    return tcb;
}

void uthread_yield(void)
{

    struct TCB* next_ready;

    if (mainThread->state == 1)
        next_ready = mainThread;
    else{
        queue_iterate(tQeeue, find_next_ready, (void*)NULL, (void**)&next_ready);
    }

    if(next_ready != NULL) {
        uthread_ctx_t *prev = currentThread->context;
        uthread_ctx_t *next = next_ready->context;

        next_ready->state = 0;
        currentThread = next_ready;

        uthread_ctx_switch(prev, next);
        /* TODO Phase 2 */
    }
}

uthread_t uthread_self(void)
{
    return currentThread->tid;
}



int uthread_create(uthread_func_t func, void *arg)
{
    if (!init) {
        numThreads = 1;
        bool err = false;
        mainThread = (struct TCB*)malloc(sizeof(struct TCB));
        if(initializeMain(mainThread, true, false, NULL, NULL, true, &err)==-1)
            return -1;
        currentThread = mainThread;
        tQeeue = queue_create();
    }
    struct TCB* new_tcb = (struct TCB*)malloc(sizeof(struct TCB));
    if(tcb_initialize(new_tcb, numThreads, false, true, func, arg, false)==NULL)
        return -1;

    if (queue_enqueue(tQeeue, (void*)new_tcb))
        return -1;


    return numThreads - 1;

}




void uthread_exit(int retval)
{
    currentThread->state = 3;
    currentThread->ret = retval;

    uthread_yield();

}

int uthread_join(uthread_t tid, int *retval)
{

    struct TCB* tcb = NULL;
    queue_iterate(tQeeue, find_tid, (void*)&tid, (void**)&tcb);

    if (tcb != NULL) {
        if (tid < 1 || tid == currentThread->tid || tcb->joined)
            return -1;
    }else{
        return -1;
    }

    if (tcb->state == 3) {
        queue_delete(tQeeue, tcb);
        return 0;
    }

    if (tcb->state==1) {
        tcb->joined = true;
        uthread_yield();
        queue_delete(tQeeue, tcb);
        return 0;
    }

    return 0;
}


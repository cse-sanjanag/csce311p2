/* scheduler.c — Cooperative threading system
   -------------------------------------------
   Threads are simple stack+context structures.
   Each has callee-saved registers + RA and SP.
   We yield explicitly — no timer interrupts yet.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define MAX_THREADS 8
#define STACK_SIZE 4096

typedef void (*thread_fn)(void*);

struct thread {
    int used;
    uint64_t ctx[14];     // s0–s11, ra, sp
    char stack[STACK_SIZE] __attribute__((aligned(16)));
    thread_fn fn;
    void *arg;
};

static struct thread threads[MAX_THREADS];
static int current = -1;

/* Provided by intr.S */
extern void context_switch(void *old_ctx, void *new_ctx);

/* Initialize thread array */
void thread_init(void) {
    memset(threads, 0, sizeof(threads));
    current = -1;
}

/* This function runs inside the new thread's context */
void thread_trampoline_c(void) {
    int idx = current;
    struct thread *t = &threads[idx];
    t->fn(t->arg);

    /* Thread exits here */
    t->used = 0;
    while (1) {
        extern void thread_yield(void);
        thread_yield();
    }
}

/* Create a new thread by setting its initial context */
int thread_create(thread_fn fn, void *arg) {
    for (int i=0;i<MAX_THREADS;i++) {
        if (!threads[i].used) {
            threads[i].used = 1;
            threads[i].fn = fn;
            threads[i].arg = arg;

            memset(threads[i].ctx, 0, sizeof(threads[i].ctx));

            /* RA = thread trampoline */
            threads[i].ctx[12] = (uint64_t)thread_trampoline_c;

            /* SP = stack top */
            threads[i].ctx[13] = (uint64_t)&threads[i].stack[STACK_SIZE - 16];

            return i;
        }
    }
    return -1;
}

/* Get next runnable thread */
static int next_thread(int cur) {
    int start = (cur + 1) % MAX_THREADS;
    for (int i=0;i<MAX_THREADS;i++) {
        int idx = (start + i) % MAX_THREADS;
        if (threads[idx].used)
            return idx;
    }
    return -1;
}

/* Start scheduling by context-switching to first thread */
void thread_start_scheduler(void) {
    int first = next_thread(-1);
    if (first < 0) return;

    current = first;

    uint64_t dummy[14] = {0};
    context_switch(dummy, threads[first].ctx);
}

/* Explicit yield between threads */
void thread_yield(void) {
    int old = current;
    int nxt = next_thread(old);
    if (nxt < 0 || nxt == old) return;

    current = nxt;
    context_switch(threads[old].ctx, threads[nxt].ctx);
}

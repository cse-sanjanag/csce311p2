/* kernel.c — Main kernel entry, shell, and demo programs
   -------------------------------------------------------
   Initializes:
     • Console
     • Filesystem
     • Scheduler
   Then creates the shell task and enters scheduler.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

void console_putc(char);
void console_puts(const char*);
int  console_getc(void);

void fs_init(void);
int fs_count(void);
const struct file_entry* fs_get_by_index(int);
const struct file_entry* fs_find(const char *);

void thread_init(void);
int  thread_create(void (*fn)(void*), void*);
void thread_start_scheduler(void);
void thread_yield(void);

static void kputs(const char *s){ console_puts(s); }

void shell_task(void *arg) {
    char buf[128];
    int pos = 0;

    kputs("riscv-os> ");

    while (1) {
        int c = console_getc();
        if (c <= 0) continue;

        if (c == '\n' || c == '\r') {
            buf[pos] = 0;
            kputs("\n");

            if (strcmp(buf, "help") == 0)
                kputs("help ls cat run reboot\n");

            else if (strcmp(buf, "ls") == 0) {
                for (int i=0;i<fs_count();i++) {
                    const struct file_entry *fe = fs_get_by_index(i);
                    kputs(fe->name);
                    kputs("\n");
                }
            }

            else if (!strncmp(buf, "cat ", 4)) {
                const struct file_entry *fe = fs_find(&buf[4]);
                if (!fe) kputs("not found\n");
                else {
                    for (uint32_t i=0;i<fe->size;i++)
                        console_putc(fe->data[i]);
                    kputs("\n");
                }
            }

            else if (!strncmp(buf, "run ", 4)) {
                const char *p = &buf[4];
                if (!strcmp(p, "hello")) {
                    extern void user_hello(void*);
                    thread_create(user_hello, NULL);
                } else if (!strcmp(p, "counter")) {
                    extern void user_counter(void*);
                    thread_create(user_counter, NULL);
                } else {
                    kputs("unknown program (use hello|counter)\n");
                }
            }

            else if (!strcmp(buf, "reboot"))
                kputs("reboot not implemented\n");

            else if (pos > 0)
                kputs("unknown\n");

            pos = 0;
            kputs("riscv-os> ");
        }
        else {
            if (pos < 127) {
                buf[pos++] = c;
                console_putc(c);
            }
        }
    }
}

void user_hello(void *arg) {
    for (int i=0;i<5;i++) {
        kputs("Hello from user program!\n");
        for (volatile int j=0;j<200000;j++);
        thread_yield();
    }
}

void user_counter(void *arg) {
    for (int i=0;i<10;i++) {
        char msg[32];
        int len = snprintf(msg, sizeof(msg), "counter: %d\n", i);
        console_puts(msg);
        for (volatile int j=0;j<200000;j++);
        thread_yield();
    }
}

int main(void) {
    kputs("riscv-os: booting...\n");

    fs_init();
    thread_init();

    thread_create(shell_task, NULL);
    thread_start_scheduler();

    for (;;);
}

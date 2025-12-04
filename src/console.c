/* console.c — Console I/O using SBI (Supervisor Binary Interface)
   ---------------------------------------------------------------
   QEMU's built-in firmware (OpenSBI) exposes ECALL IDs for printing
   characters and reading keyboard input from nographic console.
*/

#include <stdint.h>

static inline intptr_t sbi_call(int id, intptr_t arg0, intptr_t arg1, intptr_t arg2) {
    register intptr_t a0 __asm__("a0") = arg0;
    register intptr_t a1 __asm__("a1") = arg1;
    register intptr_t a2 __asm__("a2") = arg2;
    register intptr_t a7 __asm__("a7") = id;
    __asm__ volatile("ecall"
        : "+r"(a0), "+r"(a1), "+r"(a2)
        : "r"(a7)
        : "memory");
    return a0;
}

/* Print one character */
void console_putc(char c) {
    sbi_call(1, c, 0, 0);   // SBI legacy console_putchar
}

/* Read one char (-1 if none) */
int console_getc(void) {
    return sbi_call(2, 0, 0, 0);  // SBI legacy console_getchar
}

/* Print entire string */
void console_puts(const char *s) {
    while (*s) console_putc(*s++);
}

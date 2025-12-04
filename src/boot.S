/* boot.S — kernel entry point
   ----------------------------
   QEMU jumps here when booting our kernel.
   We set up a stack pointer and call main().
*/

    .section .text
    .globl _start
_start:
    # Load address of kernel stack top into sp
    la sp, kernel_stack_top

    # Call C kernel main()
    call main

    # If main ever returns, halt with WFI loop
halt:
    wfi
    j halt

# Reserve kernel stack in BSS
    .section .bss
    .align 16
kernel_stack:
    .space 0x4000        # 16 KB stack
kernel_stack_top:

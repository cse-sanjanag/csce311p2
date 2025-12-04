/* intr.S — Context switching assembly for cooperative threads
   ------------------------------------------------------------
   context_switch(old, new):
     Saves s0–s11, ra, sp to *old
     Loads s0–s11, ra, sp from *new
*/

    .text
    .globl context_switch
context_switch:
    # a0 = old ctx ptr
    # a1 = new ctx ptr

    # Save callee-saved registers into old context
    sd s0, 0(a0)
    sd s1, 8(a0)
    sd s2, 16(a0)
    sd s3, 24(a0)
    sd s4, 32(a0)
    sd s5, 40(a0)
    sd s6, 48(a0)
    sd s7, 56(a0)
    sd s8, 64(a0)
    sd s9, 72(a0)
    sd s10, 80(a0)
    sd s11, 88(a0)
    sd ra, 96(a0)
    sd sp, 104(a0)

    # Load new thread's registers
    ld s0, 0(a1)
    ld s1, 8(a1)
    ld s2, 16(a1)
    ld s3, 24(a1)
    ld s4, 32(a1)
    ld s5, 40(a1)
    ld s6, 48(a1)
    ld s7, 56(a1)
    ld s8, 64(a1)
    ld s9, 72(a1)
    ld s10, 80(a1)
    ld s11, 88(a1)
    ld ra, 96(a1)
    ld sp, 104(a1)

    ret

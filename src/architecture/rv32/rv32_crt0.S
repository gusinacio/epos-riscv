# EPOS RISC-V Program Starter

        .file "rv32_crt0.S"

        # Entry point called by SETUP for processors
        .section .text
        .align  4
        .global _start
        .type   _start, function
_start:
        # Reading mhhartid
        csrr t0, mhartid
        bnez t0, 2f
        csrw satp, zero

        .option push
        .option norelax

        la gp, __global_pointer$

        .option pop

        # Load _bss limits to a0 and a1 respectively
        la a0, __bss_start
        la a1, _end

        bgeu a0, a1, 2f  # branch if a0 >= a1
1:
        # Set all bytes in the BSS section to zero.
        sw   zero, (a0)
        addi a0, a0, 4
        bltu a0, a1, 1b  # branch if a0 < a1

2:
        # Temporary stack(s) for INIT are created and configure by SETUP
        la sp, __boot_stack__

        li t0, (0b11 << 11) | (1 << 7) | (1 << 3)  # mask for machine mode (interruptions?)
        csrw mstatus, t0

        la t1, _init
        csrw mepc, t1

        la t2, _vector_table
        csrw mtvec, t2

        la ra, 3f

        # Call _init
        #call      _init
        mret

        # Call main
        .align  4
        .globl  __epos_app_entry
        .type   __epos_app_entry, function

3:
        wfi
        j 3b

__epos_app_entry:
        call      main
        lw        a0, 0(sp)         # save main's return value to be used by exit()
        call      _fini
        call      _exit

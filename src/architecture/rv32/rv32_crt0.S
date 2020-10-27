# EPOS RISC-V Program Starter

        .file "rv32_crt0.S"

        .option norvc
        .section .text
        .align  4
        .global _start
        .type   _start, function

_start:
        # Reading mhhartid
        csrr t0, mhartid
        bnez t0, 2f

        # Assures satp register initialized to zero
        csrw satp, zero

        .option push
        .option norelax

        # Initialize global pointer
	la  gp, __global_pointer$

        .option pop

        # Gets the bss section limits
        la     a0, __bss_start
        la     a1, _end
	bgeu   a0, a1, 2f
1:
        # Write zero to .bss section
        sw	zero, (a0)
	addi	a0, a0, 4
	bltu	a0, a1, 1b

2:
        # Initialize stack pointer for boot
        la   sp, __boot_stack__

        # Initialize mstatus in machine mode
        #li t0, (0b11 << 11) | (1 << 7) | (1 << 3)
        li     t0, (0b11 << 11)
        csrw   mstatus, t0

        # Initialize mepc register
        la     t1, _init
        csrw   mepc, t1

        # Initialize mtvec register
        la     t2, _vector_table
        csrw   mtvec, t2

        # Call _init
        call _init

        # Call main
        .align  4
        .globl  __epos_app_entry
        .type   __epos_app_entry, function

__epos_app_entry:
        call      main
        lw        a0, 0(sp)         # save main's return value to be used by exit()
        call      _fini
        call      _exit

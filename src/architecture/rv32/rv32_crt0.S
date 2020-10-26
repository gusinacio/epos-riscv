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
        csrw satp, zero

        csrr t0, mhartid # reading CSR mhartid
	bnez t0, 2f

        .option push
        .option norelax
        
	la  gp, __global_pointer$

        .option pop

        la a0, __bss_start
        la a1, _end
	bgeu a0, a1, 2f
1:
        sw	zero, (a0)
	addi	a0, a0, 4
	bltu	a0, a1, 1b

2:
        # Set all bytes in the BSS section to zero.
        la   sp, __boot_stack__
        
        li t0, (0b11 << 11) | (1 << 7) | (1 << 3)  # mask for machine mode (interruptions?)
        csrw mstatus, t0

        la t1, _init
        csrw mepc, t1

        la t2, _vector_table
        csrw mtvec, t2

        la ra, 3f
        # implement

        # Call _init
        # call _init
        mret

        # Call main
        .align  4
        .globl  __epos_app_entry
        .type   __epos_app_entry, function
3:
        wfi
	j 4b
__epos_app_entry:
        call      main
        lw        a0, 0(sp)         #save main's return value to be used by exit()
        call      _fini
        call      _exit
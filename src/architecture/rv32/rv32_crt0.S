# EPOS RISC-V Program Starter

        .file "rv32_crt0.S"

        .option norvc
        .section .text
        .align  4
        .global _start
        .type   _start, function

_start:
        # Initialize mtvec register
        la     t2, trap_vector
        csrw   mtvec, t2
        
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

        # Call _init
        call _init

        j __epos_app_entry

    .align 2
trap_vector:
        addi       sp, sp, -64
        sw         ra, 0(sp)
        sw         a0, 4(sp)
        sw         a1, 8(sp)
        sw         a2, 12(sp)
        sw         a3, 16(sp)
        sw         a4, 20(sp)
        sw         a5, 24(sp)
        sw         a6, 28(sp)
        sw         a7, 32(sp)
        sw         t0, 36(sp)
        sw         t1, 40(sp)
        sw         t2, 44(sp)
        sw         t3, 48(sp)
        sw         t4, 52(sp)
        sw         t5, 56(sp)
        sw         t6, 60(sp)
        jal        _int_entry
        lw         ra, 0(sp)
        lw         a0, 4(sp)
        lw         a1, 8(sp)
        lw         a2, 12(sp)
        lw         a3, 16(sp)
        lw         a4, 20(sp)
        lw         a5, 24(sp)
        lw         a6, 28(sp)
        lw         a7, 32(sp)
        lw         t0, 36(sp)
        lw         t1, 40(sp)
        lw         t2, 44(sp)
        lw         t3, 48(sp)
        lw         t4, 52(sp)
        lw         t5, 56(sp)
        lw         t6, 60(sp)
        addi       sp, sp, 64
        mret

        # Call main
        .align  4
        .globl  __epos_app_entry
        .type   __epos_app_entry, function
__epos_app_entry:
        call      main
        lw        a0, 0(sp)         # save main's return value to be used by exit()
        call      _fini
        call      _exit

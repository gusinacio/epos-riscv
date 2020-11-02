# EPOS RISC-V Program Starter

        .file "rv32_crt0.S"

        .option norvc
        .section .text
        .align  4
        .global _start
        .type   _start, function

_start:
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

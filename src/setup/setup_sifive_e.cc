// EPOS RISC-V sifive SETUP

#include <system/config.h>

extern "C" { void _vector_table() __attribute__ ((used, naked, section(".init"))); }

// Interrupt Vector Table
void _vector_table()
{
    ASM(".word  _int_entry + 1    \n"
        "j _start                 \n"
       );
};

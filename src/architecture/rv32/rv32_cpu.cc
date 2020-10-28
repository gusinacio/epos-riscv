// EPOS RISC-V 32 CPU Mediator Implementation

#include <architecture/rv32/rv32_cpu.h>
#include <system.h>

__BEGIN_SYS

// Class attributes
unsigned int CPU::_cpu_clock;
unsigned int CPU::_bus_clock;

// Class methods
void CPU::Context::save() volatile
{
    ASM("c_save:                                \n"
        "       j   c_save                      \n");
    // implement
}

void CPU::Context::load() const volatile
{
	System::_heap->free(reinterpret_cast<void *>(Memory_Map::SYS_STACK), Traits<System>::STACK_SIZE);
	ASM(" mv     sp, %0                 \n"
		" addi   sp, sp, 112            \n" : : "r"(this)
	   );

	ASM(" lw      x5,  -108(sp)         \n"
		" lw      x6,  -104(sp)         \n"
		" lw      x7,  -100(sp)         \n"
		" lw      x8,   -96(sp)         \n"
		" lw      x9,   -92(sp)         \n"
		" lw     x10,   -88(sp)         \n"
		" lw     x11,   -84(sp)         \n"
		" lw     x12,   -80(sp)         \n"
		" lw     x13,   -76(sp)         \n"
		" lw     x14,   -72(sp)         \n"
		" lw     x15,   -68(sp)         \n"
		" lw     x16,   -64(sp)         \n"
		" lw     x17,   -60(sp)         \n"
		" lw     x18,   -56(sp)         \n"
		" lw     x19,   -52(sp)         \n"
		" lw     x20,   -48(sp)         \n"
		" lw     x21,   -44(sp)         \n"
		" lw     x22,   -40(sp)         \n"
		" lw     x23,   -36(sp)         \n"
		" lw     x24,   -32(sp)         \n"
		" lw     x25,   -28(sp)         \n"
		" lw     x26,   -24(sp)         \n"
		" lw     x27,   -20(sp)         \n"
		" lw     x28,   -16(sp)         \n"
		" lw     x29,   -12(sp)         \n"
		" lw     x30,    -8(sp)         \n"
		" lw     x31,    -4(sp)         \n"
		" lw      x1,  -112(sp)         \n"  // pop LR
		" lw     x31,     0(sp)         \n"  // pop PC
		" jalr    x0,     (x31)         \n"  // jump to PC and link register x0
	   );
}

void CPU::switch_context(Context ** o, Context * n)
{
	ASM(" sw     x31,    -4(sp)          \n"  // push x31 into stack to use as temp register
		" la     x31,     .ret           \n"  // get return address
		" sw     x31,    -0(sp)          \n"  // push return address as PC
		" lw     x31,    -4(sp)          \n"  // pop x31
		" sw      x5,  -108(sp)          \n"  // push x5-x31
		" sw      x6,  -104(sp)          \n"
		" sw      x7,  -100(sp)          \n"
		" sw      x8,   -96(sp)          \n"
		" sw      x9,   -92(sp)          \n"
		" sw     x10,   -88(sp)          \n"
		" sw     x11,   -84(sp)          \n"
		" sw     x12,   -80(sp)          \n"
		" sw     x13,   -76(sp)          \n"
		" sw     x14,   -72(sp)          \n"
		" sw     x15,   -68(sp)          \n"
		" sw     x16,   -64(sp)          \n"
		" sw     x17,   -60(sp)          \n"
		" sw     x18,   -56(sp)          \n"
		" sw     x19,   -52(sp)          \n"
		" sw     x20,   -48(sp)          \n"
		" sw     x21,   -44(sp)          \n"
		" sw     x22,   -40(sp)          \n"
		" sw     x23,   -36(sp)          \n"
		" sw     x24,   -32(sp)          \n"
		" sw     x25,   -28(sp)          \n"
		" sw     x26,   -24(sp)          \n"
		" sw     x27,   -20(sp)          \n"
		" sw     x28,   -16(sp)          \n"
		" sw     x29,   -12(sp)          \n"
		" sw     x30,    -8(sp)          \n"
		" sw     x31,    -4(sp)          \n"
		" sw      x1,  -112(sp)          \n"  // push lr
		" addi    sp,  sp, -112          \n"  // adjusts SP
		" sw      sp,     0(a0)          \n"  // updates the saved context in a0
	   );

	ASM("      mv      sp,  a1           \n"  // gets the context to be loaded into SP
		"      addi    sp,  sp, 112      \n"  // adjusts stack pointer
		"      lw      x5,  -108(sp)     \n"  // pop x5-x31
		"      lw      x6,  -104(sp)     \n"
		"      lw      x7,  -100(sp)     \n"
		"      lw      x8,   -96(sp)     \n"
		"      lw      x9,   -92(sp)     \n"
		"      lw     x10,   -88(sp)     \n"
		"      lw     x11,   -84(sp)     \n"
		"      lw     x12,   -80(sp)     \n"
		"      lw     x13,   -76(sp)     \n"
		"      lw     x14,   -72(sp)     \n"
		"      lw     x15,   -68(sp)     \n"
		"      lw     x16,   -64(sp)     \n"
		"      lw     x17,   -60(sp)     \n"
		"      lw     x18,   -56(sp)     \n"
		"      lw     x19,   -52(sp)     \n"
		"      lw     x20,   -48(sp)     \n"
		"      lw     x21,   -44(sp)     \n"
		"      lw     x22,   -40(sp)     \n"
		"      lw     x23,   -36(sp)     \n"
		"      lw     x24,   -32(sp)     \n"
		"      lw     x25,   -28(sp)     \n"
		"      lw     x26,   -24(sp)     \n"
		"      lw     x27,   -20(sp)     \n"
		"      lw     x28,   -16(sp)     \n"
		"      lw     x29,   -12(sp)     \n"
		"      lw     x30,    -8(sp)     \n"
		"      lw     x31,    -4(sp)     \n"
		"      lw      x1,  -112(sp)     \n"  // pop LR
		"      lw     x31,     0(sp)     \n"  // pop PC
		"      jalr    x0,     (x31)     \n"  // return for the thread entering the CPU
		".ret: jalr    x0,      (x1)     \n"  // return for the thread leaving the CPU
	   );
}

__END_SYS

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
    ASM("       sw      x31, -120(sp)           \n"     // push x31 to its offset on the stack to use it as a temporary
        "       la      x31,      pc            \n"
        "       sw      x31,   -4(sp)           \n"     // push PC
        "       lw      x31, -120(sp)           \n"     // pop x31
        "       sw       x5, -112(sp)           \n"     // push x5-x31
        "       sw       x6, -108(sp)           \n"
        "       sw       x7, -104(sp)           \n"
        "       sw       x8, -100(sp)           \n"
        "       sw       x9,  -96(sp)           \n"
        "       sw      x10,  -92(sp)           \n"
        "       sw      x11,  -88(sp)           \n"
        "       sw      x12,  -84(sp)           \n"
        "       sw      x13,  -80(sp)           \n"
        "       sw      x14,  -76(sp)           \n"
        "       sw      x15,  -72(sp)           \n"
        "       sw      x16,  -68(sp)           \n"
        "       sw      x17,  -64(sp)           \n"
        "       sw      x18,  -60(sp)           \n"
        "       sw      x19,  -56(sp)           \n"
        "       sw      x20,  -52(sp)           \n"
        "       sw      x21,  -48(sp)           \n"
        "       sw      x22,  -44(sp)           \n"
        "       sw      x23,  -40(sp)           \n"
        "       sw      x24,  -36(sp)           \n"
        "       sw      x25,  -32(sp)           \n"
        "       sw      x26,  -28(sp)           \n"
        "       sw      x27,  -24(sp)           \n"
        "       sw      x28,  -20(sp)           \n"
        "       sw      x29,  -16(sp)           \n"
        "       sw      x30,  -12(sp)           \n"
        "       sw      x31,   -8(sp)           \n"
        "       sw       ra, -116(sp)           \n");   // push LR

    ASM("       addi     sp, sp, -116           \n"                     // complete the partial pushes above by adjusting the SP
        "       sw       sp, 0(%0)              \n" : : "r"(this));     // update the this pointer to match the context saved on the stack
}

void CPU::Context::load() const volatile
{
    System::_heap->free(reinterpret_cast<void *>(Memory_Map::SYS_STACK), Traits<System>::STACK_SIZE);
    ASM("       mv      sp, %0                  \n"                     // load the stack pointer with the this pointer
        "       addi    sp,  sp, 116            \n" : : "r"(this));     // adjust the stack pointer to match the subsequent series of pops

    ASM("       lw      x5,  -112(sp)           \n"     // pop x5-x31
        "       lw      x6,  -108(sp)           \n"
        "       lw      x7,  -104(sp)           \n"
        "       lw      x8,  -100(sp)           \n"
        "       lw      x9,   -96(sp)           \n"
        "       lw      x10,  -92(sp)           \n"
        "       lw      x11,  -88(sp)           \n"
        "       lw      x12,  -84(sp)           \n"
        "       lw      x13,  -80(sp)           \n"
        "       lw      x14,  -76(sp)           \n"
        "       lw      x15,  -72(sp)           \n"
        "       lw      x16,  -68(sp)           \n"
        "       lw      x17,  -64(sp)           \n"
        "       lw      x18,  -60(sp)           \n"
        "       lw      x19,  -56(sp)           \n"
        "       lw      x20,  -52(sp)           \n"
        "       lw      x21,  -48(sp)           \n"
        "       lw      x22,  -44(sp)           \n"
        "       lw      x23,  -40(sp)           \n"
        "       lw      x24,  -36(sp)           \n"
        "       lw      x25,  -32(sp)           \n"
        "       lw      x26,  -28(sp)           \n"
        "       lw      x27,  -24(sp)           \n"
        "       lw      x28,  -20(sp)           \n"
        "       lw      x29,  -16(sp)           \n"
        "       lw      x30,  -12(sp)           \n"
        "       lw      x31,   -8(sp)           \n"
        "       lw       ra, -116(sp)           \n"     // pop LR
        "       lw      x31,   -4(sp)           \n"     // pop PC
        "       jalr     x0,    (x31)           \n");   // jump to pc, as x0 is passed to be linked it is equivalent to a jr (x31)
}

void CPU::switch_context(Context ** o, Context * n)
{
    ASM("       sw      x31,   -8(sp)           \n"     // push x31 into the stack to use it as a temporary register
        "       la      x31,   .ret             \n"     // get the return address
        "       sw      x31,   -4(sp)           \n"     // push the return address as PC
        "       lw      x31,   -8(sp)           \n"     // pop x31
        "       sw       x5, -112(sp)           \n"     // push x5-x31
        "       sw       x6, -108(sp)           \n"
        "       sw       x7, -104(sp)           \n"
        "       sw       x8, -100(sp)           \n"
        "       sw       x9,  -96(sp)           \n"
        "       sw      x10,  -92(sp)           \n"
        "       sw      x11,  -88(sp)           \n"
        "       sw      x12,  -84(sp)           \n"
        "       sw      x13,  -80(sp)           \n"
        "       sw      x14,  -76(sp)           \n"
        "       sw      x15,  -72(sp)           \n"
        "       sw      x16,  -68(sp)           \n"
        "       sw      x17,  -64(sp)           \n"
        "       sw      x18,  -60(sp)           \n"
        "       sw      x19,  -56(sp)           \n"
        "       sw      x20,  -52(sp)           \n"
        "       sw      x21,  -48(sp)           \n"
        "       sw      x22,  -44(sp)           \n"
        "       sw      x23,  -40(sp)           \n"
        "       sw      x24,  -36(sp)           \n"
        "       sw      x25,  -32(sp)           \n"
        "       sw      x26,  -28(sp)           \n"
        "       sw      x27,  -24(sp)           \n"
        "       sw      x28,  -20(sp)           \n"
        "       sw      x29,  -16(sp)           \n"
        "       sw      x30,  -12(sp)           \n"
        "       sw      x31,   -8(sp)           \n"
        "       sw       ra, -116(sp)           \n"     // push LR
        "       addi     sp, sp, -116           \n"     // complete the partial pushes above by adjusting the SP
        "       sw       sp,  0(a0)             \n");   // update Context * volatile * o

    ASM("       mv       sp,  a1                \n"     // get Context * volatile n into SP
        "       addi     sp,  sp, 116           \n"     // adjust stack pointer to complete the partial pops
        "       lw      x5,  -112(sp)           \n"     // pop x5-x31
        "       lw      x6,  -108(sp)           \n"
        "       lw      x7,  -104(sp)           \n"
        "       lw      x8,  -100(sp)           \n"
        "       lw      x9,   -96(sp)           \n"
        "       lw      x10,  -92(sp)           \n"
        "       lw      x11,  -88(sp)           \n"
        "       lw      x12,  -84(sp)           \n"
        "       lw      x13,  -80(sp)           \n"
        "       lw      x14,  -76(sp)           \n"
        "       lw      x15,  -72(sp)           \n"
        "       lw      x16,  -68(sp)           \n"
        "       lw      x17,  -64(sp)           \n"
        "       lw      x18,  -60(sp)           \n"
        "       lw      x19,  -56(sp)           \n"
        "       lw      x20,  -52(sp)           \n"
        "       lw      x21,  -48(sp)           \n"
        "       lw      x22,  -44(sp)           \n"
        "       lw      x23,  -40(sp)           \n"
        "       lw      x24,  -36(sp)           \n"
        "       lw      x25,  -32(sp)           \n"
        "       lw      x26,  -28(sp)           \n"
        "       lw      x27,  -24(sp)           \n"
        "       lw      x28,  -20(sp)           \n"
        "       lw      x29,  -16(sp)           \n"
        "       lw      x30,  -12(sp)           \n"
        "       lw      x31,   -8(sp)           \n"
        "       lw       ra, -116(sp)           \n"     // pop LR
        "       lw      x31,   -4(sp)           \n");   // pop PC
        int_enable();
    ASM("       jalr     x0,    (x31)           \n");     // return (for the thread entering the CPU)
    ASM(".ret:  jalr     x0,     (ra)           \n");   // return (for the thread leaving the CPU)
}


__END_SYS

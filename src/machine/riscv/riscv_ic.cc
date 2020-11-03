// EPOS RISC-V IC Mediator Implementation

#include <machine/machine.h>
#include <machine/ic.h>

extern "C" { void _int_entry() __attribute__ ((aligned(4), alias("_ZN4EPOS1S2IC5entryEv"))); }
extern "C" { void _dispatch(unsigned int) __attribute__ ((alias("_ZN4EPOS1S2IC8dispatchEv"))); }
extern "C" { void _undefined_instruction() __attribute__ ((alias("_ZN4EPOS1S2IC21undefined_instructionEj"))); }
extern "C" { void _software_interrupt() __attribute__ ((alias("_ZN4EPOS1S2IC18software_interruptEj"))); }
extern "C" { void _prefetch_abort() __attribute__ ((alias("_ZN4EPOS1S2IC14prefetch_abortEj"))); }
extern "C" { void _data_abort() __attribute__ ((alias("_ZN4EPOS1S2IC10data_abortEj"))); }
extern "C" { void _reserved() __attribute__ ((alias("_ZN4EPOS1S2IC8reservedEj"))); }
extern "C" { void _fiq() __attribute__ ((alias("_ZN4EPOS1S2IC3fiqEj"))); }
extern "C" { void _exception_handling() __attribute__ ((alias("_ZN4EPOS1S2IC18exception_handlingEv"))); }

__BEGIN_SYS

// Class attributes
IC::Interrupt_Handler IC::_int_vector[IC::INTS];
CPU::Reg32 IC::_old_state;

// Class methods
void IC::entry()
{
    ASM(
        "addi       sp, sp, -64            \n"
        "sw         ra, 0(sp)              \n"
        "sw         a0, 4(sp)              \n"
        "sw         a1, 8(sp)              \n"
        "sw         a2, 12(sp)             \n"
        "sw         a3, 16(sp)             \n"
        "sw         a4, 20(sp)             \n"
        "sw         a5, 24(sp)             \n"
        "sw         a6, 28(sp)             \n"
        "sw         a7, 32(sp)             \n"
        "sw         t0, 36(sp)             \n"
        "sw         t1, 40(sp)             \n"
        "sw         t2, 44(sp)             \n"
        "sw         t3, 48(sp)             \n"
        "sw         t4, 52(sp)             \n"
        "sw         t5, 56(sp)             \n"
        "sw         t6, 60(sp)             \n"
        "csrr       a0, mcause             \n"
        "jal        _dispatch              \n"
        "lw         ra, 0(sp)              \n"
        "lw         a0, 4(sp)              \n"
        "lw         a1, 8(sp)              \n"
        "lw         a2, 12(sp)             \n"
        "lw         a3, 16(sp)             \n"
        "lw         a4, 20(sp)             \n"
        "lw         a5, 24(sp)             \n"
        "lw         a6, 28(sp)             \n"
        "lw         a7, 32(sp)             \n"
        "lw         t0, 36(sp)             \n"
        "lw         t1, 40(sp)             \n"
        "lw         t2, 44(sp)             \n"
        "lw         t3, 48(sp)             \n"
        "lw         t4, 52(sp)             \n"
        "lw         t5, 56(sp)             \n"
        "lw         t6, 60(sp)             \n"
        "addi       sp, sp, 64             \n");
    ASM("mret                              \n");
}

void IC::dispatch()
{
    Interrupt_Id id = int_id();

    if((id != INT_SYS_TIMER) || Traits<IC>::hysterically_debugged)
        db<IC>(TRC) << "IC::dispatch(i=" << id << ")" << endl;

    _int_vector[id](id);
}

void IC::int_not(Interrupt_Id i)
{
    db<IC>(WRN) << "IC::int_not(i=" << i << ")" << endl;
}

void IC::hard_fault(Interrupt_Id i)
{
    db<IC>(ERR) << "IC::hard_fault(i=" << i << ")" << endl;
    Machine::panic();
}

void IC::undefined_instruction(Interrupt_Id i)
{
    db<IC>(ERR) << "Undefined instruction(i=" << i << ")" << endl;
    Machine::panic();
}

void IC::software_interrupt(Interrupt_Id i)
{
    db<IC>(ERR) << "Software interrupt(i=" << i << ")" << endl;
    Machine::panic();
}

void IC::prefetch_abort(Interrupt_Id i)
{
    db<IC>(ERR) << "Prefetch abort(i=" << i << ")" << endl;
    Machine::panic();
}

void IC::data_abort(Interrupt_Id i)
{
    db<IC>(ERR) << "Data abort(i=" << i << ")" << endl;
    Machine::panic();
}

void IC::reserved(Interrupt_Id i)
{
    db<IC>(ERR) << "Reserved(i=" << i << ")" << endl;
    Machine::panic();
}

void IC::fiq(Interrupt_Id i)
{
    db<IC>(ERR) << "FIQ handler(i=" << i << ")" << endl;
    Machine::panic();
}

void IC::exception_handling()
{
    db<IC>(ERR) << "Exception abort" << endl;
    // IMPLEMENT
    Machine::panic();
}
__END_SYS

// EPOS RISC-V Interrupt Controller Initialization

#include <architecture/cpu.h>
#include <machine/ic.h>
#include <machine/timer.h>

__BEGIN_SYS

// Class methods
void IC::init()
{
    db<Init, IC>(TRC) << "IC::init()" << endl;

    CPU::int_disable(); // will be reenabled at Thread::init() by Context::load()

    disable(); // will be enabled on demand as handlers are registered

    // Set all interrupt handlers to int_not()
    for(Interrupt_Id i = 0; i < INTS; i++)
        _int_vector[i] = int_not;

    // Sets IPI interruptions handler
    if (Traits<System>::multicore) {
    	int_vector(IC::MACHINE_SOFT_INT, IC::ipi_eoi);
        //enable(IC::MACHINE_SOFT_INT);
    }

}

__END_SYS

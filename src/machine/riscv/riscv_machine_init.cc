// EPOS RISC V Initialization

#include <machine.h>

__BEGIN_SYS

void Machine::pre_init(System_Info * si)
{
    if(CPU::id() == 0)
        Display::init();

    db<Init>(WRN) << "Machine::pre_init()" << endl;

    //db<Init, Machine>(TRC) << "Machine::pre_init()" << endl;

    if(CPU::id() == 0) {
        if(Traits<IC>::enabled) {
            IC::init();

            db<Init>(WRN) << "Machine::IC CPUS: "<< Traits<Build>::CPUS << endl;

            // Wake up remaining CPUs
            //si->bm.n_cpus = Traits<Build>::CPUS;
            if(Traits<System>::multicore)
            {
                for (unsigned int i = 1; i < Traits<Build>::CPUS; ++i)
                    IC::ipi(i, IC::MACHINE_SOFT_INT);

                smp_barrier_init(Traits<Build>::CPUS);
            }
        }
    }
}

void Machine::init()
{
    db<Init, Machine>(TRC) << "Machine::init()" << endl;

    if(Traits<Timer>::enabled)
        Timer::init();
}

__END_SYS

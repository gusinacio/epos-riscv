// EPOS RISC-V Mediator Declarations

#ifndef __riscv_machine_h
#define __riscv_machine_h

#include <architecture.h>
#include <machine/machine.h>
#include <machine/ic.h>
#include <system/info.h>
#include <system/memory_map.h>
#include <system.h>

__BEGIN_SYS

class Machine: private Machine_Common
{
    friend class Init_System;
    friend class First_Object;

private:
    static const bool smp = Traits<System>::multicore;

public:

    enum {
        SIFIVE_TEST_FAIL = 0x3333,
	    SIFIVE_TEST_PASS = 0x5555,
        SIFIVE_TEST_CTRL_ADDR = 0x100000,
    };

    Machine() {}

    static void delay(const Microsecond & time) {
        assert(Traits<TSC>::enabled);
        TSC::Time_Stamp end = TSC::time_stamp() + time * (TSC::frequency() / 1000000);
        while(end > TSC::time_stamp());
    }

    static void panic();

    static void reboot()
    {
        if (Traits<System>::reboot) {
            db<Machine>(WRN) << "Machine::reboot()" << endl;
            poweroff();
        } else {
            CPU::halt();
        }
    }
    static void poweroff()
    {
        db<Machine>(WRN) << "Machine::poweroff()" << endl;
        volatile CPU::Reg32 *test = (CPU::Reg32 *)(void *)(SIFIVE_TEST_CTRL_ADDR);
        *test = SIFIVE_TEST_PASS;
        while (1) {
            asm volatile("");
        }
    }

    static void smp_barrier_init(unsigned int n_cpus) {
        db<Machine>(TRC) << "SMP::init()" << endl;
        // IMPLEMENT
    }

    static const UUID & uuid() { return System::info()->bm.uuid; }

private:
    static void pre_init(System_Info * si);
    static void init();
};

__END_SYS

#endif

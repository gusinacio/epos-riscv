// EPOS RISC-V 32 CPU Mediator Declarations

#ifndef __riscv32_h
#define __riscv32_h

#include <architecture/cpu.h>

__BEGIN_SYS

class CPU: protected CPU_Common
{
    friend class Init_System;
    friend class Machine;

private:
    static const bool smp = Traits<System>::multicore;

public:
    // CPU Native Data Types
    using CPU_Common::Reg8;
    using CPU_Common::Reg16;
    using CPU_Common::Reg32;
    using CPU_Common::Reg64;
    using Reg = CPU_Common::Reg32;
    using Log_Addr = CPU_Common::Log_Addr<Reg>;
    using Phy_Addr = CPU_Common::Phy_Addr<Reg>;

    static const bool thumb = true;

    // CPU Flags
    typedef Reg32 Flags;
    enum {
        //implement
        FLAG_Z = 1 << 30,

        SIFIVE_TEST_FAIL = 0x3333,
	    SIFIVE_TEST_PASS = 0x5555,
        SIFIVE_TEST_CTRL_ADDR = 0x100000,
    };

    // CPU Context
    class Context
    {
    public:
        Context(const Log_Addr & entry, const Log_Addr & exit): _reg_ra(exit), _reg_x5(0), _reg_x6(0),
        _reg_x7(0), _reg_x8(0), _reg_x9(0), _reg_x10(0), _reg_x11(0), _reg_x12(0), _reg_x13(0), _reg_x14(0),
        _reg_x15(0), _reg_x16(0), _reg_x17(0), _reg_x18(0), _reg_x19(0), _reg_x20(0), _reg_x21(0), _reg_x22(0),
        _reg_x23(0), _reg_x24(0), _reg_x25(0), _reg_x26(0), _reg_x27(0), _reg_x28(0), _reg_x29(0), _reg_x30(0),
        _reg_x31(0), _reg_ip(entry) {}

        void save() volatile  __attribute__ ((naked));
        void load() const volatile;

        friend Debug & operator<<(Debug & db, const Context & c) {
            db << hex
               << "{REG NAME" << 0 //registers
               << "}" << dec;
            return db;
        }

    public:
        Reg32 _reg_ra; // return address
        Reg32 _reg_x5; // general-purpose registers (x5-31)
        Reg32 _reg_x6;
        Reg32 _reg_x7;
        Reg32 _reg_x8;
        Reg32 _reg_x9;
        Reg32 _reg_x10;
        Reg32 _reg_x11;
        Reg32 _reg_x12;
        Reg32 _reg_x13;
        Reg32 _reg_x14;
        Reg32 _reg_x15;
        Reg32 _reg_x16;
        Reg32 _reg_x17;
        Reg32 _reg_x18;
        Reg32 _reg_x19;
        Reg32 _reg_x20;
        Reg32 _reg_x21;
        Reg32 _reg_x22;
        Reg32 _reg_x23;
        Reg32 _reg_x24;
        Reg32 _reg_x25;
        Reg32 _reg_x26;
        Reg32 _reg_x27;
        Reg32 _reg_x28;
        Reg32 _reg_x29;
        Reg32 _reg_x30;
        Reg32 _reg_x31;
        Reg32 _reg_ip; // instruction pointer
    };

    
    // Interrupt Service Routines
    typedef void (ISR)();

    // Fault Service Routines (exception handlers)
    typedef void (FSR)();

public:
    CPU() {};

public:
    // Register access
    static Reg32 sp() {
        Reg32 value;
        ASM("lw %0, sp" : "=r"(value) :);
        return value;
    }

    static void sp(const Reg32 & sp) {
        ASM("sw sp, %0" : : "r"(sp) : "sp");
    }

    static Reg32 fr() {
        //implement
        return 0;
    }

    static void fr(const Reg32 & fr) {
        //implement
    }

    static Log_Addr ip() {
        Reg32 value;
        ASM("lw %0, pc" : "=r"(value) :);
        return value;
    }

    static Reg32 pdp() { return 0; }
    static void pdp(const Reg32 & pdp) {}


    // Atomic operations

    using CPU_Common::tsl;
    /*
    template<typename T>
    static T tsl(volatile T & lock) {
        //implement
    }
    */

    using CPU_Common::finc;
    /*
    template<typename T>
    static T finc(volatile T & value) {
        //implement
    }
    */

    using CPU_Common::fdec;
    /*
    template<typename T>
    static T fdec(volatile T & value) {
        //implement
    }
    */

    using CPU_Common::cas;
    /*
    template <typename T>
    static T cas(volatile T & value, T compare, T replacement) {
        //implement
    }
    */

    // Power modes
    static void halt() {
        volatile Reg32 *test = (Reg32 *)(void *)(SIFIVE_TEST_CTRL_ADDR);
        *test = SIFIVE_TEST_PASS;
        while (1) {
            asm volatile("");
        }
    }

    //implement
    static Flags flags() {
        //implement
        return 0;
    }

    static void flags(const Flags & flags) {
        //implement
    }

    static unsigned int id() {
        //implement
        return 0;
    }

    static unsigned int cores() {
        return Traits<Build>::CPUS;
    }

    static void smp_barrier(unsigned long cores = cores()) { CPU_Common::smp_barrier<&finc>(cores, id()); }

    static void int_enable() { /* implement */ }
    static void int_disable() { /* implement */ }

    static bool int_enabled() { /* implement */ return 0; }
    static bool int_disabled() { /* implement */ return !int_enabled(); }

    static void csrr31() { /* implement - write ctrl and status register to x31 */ }
    static void csrw31() { /* implement - write x31 to ctrl and status register */ }

    static unsigned int int_id() { return 0; }

    static void switch_context(Context ** o, Context * n) __attribute__ ((naked));

    template<typename ... Tn>
    static Context * init_stack(const Log_Addr & usp, Log_Addr sp, void (* exit)(), int (* entry)(Tn ...), Tn ... an) {
        sp -= sizeof(Context);
        Context * ctx = new(sp) Context(entry, exit);
        init_stack_helper(&ctx->_reg_x10, an ...);
        return ctx;
    }
    template<typename ... Tn>
    static Log_Addr init_user_stack(Log_Addr sp, void (* exit)(), Tn ... an) {
        sp -= sizeof(Context);
        Context * ctx = new(sp) Context(0, exit);
        init_stack_helper(&ctx->_reg_x10, an ...);
        return sp;
    }

    static int syscall(void * message);
    static void syscalled();

    using CPU_Common::htole64;
    using CPU_Common::htole32;
    using CPU_Common::htole16;
    using CPU_Common::letoh64;
    using CPU_Common::letoh32;
    using CPU_Common::letoh16;

    using CPU_Common::htobe64;
    using CPU_Common::htobe32;
    using CPU_Common::htobe16;
    using CPU_Common::betoh64;
    using CPU_Common::betoh32;
    using CPU_Common::betoh16;

    using CPU_Common::htonl;
    using CPU_Common::htons;
    using CPU_Common::ntohl;
    using CPU_Common::ntohs;

private:
    template<typename Head, typename ... Tail>
    static void init_stack_helper(Log_Addr sp, Head head, Tail ... tail) {
        *static_cast<Head *>(sp) = head;
        init_stack_helper(sp + sizeof(Head), tail ...);
    }
    static void init_stack_helper(Log_Addr sp) {}

    static void init();

private:
    static unsigned int _cpu_clock;
    static unsigned int _bus_clock;
};

inline CPU::Reg64 htole64(CPU::Reg64 v) { return CPU::htole64(v); }
inline CPU::Reg32 htole32(CPU::Reg32 v) { return CPU::htole32(v); }
inline CPU::Reg16 htole16(CPU::Reg16 v) { return CPU::htole16(v); }
inline CPU::Reg64 letoh64(CPU::Reg64 v) { return CPU::letoh64(v); }
inline CPU::Reg32 letoh32(CPU::Reg32 v) { return CPU::letoh32(v); }
inline CPU::Reg16 letoh16(CPU::Reg16 v) { return CPU::letoh16(v); }

inline CPU::Reg64 htobe64(CPU::Reg64 v) { return CPU::htobe64(v); }
inline CPU::Reg32 htobe32(CPU::Reg32 v) { return CPU::htobe32(v); }
inline CPU::Reg16 htobe16(CPU::Reg16 v) { return CPU::htobe16(v); }
inline CPU::Reg64 betoh64(CPU::Reg64 v) { return CPU::betoh64(v); }
inline CPU::Reg32 betoh32(CPU::Reg32 v) { return CPU::betoh32(v); }
inline CPU::Reg16 betoh16(CPU::Reg16 v) { return CPU::betoh16(v); }

inline CPU::Reg32 htonl(CPU::Reg32 v) { return CPU::htonl(v); }
inline CPU::Reg16 htons(CPU::Reg16 v) { return CPU::htons(v); }
inline CPU::Reg32 ntohl(CPU::Reg32 v) { return CPU::ntohl(v); }
inline CPU::Reg16 ntohs(CPU::Reg16 v) { return CPU::ntohs(v); }

__END_SYS

#endif

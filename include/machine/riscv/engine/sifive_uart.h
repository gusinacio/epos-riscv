// EPOS ARM NS16550A UART Mediator Declarations

#ifndef __sifive_uart_h
#define __sifive_uart_h

#include <architecture/cpu.h>
#define __uart_common_only__
#include <machine/uart.h>
#undef __uart_common_only__

__BEGIN_SYS

class Sifive_Uart: public UART_Common
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::UARTx_BASE) PL011".

private:
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg32 Reg32;

    static const unsigned int unit = Traits<UART>::CLOCK;

public:
    // Registers offsets from BASE (i.e. this)
    enum {
        /* UART Registers */
        UART_REG_TXFIFO = 0,
        UART_REG_RXFIFO = 1,
        UART_REG_TXCTRL = 2,
        UART_REG_RXCTRL = 3,
        UART_REG_IE     = 4,
        UART_REG_IP     = 5,
        UART_REG_DIV    = 6,

        /* TXCTRL register */
        UART_TXEN       = 1,
        UART_TXSTOP     = 2,

        /* RXCTRL register */
        UART_RXEN       = 1,

        /* IP register */
        UART_IP_TXWM    = 1,
        UART_IP_RXWM    = 2
    };

public:
    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        Reg32 divisor = CLOCK / baud_rate - 1;
        uart(UART_REG_DIV) = divisor;
        uart(UART_REG_TXCTRL) = UART_TXEN;
        uart(UART_REG_RXCTRL) = UART_RXEN;
        uart(UART_REG_IE) = 0;
    }

    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits) {
        // TODO retornar dados
    }

    Reg8 rxd() { return uart(UART_REG_RXFIFO); }
    void txd(Reg8 c) { uart(UART_REG_TXFIFO) = c; }


    bool rxd_ok() { return !(uart(UART_REG_RXFIFO) < 0); }
    bool txd_ok() { return !(uart(UART_REG_TXFIFO) < 0); }

    bool rxd_full() { 
        // implement
        return false; 
    }
    bool txd_empty() {
        // implement
        return false; 
    }

    bool busy() {
        // implement
        return false; 
    }

    void enable() { uart(UCR) |= UEN | TXE | RXE; }
    void disable() { uart(UCR) &= ~UEN; }

    void int_enable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        uart(UIM) &= ~((receive ? UIMRX : 0) | (transmit ? UIMTX : 0));
    }
    void int_disable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        uart(UCR) |= (receive ? UIMRX : 0) | (transmit ? UIMTX : 0);
    }

    void reset() {
        unsigned int b, db, p, sb;
        config(&b, &db, &p, &sb);
        config(b, db, p, sb);
    }

    void loopback(bool flag) {
        if(flag)
            uart(UCR) |= int(LBE);
        else
            uart(UCR) &= ~LBE;
    }

private:
    volatile Reg32 & uart(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif

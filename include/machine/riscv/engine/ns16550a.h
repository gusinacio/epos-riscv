// EPOS ARM NS16550A UART Mediator Declarations

#ifndef __ns16550a_h
#define __ns16550a_h

#include <architecture/cpu.h>
#define __uart_common_only__
#include <machine/uart.h>
#undef __uart_common_only__

__BEGIN_SYS

class NS16550A: public UART_Common
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
        Reg32 lcrh = data_bits == 8 ? WLEN8 : data_bits == 7 ? WLEN7 : data_bits = 6 ? WLEN6 : WLEN5; // config data bits
        lcrh |= FEN; // always use FIFO
        lcrh |= stop_bits == 2 ? STP2 : 0; // config stop bits
        lcrh |= (parity == UART_Common::EVEN) ? (EPS | PEN) : (parity == UART_Common::ODD) ? PEN : 0; // config and enable even/odd parity

        uart(UCR) &= ~UEN;                       // Disable UART for configuration
        uart(ICR) = ~0;                          // Clear all interrupts
        uart(UIM) = UIMALL;                      // Disable all interrupts
        Reg32 br = CLOCK / (baud_rate / 300);   // Factor by the minimum BR to preserve meaningful bits of FBRD
        uart(IBRD) = br / 300;                   // IBRD = int(CLOCK / baud_rate)
        uart(FBRD) = br / 1000;                  // FBRD = int(0.1267 * 64 + 0.5) = 8
        uart(LCRH) = lcrh;                       // Write the serial parameters configuration
        uart(UIM) = UIMTX | UIMRX;               // Mask TX and RX interrupts for polling operation
        uart(UCR) |= UEN | TXE | RXE;            // Enable UART


        uint32_t uart_freq = getauxval(UART0_CLOCK_FREQ);
        uint32_t divisor = uart_freq / (16 * baud_rate);
        uart[UART_LCR] = UART_LCR_DLAB;
        uart[UART_DLL] = divisor & 0xff;
        uart[UART_DLM] = (divisor >> 8) & 0xff;
        uart[UART_LCR] = UART_LCR_PODD | UART_LCR_8BIT;
    }

    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits) {
        Reg32 lcrh = uart(LCRH);
        *data_bits = 5 + (lcrh & WLEN8);
        *parity = (lcrh & PEN) ? (1 + (lcrh & EPS)) : 0;
        *baud_rate = (CLOCK * 300) / (uart(FBRD) * 1000 + uart(IBRD) * 300);
        *stop_bits = (uart(LCRH) & STP2) ? 2 : 1;
    }

    Reg8 rxd() { return uart(DR); }
    void txd(Reg8 c) { uart(DR) = c; }


    bool rxd_ok() { return !(uart(FR) & RXFE); }
    bool txd_ok() { return !(uart(FR) & TXFF); }

    bool rxd_full() { return (uart(FR) & RXFF); }
    bool txd_empty() { return (uart(FR) & TXFE) && !(uart(FR) & BUSY); }

    bool busy() { return (uart(FR) & BUSY); }

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

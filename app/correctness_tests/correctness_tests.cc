#include <utility/ostream.h>
#include <process.h>

using namespace EPOS;

OStream cout;

Thread *threads[3];

int thread_test(int id, int time)
{
    cout << "Running thread " << id << "-> Configured Burst Time: " << time << endl;
    return 0;
}

void print_mstatus()
{
    CPU::Reg32 mstatus;
    ASM("csrr %0, mstatus" : "=r"(mstatus) :);
    cout << "mstatus=" << bin << mstatus << endl;
}

void print_mtvec()
{
    CPU::Reg32 mtvec;
    ASM("csrr %0, mtvec" : "=r"(mtvec) :);
    cout << "mtvec=" << hex << mtvec << endl;
}

void print_satp()
{
    CPU::Reg32 satp;
    ASM("csrr %0, satp" : "=r"(satp) :);
    cout << "satp=" << bin << satp << endl;
}

void print_registers()
{
    register unsigned int sp asm("sp");
    register unsigned int gp asm("gp");
    register unsigned int x5 asm("x5");
    register unsigned int x6 asm("x6");
    register unsigned int x7 asm("x7");
    register unsigned int x8 asm("x8");
    register unsigned int x9 asm("x9");
    register unsigned int x10 asm("x10");
    register unsigned int x11 asm("x11");
    register unsigned int x12 asm("x12");
    register unsigned int x13 asm("x13");
    register unsigned int x14 asm("x14");
    register unsigned int x15 asm("x15");
    register unsigned int x16 asm("x16");
    register unsigned int x17 asm("x17");
    register unsigned int x18 asm("x18");
    register unsigned int x19 asm("x19");
    register unsigned int x20 asm("x20");
    register unsigned int x21 asm("x21");
    register unsigned int x22 asm("x22");
    register unsigned int x23 asm("x23");
    register unsigned int x24 asm("x24");
    register unsigned int x25 asm("x25");
    register unsigned int x26 asm("x26");
    register unsigned int x27 asm("x27");
    register unsigned int x28 asm("x28");
    register unsigned int x29 asm("x29");
    register unsigned int x30 asm("x30");
    register unsigned int x31 asm("x31");
    cout << "registers { sp=" << sp
         << ",gp=" << gp
         << ",x5=" << x5
         << ",x6=" << x6
         << ",x7=" << x7
         << ",x8=" << x8
         << ",x9=" << x9
         << ",x10=" << x10
         << ",x11=" << x11
         << ",x12=" << x12
         << ",x13=" << x13
         << ",x14=" << x14
         << ",x15=" << x15
         << ",x16=" << x16
         << ",x17=" << x17
         << ",x18=" << x18
         << ",x19=" << x19
         << ",x20=" << x20
         << ",x21=" << x21
         << ",x22=" << x22
         << ",x23=" << x23
         << ",x24=" << x24
         << ",x25=" << x25
         << ",x26=" << x26
         << ",x27=" << x27
         << ",x28=" << x28
         << ",x29=" << x29
         << ",x30=" << x30
         << ",x31=" << x31 << " }" << endl;
}

int bss_test_1, bss_test_2, bss_test_3;

int main()
{
    int n = 1;

    cout << endl
         << endl
         << "===== Bem-vindo ao teste de corretude do P1! =====" << endl
         << endl
         << endl;

    cout << n++ << ". Teste de corretude de inicialização do sistema!" << endl;

    cout << "- Endereço base da memória: " << hex << Traits<Machine>::MEM_BASE << dec << endl;

    cout << "- Teste BSS: Print de Variáveis Globais" << endl;
    cout << "bss_test_1: " << bss_test_1 << endl;
    cout << "bss_test_2: " << bss_test_2 << endl;
    cout << "bss_test_3: " << bss_test_3 << endl;

    cout << "- Teste de corretude de inicialização" << endl;
    print_mstatus();

    cout << "- Registradores" << endl;
    print_registers();

    cout << "- Trap Vector" << endl;
    print_mtvec();

    cout << endl;

    cout << n++ << ". Teste de corretude inicialização MMU" << endl;
    print_satp();
    cout << endl;

    cout << n++ << ". Teste de saída via UART!" << endl;
    cout << "Hello World!" << endl;
    cout << endl;

    cout << n++ << ". Teste de corretude de troca de contexto" << endl;

    cout << endl;

    cout << n++ << ". Corretude do escalonador SJF!" << endl;
    const int timing[] = {100, 50, 70};
    for (int i = 0; i < 3; i++)
        threads[i] = new Thread(Thread::Configuration(Thread::READY, Thread::Criterion(timing[i])), &thread_test, i + 1, timing[i]);
    for (int i = 0; i < 3; i++)
        threads[i]->join();

    return 0;
}

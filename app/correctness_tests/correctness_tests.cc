#include <utility/ostream.h>
#include <process.h>

using namespace EPOS;

OStream cout;

Thread *threads[3];

int thread_test(int id, int time)
{
    cout << "Running thread " << id << " on core ["<< CPU::id() <<"] -> Configured Burst Time: " << time << endl;
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

int context_test() {
    int switch_context = 7331;
    cout << "CONTEXT_THREAD(switch_context)=" << switch_context << endl;
    return 0;
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
    int switch_context = 1337;
    cout << "MAIN(switch_context)=" << switch_context << endl;
    Thread * context = new Thread(Thread::Configuration(Thread::READY, Thread::Criterion(1)), &context_test);
    context->join();
    cout << "MAIN(switch_context)=" << switch_context << endl;
    cout << endl;

    cout << n++ << ". Corretude do escalonador GFS!" << endl;
    const int timing[] = {100, 50, 70};
    for (int i = 0; i < 3; i++)
        threads[i] = new Thread(Thread::Configuration(Thread::READY, Thread::Criterion(timing[i])), &thread_test, i + 1, timing[i]);
    for (int i = 0; i < 3; i++)
        threads[i]->join();

    return 0;
}

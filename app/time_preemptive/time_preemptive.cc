#include <utility/ostream.h>
#include <process.h>
#include <time.h>

using namespace EPOS;

OStream cout;

int io_thread(int id) {
    cout << "IO Thread " << id << " Working..." << endl;
    for(int i = 0; i < 100; i++) {
        int j = 0;
        while(j++ < 1000);
        Thread::yield();
    }
    cout << "IO Thread " << id << " Finished..." << endl;
    return 0;
}

int cpu_thread1() {
    cout << "CPU Thread 1 Working..." << endl;
    for(int i = 0; i < 10; i++) {
        int j = 0;
        while(j++ < 1000000);
    }
    cout << "CPU Thread 1 Finished..." << endl;
    return 0;
}

int cpu_thread2() {
    cout << "CPU Thread 2 Working..." << endl;
    for(int i = 0; i < 3; i++) {
        int j = 0;
        while(j++ < 10000000);
    }
    cout << "CPU Thread 2 Finished..." << endl;
    return 0;
}

int main() {
    Thread *io1, *io2;
    Thread *cpu1, *cpu2;

    io1 = new Thread(&io_thread, 1);
    cpu1 = new Thread(&cpu_thread1);
    io2 = new Thread(&io_thread, 2);
    cpu2 = new Thread(&cpu_thread2);

    io1->join();
    io2->join();
    cpu1->join();
    cpu2->join();

    cout << "IO Thread 1 Priority: " << io1->priority() << endl;
    cout << "IO Thread 2 Priority: " << io2->priority() << endl;
    cout << "CPU Thread 1 Priority: " << cpu1->priority() << endl;
    cout << "CPU Thread 2 Priority: " << cpu2->priority() << endl;

    delete io1;
    delete cpu1;
    delete io2;
    delete cpu2;

    return 0;
}

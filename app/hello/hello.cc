#include <utility/ostream.h>
#include <process.h>

using namespace EPOS;

OStream cout;

int func_a()
{
	CPU::mstatus(CPU::mstatus() | (1 << 3));
	while (true)
	{
		int i = 0;
		cout << "Thread A Running" << endl;
		while(i++ < 10000000);
	}
	return 0;
}

int func_b()
{
	CPU::mstatus(CPU::mstatus() | (1 << 3));
	while (true)
	{
		int i = 0;
		cout << "Thread B Running" << endl;
		while(i++ < 10000000);
	}
	return 0;
}

int main()
{
	Thread *a, *b;

    cout << "Hello World!" << endl;

    a = new Thread(&func_a);
    b = new Thread(&func_b);

    a->join();
    b->join();
    // while (1) {
    //     continue;
    // }
    return 0;
}

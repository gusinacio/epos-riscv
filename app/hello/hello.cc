#include <utility/ostream.h>
#include <process.h>

using namespace EPOS;

OStream cout;

Thread *t1, *t2, *t3;

int thread_test(int id, int time)
{
	cout << "Running thread " << id << "-> Configured Burst Time: " << time << endl;
	return 0;
}

int main()
{
	const int time1 = 100, time2 = 50, time3 = 70;

    cout << "Hello Mundo injusto!" << endl;

    t1 = new Thread(Thread::Configuration(Thread::READY, Thread::Criterion(time1)), &thread_test, 1, time1);
    t2 = new Thread(Thread::Configuration(Thread::READY, Thread::Criterion(time2)), &thread_test, 2, time2);
    t3 = new Thread(Thread::Configuration(Thread::READY, Thread::Criterion(time3)), &thread_test, 3, time3);

    t1->join();
    t2->join();
    t3->join();

    return 0;
}

# P3 - EPOS

## Installing

Simply extract the tarball or clone the repository in a convenient location for you.

After that, it is important to check in ```makedefs``` the PATH for your compiled toolchain that targets 32-bit RISC-V.

## Testing

Go into the directory where you extracted EPOS and issue a ```make all``` to have instances of EPOS built for each of the applications in the ```app``` directory to build the intended tests for this delivery (P3). 

## Running

After building an application-oriented instance of EPOS, you can run the application with the tailored EPOS on QEMU using: ```make APPLICATION=<app> run```. The prepared tests are run with ```make APPLICATION=concurrent_philosofers_dinner run``` for atomics operations and valuation, and ```make APPLICATION=time_preemptive run``` to test the implemented scheduler in multicore. The applications run in an instance of QEMU, SiFive-E RISC-V machine.

## Tests done

-------------------------------------------------------
### Multicore Initialization
For the multicore initialization, all the cores start at the same entry point making a small initialization process, initializing their operation mode, the mtvec and enabling interrupts. After this initial process, all the harts except the core 0 will issue a ```wfi```, waiting for an interrupt to be waken, while the core 0 proceeds with the normal system initialization. In an opportune moment, the core 0 wakes up all the other cores issuing an IPI and all the other cores proceed with the initialization in the same way. This can be checked by the test app initialization itself, that uses 4 cores to successfully execute.

-------------------------------------------------------
### Atomic Operations
We implemented the ```tls```, ```finc```, ```fdec``` and ```cas``` operations in CPU using the atomic instructions in risc-v ```sc.w``` and ```lr.w```.

-------------------------------------------------------
### Timers configuration / Interrupt Handling
Each core has its own counter and uses offsets of 8 for every core in the mtime.

-------------------------------------------------------
### IPI
These are inter-processor interrupts, and can be done first enabling the interrupt id[3] and every time we write in the CLINT core offset, we create an interruption which can be used to synchronize the cores in a given point of code and create interruptions for each other.

-------------------------------------------------------
### Global Multicore Scheduling
The Global Multicore Scheduler implemented is a multicore version of the previously implemented Feedback Scheduling (FS). This global multicore version (GFS) is based in the Multihead Scheduling List implemented in EPOS, that represents a single scheduling list with multiple heads (one for each core), and its functioning can be seen in the test application.
It is important to note that in some executions the test may freeze, but in most cases it successfully completes.




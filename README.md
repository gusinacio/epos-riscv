# P1 - EPOS

## Installing

Simply extract the tarball or clone the repository in a convenient location for you.

After that, it is important to check in ```makedefs``` the PATH for your compiled toolchain that targets 32-bit RISC-V.

## Testing

Go into the directory where you extracted EPOS and issue a ```make all``` to have instances of EPOS built for each of the applications in the ```app``` directory, or ```make APPLICATION=correctness_tests``` to build the intended tests for this deliver (P1). 

## Running

After building an application-oriented instance of EPOS, you can run the application with the tailored EPOS on QEMU using: ```make APPLICATION=<app> run```. The prepared tests are run with ```make APPLICATION=correctness_tests run```. The application runs in an instance of QEMU, SiFive-E RISC-V machine.

## Tests done

-------------------------------------------------------

### BSS test
We have 3 uninitialized global variables that we print to prove that they’re zero.

-------------------------------------------------------

### Trap Vector
We print the `mtvec` register to prove that there is an address.

-------------------------------------------------------

### Memory Initialization
We print the memory base address to prove its initialization at 0x80000000 as defined in the QEMU Memory Map to the SiFive-E machine.

-------------------------------------------------------

### Operation Mode
We print the `mstatus` in binary to prove its initialization in the Machine Mode (0b11 << 11).

-------------------------------------------------------

### Registers cleaning
The registers are cleaned on a Context load. Printing all the registers are a little tricky because it may dirty some registers in the process.

-------------------------------------------------------

### MMU
We print the `satp` in binary register to prove that it is initialized with zero mode (no virtual_to_physical address translation).

------------------------------------------------------

### UART
We print a “Hello World!”.

------------------------------------------------------

### Switch Context
This is difficult to prove that we are switching contexts but we tried to create a variable where we print initially, call another thread which prints the same variable name, and after context switch again, we print the old variable again.

------------------------------------------------------

### Scheduler
We used the Shortest Job First (SJF) algorithm. The algorithm consists in telling the scheduler the intended duration of the created job, and it schedules the shortest ones first. It is a non-preemptive, non-timed and non-dinamic algorithm, and we created a test where 3 Threads with different durations are created. Each thread prints its duration and its id, to prove that those with shorter durations execute first. 
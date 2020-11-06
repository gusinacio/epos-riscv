# P2 - EPOS

## Installing

Simply extract the tarball or clone the repository in a convenient location for you.

After that, it is important to check in ```makedefs``` the PATH for your compiled toolchain that targets 32-bit RISC-V.

## Testing

Go into the directory where you extracted EPOS and issue a ```make all``` to have instances of EPOS built for each of the applications in the ```app``` directory to build the intended tests for this delivery (P2). 

## Running

After building an application-oriented instance of EPOS, you can run the application with the tailored EPOS on QEMU using: ```make APPLICATION=<app> run```. The prepared tests are run with ```make APPLICATION=alarm_test run``` for alarms and delays evaluation, and ```make APPLICATION=time_preemptive run``` to test the implemented scheduler. The applications run in an instance of QEMU, SiFive-E RISC-V machine.

## Tests done

-------------------------------------------------------
### Timers Initialization and Configuration
The timers were set to work with two different channels (Alarm and Scheduler), and configured with their respective handlers. The timers use the MTIME and MTIMECMP registers to keep track of time_stamps and to launch timer interrupts. The correct configuration can be inferred in the subsequent tests since timers are the base for the other points evaluated in this delivery.

-------------------------------------------------------
### Timer Handler
The timer handler calls again config() for setting MTIME and MTIMECMP registers and keeps triggering interruptions. The handler also calls the scheduler and alarms handlers.

-------------------------------------------------------
### Alarms Configuration
To test the alarms functioning we used the available alarm_test already implemented in EPOS source tree. It programs two distinct alarms and a delay, and alternates the handlers execution according to the programmed time.

-------------------------------------------------------
### Interrupts Handling
For handling the interruptions, we used the mtvec for setting the address of `_int_entry`. It saves all registers, and calls the exception_handler. If the mcauses says that it is an interruption it calls the handler for the given interruption_id, else, it calls PANIC() because it is an untreated exception.

-------------------------------------------------------
### Time Preemptive Scheduler
The Scheduling algorithm implemented for this delivery was the Feedback Scheduling (FS) algorithm. It consists in a dynamic, time-preemptive algorithm that uses the idea of multiple queues to realize the scheduling. In our implementation we simulated the multiple queues as different priorities, where greater priority threads execute first, and threads with the same priority are scheduled in a Round-Robin manner. It is a dynamic algorithm, where the dynamic change is given by the thread behavior: CPU-Bound threads that consume their QUANTUMs tend to decrease their priority and go to lower queues, while IO-Bound threads that use only a part of their QUANTUM go up. These changes are made using a Priority based Scheduling Criteria and updates using demote/promote to adjust the priorities.
In the ```time_preemptive``` application, we create three distinct threads: A and B that simulate CPU-Bound threads and only execute a while(true) loop; and C
that simulates an IO-Bound thread and calls a Delay to be awaken by an Alarm on each execution.
<Describe problem with threads scheduling>


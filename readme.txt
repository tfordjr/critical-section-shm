Terry Ford Project 2 - master/slave using shm to access to critical sections one at a time 09/17/2023
// Github: https://github.com/tfordjr/critical-section-shm.git

How to run my program:
make command will generate executables master and slave
./reverse command with required -t arg will run the program.
sample command to run the program: ./master -t 60 10
where -t arg defines how many seconds before master tires of waiting and kills all children,
and where the final arg is the number of processes created that will access critical section cstest

Implementation:
source code files are: master.c, slave.c, config.h, shm.h 
master.c creates specified number of children, logs to logfile.master.PID
slave.c dictates children to take turns writing to cstest by waiting and checking repeatedly
config.h defines timeoutSeconds and maxChildren variables
shm.h defines initshm and openshm, and keeps shared memory details all in one spot where it can be easily modified
Also included are readme.txt and Makefile, which uses pattern rules. 


Notable inclusions are:
perror() messages for each file open operation and for failed forks
modular design philosophy to increase readability, reusability and ease of use
Makefile with pattern rules, make operation raises no warnings. make clean removes all logfiles, executables
Appropriate comments that briefly explain implementation and methods used
Minimal shared server resources used by only utilizing 1 byte (boolean) to successfully coordinate child processes
Minimal redundant code. Most is designed modularly and reused effectively.
shared memory is a small size and always properly allocated, detached, and deallocated. 
Zombie processes are never spawned from my project.

Concerns:
I believe the assignment instructions want us to let our child processes waste time in the critical resource,
and I don't understand what purpose this serves at all. I believe this implementation implements the slave.c source
file as intended, where the child process does waste time within the critical resource, but I don't understand what 
that accomplishes.

I could easily make a more efficient child traffic control process using semaphores or even this same implementation
but without the wait times! I just don't understand why this style of implementation is the objective for this project.

Notable exclusions:
When incorrect number of arguments are given, program often segfaults.
cstest is commonly empty when program suffers timeout termination, I assume because child is killed with cstest open 
     and that erases the file. Not sure if that can be fixed. 
Child processes always write to critical resource 5 times in succession and then terminate. I'm not sure why this is
     the case. I don't know how child processes are always to first to access critical selection after leaving it. 
config.h does assign maxChildren and timeout default values, but they are only used if values provided are inelligible. 




Be sure to use ipcs to check server resources
and ipcrm -a to remove all stray resources allocated to you if need be

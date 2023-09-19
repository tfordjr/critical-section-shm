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

Notable exclusions are:
slave.c waiting in the critical section. I'm not sure what purpose this serves. The slave.c implementation that I have uses delays to allow for other processes to access critical resource before attempting to enter critical resource itself. 
From what I understand, the instructions want the child process to waste time within the critical resource. I don't understand what purpose it serves for a child process to gain access to a critical resource and waste time in it to deprive other processes access to the critical resource. I would like to understand that requirement better before implementing it. 





Be sure to use ipcs to check server resources
and ipcrm -a to remove all stray resources allocated to you if need be

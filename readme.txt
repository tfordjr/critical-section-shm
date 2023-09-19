Terry Ford Project 2 - master/slave using shm to access to critical sections one at a time 09/17/2023
Git: No git repo-not required in the project 2 specifications.

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
Makefile with pattern rules, make operation does not raise compile warnings or errors
Appropriate comments that briefly explain implementation and methods used

Notable exclusions are:


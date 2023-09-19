// Terry Ford Jr - CS 4760 - Project 2 - 09/17/23


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include <sys/wait.h>
#include "config.h"
#include "shm.h"

void help();
void sigCatch(int);
void timeout(int);
void logfile();
void forkandwait(int);

int main(int argc, char** argv){
	int option;   
  	int numChildren; 
   	if (argc < 3) {   //  Helps to prevent users from providing too few arguments
		perror("master: Error: Not enough arguments provided\n");
		return 0;
	}

        while ( (option = getopt(argc, argv, "t:")) != -1) {    // Only one arg: t- 
                switch(option) {				
                  	case 't':                    
		    		for (int i = 1; i < argc; i++) {   //  cycles through args to find -t seconds 
       			 		if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            		 			timeoutSeconds = atoi(argv[i + 1]);  // casts str to int
            	    	 			break;
        	         		}
		    		}			   	 
	  	  	default:
		  		for (int i = 1; i < argc; i++) {    // Cycles to find final numChildren arg
					if (strcmp(argv[i], "-t") == 0 && i + 2 < argc) {
                        			if (atoi(argv[i + 2]) > maxChildren){
							numChildren = 20;//  Does not allow more than 20 children
							printf("Too many children provided! ");
							printf("Proceeding with %d children\n", maxChildren);
						} else {
							numChildren = atoi(argv[i + 2]);    // Casts arg to int
						}					
					break;
                        		} 
		  		}
		}
	}

	bool* lock = initshm(); // initialize shared memory and return referent to it so we can deallocate later

	signal(SIGINT, sigCatch);    // Catch ctrl+c signal 
	signal(SIGALRM, timeout);    // Catch timeout signal
	alarm(timeoutSeconds);

	forkandwait(numChildren);    // handles child and parent actions after fork

	shmdt(lock);
	deallocateshm();	// Detach and deallocate shared memory
	logfile();
	return 0;	
}
            
void sigCatch(int signum) {     // Interrupt condition
	printf("Cannot end program with Ctrl+C\n");
	signal(SIGINT, sigCatch);
}

void timeout(int signum){       // Timeout condition
	printf("Timeout has occured. Now terminating all child processes.\n ");
	logfile();
	shmdt(lock);
	deallocateshm();	// Detach and deallocate shared memory before termination 
	kill(0, SIGKILL);       // Kill all child processes
}

void logfile(){             // Master log file generation
        char t[9];
        strftime(t, sizeof(t), "%T", localtime(&(time_t){time(NULL)}));

        char outfile[22];
        snprintf(outfile, sizeof(outfile), "logfile.master.%d", getpid());

        FILE* outputfile = fopen(outfile, "a");
        fprintf(outputfile, "%s Master process terminated - master(parent) pid: %d\n", t, getpid());
        fclose(outputfile);
}

void forkandwait(int numChildren){
	signal(SIGINT, sigCatch);

	for (int i = 0; i < numChildren; i++) {
	
		pid_t childPid = fork();  // Create Child here

		if (childPid == 0 ) {                 // Each child uses exec to run ./slave	
		 	if(execl("./slave","slave", (char *)NULL) == -1) {   
				perror("Child execution of slave failed ");				
			}	
			exit(0);
				
		} else 	if (childPid == -1) {  // Error message for failed fork (child has PID -1)
            		perror("master: Error: Fork has failed!");
            		exit(0);
        	}       
		wait(NULL);  // Parent waits to assure children perform in order
    	}
		
	for (int i = 0; i < numChildren; i++) { 
		wait(NULL);	// Parent Waiting for children
	}
	printf("Child processes have completed.\n");
}

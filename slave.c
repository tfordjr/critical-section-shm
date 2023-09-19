#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "shm.h"

void logfile();

int main(){	
	
	FILE* outputfile = fopen("cstest", "a");
	if (!outputfile) {   // perror file open failure
                perror("slave: Error: File open operation failed!");
                exit(0);
        }

	srand(getpid());  // This ensures that even if children happen simultaneously, they have different delays
	int randomDelay = rand() % 3 + 1;

	bool* lock = openshm();   // opening existing shared memory in child process

	for (int i = 0; i < 10; i++){    // Try the lock 10 times
		sleep(randomDelay);	
		if(!(*lock)){		 // If unlocked				
			*lock = 1;	 // Lock it
			fprintf(outputfile, "%s File modified by process number %d\n", t, getpid()); // access
			*lock = 0;	 // Unlock it	
			shmdt(lock);     // detatch from shared memory
			logfile();       
			fclose(outputfile);
			exit(0);	
		}		
		sleep(randomDelay);
	}
	perror("slave: Error: Child never got to write to file\n");
	fclose(outputfile);
	return 0;
}

void logfile(){
	char t[9];
	strftime(t, sizeof(t), "%T", localtime(&(time_t){time(NULL)}));   // Init t with HH:MM:SS
	
	char outfile[16];
	snprintf(outfile, sizeof(outfile), "logfile.%d", getpid());  // generate logfile name with pid

	FILE* outputfile = fopen(outfile, "a");
	fprintf(outputfile, "%s File modified by process number %d\n", t, getpid());   // log to logfile
	fclose(outputfile);
}


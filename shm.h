#ifndef SHM_H
#define SHM_H

#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_NAME "/shared_memory"
#define SHM_SIZE sizeof(bool) 

bool* initshm(){
        // Allocates shared memory boolean lock. lock will ensure one child works at a time on cstest
        int shmid = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);   //  Creating shared memory
        if (shmid == -1) {
                perror("master: Error: shmget failed\n");
                exit(0);
        }

        int trunc = ftruncate(shmid, SHM_SIZE);
        if (trunc == -1){
                perror("master: Error: truncation failed\n");
                exit(0);
        }

        bool* lock = (bool*)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0);
        if (lock == MAP_FAILED) {
                perror("master: Error: mmat failed\n");
                exit(0);
        }

        *lock = 0; // Initializing lock to the 'unlocked' position so children can gain access to critical section
     	return lock;   
}

bool* openshm(){	
	int shmid = shm_open(SHM_NAME, O_RDWR, 0);    //Creating shared memory
        if (shmid == -1) {
                perror("slave: openshm(): Error: shm_open failed\n");
                exit(0);
        }

        bool* lock = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmid, 0); //  Attaching Shared Memory
        if (lock == MAP_FAILED) {
                perror("slave: openshm(): Error: mmap failed\n");
                exit(0);
        }
	return lock;
}

void deallocateshm(){
	if(shm_unlink(SHM_NAME) == -1){
		perror("shm.h: Error: shm_unlink falied\n");
		exit(0);
	}
}

#endif

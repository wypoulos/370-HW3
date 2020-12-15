// coordinator.c
// Wyatt Poulos


#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>

int main(int argc, char *argv[])
{
	
	//Check to make sure there is a valid number of arguments
	if(argc < 3){
		printf("[Coordinator] Not enough arguments!\n");
		return 0;
	}	

	int childSharedMemIds[argc - 2], prossID[argc - 2];

	//Iterate throught the arguments starting with first dividend 
	for(int i = 2; i < argc; i++){
		
		//Array for pipe to create read and write ends
		int fd[2];

		char filething[10];
		
		// fd[0] - read
		// fd[1] - write
		//Creating pipe
		if(pipe(fd) == -1){
			printf("Coordinator: An error ocurred with opening the pipe\n");
		}
		
		//Forking and obtaing processID
		prossID[i - 2]= fork();
		

		if(prossID[i - 2] == -1){
			printf("Coordinator: An error occured with fork\n");
		}
		//If the process id is 0 we are in the child and need to do child things
		if(prossID[i - 2] == 0){
			snprintf(filething, 10, "%d",  fd[0]);
			execlp("checker","checker",argv[1],argv[i],filething,NULL);
			
		//If the process id is not 0 we are in the parent and need to do parent things
		}else{
			printf("Coordinator: forked process with ID %d.\n", prossID[i-2]);

			//Creating shared memory 
			childSharedMemIds[i - 2] = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
			printf("Coordinator: wrote shm ID %d to pipe (4 bytes)\n", childSharedMemIds[i - 2]);

			//Writing shared memory id to pipe for child to read
			write(fd[1], &childSharedMemIds[i - 2], sizeof(int));

			//Closing write end of the pipe
			close(fd[1]);
			
		}
		
	}

	for(int i = 2; i < argc; i++){
		printf("Coordinator: waiting on child process ID %d...\n", prossID[i - 2]);	
		waitpid(prossID[i - 2],NULL,0);
		
		int *sh;

		//Obtaing the 1 or 0 from the shared memory location and attaching to the chared memory	
		sh = (int*)shmat(childSharedMemIds[i - 2],NULL,0);

		
		//Interpereting results
		if(*sh == 1){
			printf("Coordinator result 1 read from shared memory: %d is divisible by %d.\n",atoi(argv[i]),atoi(argv[1]));
		}
		if(*sh == 0){
			printf("Coordinator result 0 read from shared memory: %d is not divisible by %d.\n",atoi(argv[i]),atoi(argv[1]));
		}

		//Controlling the shared memory in this case marking it to be destroyed
		shmctl(childSharedMemIds[i - 2], IPC_RMID,NULL);

		//Detaching from shared memory
		shmdt(sh);	
	}

	//End of program!
	printf("Coordinator exiting.\n");
	return 0;
}

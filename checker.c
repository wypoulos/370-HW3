// checker.c
// Wyatt Poulos
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/shm.h>

int main(int argc, char *argv[]){

	//Getts the child process id and puts it in processID for later use
	int processID = getpid();
	
	printf("Checker process [%d]: Starting.\n", processID);
	
	//Using the atoi function we convert from a char* to an int
	//atoi() returns a 0 if what is given is a bad value
	int bottom = atoi(argv[1]);
	int top = atoi(argv[2]);
	int pipeRead = atoi(argv[3]);
	
	//If the bottom equals zero that a problem because zero cannot be in the denominator 
	// && if the argument is equal to zero then we know its definitly a zero and not an atoi error
	if(bottom == 0 && strcmp(argv[1], "0")==0){
		printf("Checker process[%d]: Error ZERO in DENOMINATOR.\n", processID);
		printf("Checker process [%d]: Returning 0. \n", processID);
		return 0;
	//Appon passing the first if statment it needs to pass this one to check for non atoiable values
	}else if(bottom == 0){
		printf("Checker process [%d]: Invalid Entry -> %s. \n", processID, argv[1]);
		printf("Checker process [%d]: Returning 0. \n", processID);
		return 0;
	}
	
	//Similar to the above if statements but for top. We need to check if the value passed in was
	//something like a letter or string so that we can catch it not spit out a bad value
	if(strcmp(argv[2],"0") != 0 && top == 0){
		printf("Checker process [%d]: Invalid Entry -> %s. \n", processID, argv[2]);
		printf("Checker process [%d]: Returning 0. \n", processID);
		return 0;		
	}

	//Variables for shared memory 
	int sharedMemID, *shmated;
	//Reading the location of the shared memory in from pipe
	read(pipeRead, &sharedMemID, sizeof(int));
	//Closing the read end of the pipe
	close(pipeRead);
	//Attaching to shared memory
	shmated = (int*)shmat(sharedMemID,NULL,0);
	printf("Checker process [%d]: read 4 bytes containing shm ID %d\n", processID, sharedMemID);

	//Finally after those checks we can be confident that the arguments passed are numbers and perform
	//a check to see if the top is dividisble by the bottom
	if(top % bottom == 0){
		printf("Checker process [%d]: %d *IS* divisible by %d.\n", processID, top, bottom);
		printf("Checker process [%d]: wrote result (1) to shared memory. \n", processID);
		//Return 1
		*shmated = 1;
		
	}else{
		printf("Checker process [%d]: %d *IS NOT* divisible by %d.\n", processID, top, bottom);
		printf("Checker process [%d]: wrote result (0) to shared memory. \n", processID);
		*shmated = 0;
	}
	
	//detaching from shared memory
	shmdt(shmated);
	return 0;
}

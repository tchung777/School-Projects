/**
 * @program	19 Child Processes
 * @brief	This is a program that creates 19 child processes. How I went about this was by first creating 18 child              
 * processes from the original root using recursion. The predicted process tree has 2 branches of generation 1's, 2 branches  
 * of generation 2's, and 3 branches of generation 3's. Then I created one more fork() from the original root but proceed 
 * to stunt it using the clone parameter to make a total of 19 processes.
 *
 * @copyright	Created by Thomas Chung on 10/5/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define NUM_PROC 3
#define TOTAL_PROC 9
int global;
int clone = 1;

/**
 * @function	Spawn
 * @brief       This function generates 18 process children, given a starting parameter of 2. The idea is that there's a 
 * desired shape for the process tree we want in order to have 19 child processes. To first have 18 processes, we want to 
 * create a 3 generation tree with the first generation be two branches, the second generation, be 2 branches, and the third
 * be three. 
 */

void spawn(int this_fn, int proc_count){
	int i, status;
	global++;
	if(global == 3)
		proc_count = 3;
	for (i = 0; i < proc_count && this_fn == getpid() && proc_count > 1 && global <= 3; i++){
		int pid = fork();
		if (pid < 0) continue;
		else if (pid == 0){
			printf("%d Process %d spawned by process %d\n",global, getpid(),getppid());
			spawn(getpid(), proc_count);
		}else
			waitpid(-1,&status,0);
	}
}

/**
 * @function	Program Main
 * 
 */

int main(int argc, char *argv[]) {
	if(clone == 2) return 0;
	int status;
	clone = 2;
	if(fork() == 0)
		printf("%d Process %d spawned by root process %d\n",global, getpid(),getppid());
	else {
		printf("Main process starting at %d\n", getpid());
		spawn(getpid(), 2);
	}
	
}
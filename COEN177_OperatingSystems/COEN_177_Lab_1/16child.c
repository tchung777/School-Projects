/**
 * @program 16 Child
 * @brief This function creates 16 child processes. It utilizes a recursive function to create a linear tree.
 * It evaluates an input parameter and proceeds to recursively call fork() until we reach our reachNUM constant.
 * In this case, it is 16. 
 *
**/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define reachNUM 16

int spawn(int i) {
	//fork() replicates the original program as a child but assigns a unique ID process to this child
	pid_t pid = fork();
	int status;
	
	if(pid < 0) return 1;
	//If you're a parent then just wait around
	if(pid > 0)
		waitpid(-1, &status, 0);
	else {
		printf("%d Child ID %d created by parent %d. \n",i, getpid(), getppid());
		
		if(i < reachNUM)
			spawn(++i);
	}
	
	return 0;
}

int main(int argc, char *argv[]) {
	return spawn(1);
}
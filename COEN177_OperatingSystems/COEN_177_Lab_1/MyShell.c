/* @program My Shell
 * @brief This program simulates a Unix shell. It was created for the purpose of
 * using system calls to create child processes as well as launch executables.
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

#define MAX_CMD_LENGTH 100

/**
 * @function Exit Program
 * @brief This function determines if the shell should terminate if "exit" is inputted
 *
**/
int exitProgram(char *cmd) {
	const char *exitChar = "exit";
	if(strcmp(cmd, exitChar) == 0){
		return 1;
	}
	return 0;
}

/**
 * @function Program Main
**/

int main(int argc, char *argv[]) {
	while(1) {
		printf("~$ ");
		char buffer[MAX_CMD_LENGTH] = {0};
		char cmd[MAX_CMD_LENGTH] = {0};
		
		/*Use fgets() over gets because you can't enter the size of the buffer as a parameter into gets(), 
		 *allowing the user to overflow the buffer i.e: inputing a string larger than the buffer
		 *char *fgets(char * restrict str, int size, FILE * restrict stream);
		 *char *gets(char *str);*/
		
		fgets(buffer, sizeof(buffer),stdin);
		
		/*Got to strip of the end of the buffer because fgets includes newline at the end*/
		buffer[strlen(buffer)-1] = '\0';
		
		/* Transferring from buffer to command*/
		if (strlen(buffer) > 0){
			int numargs = 0;
			int i;
			/* Finds the number of arguments following the command 
			 */
			for (i = 0; buffer[i]; i++){
				if (buffer[i] == ' ')
					numargs++;
			}
			/* Parses the command input to obtain the command and the
			 * program parameters passed into the command input
			 */
			char *args[numargs+2];
			char *tempstr = strtok(buffer, " ");
			strcpy(cmd, tempstr);
			args[0] = tempstr;
			tempstr = strtok(NULL, " ");
			for (i = 0; i < numargs; i++){
				strcpy(args[i+1], tempstr);
				tempstr = strtok(NULL, " ");
			}
			
	    	/* Appends a terminating character to the argument list */
			args[numargs+1] = '\0';
			
			/* Evaluate processes */
			if(!exitProgram(cmd)) {
				/*fork() returns a value of pid_t, use var pid to hold the ProcessID of your process*/
				pid_t pid = fork(); 
				if(pid > 0) {
					int status;
					/*PID greater than 0 and is not zero, then it's successful and pid = ProcessID of Child*/
					waitpid(-1,&status, 0);
					
				} else if(pid == 0) {	
					/*PID is 0 then you're the child process and you have no more children
					 *getppid to get Parent's PID
					
					 *if execvp returns a value other than 0, we have an error. Refer to execvp page.*/
					execve(cmd, args, 0);
					return 0;
				}		
				
				/*if PID is negative, child creation failed.*/
				
				else {
					return 1;
				}
			} else return 0;
		}
		
	}
}
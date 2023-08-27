#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "task1_functions.c"

/**
	@brief Forking the bash to launch the desired command in the child process while parent waits.
	@param elements Null terminated list of arguments (including program).
	@return 1 (program continues running).
 */
int forking(char** elements)
{
	pid_t pid;
	int status;
	
	//pid = fork();

	if ((pid = fork()) < 0) {	//forking error
		perror("matthew");
		exit(1);
	} else if (pid == 0) {		//child process
		if (execvp(elements[0], elements) == -1) {
      			perror("matthew");
    		}
    		exit(1);
	} else {			//parent process
		do {
      			waitpid(pid, &status, WUNTRACED);
    		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

  	return 1;
}

/**
	@brief Execute shell function or fork.
	@param elements Null terminated list of arguments.
   	@return 1 when shell should continue running, 0 when it should terminate
 */
int execute(char** elements)
{
	//if command is empty
	if (elements[0] == NULL) {
    		return 1;
  	}

  	for (int i = 0; i < (sizeof(function_str) / sizeof(char*)); i++) {
    		if (strcmp(elements[0], function_str[i]) == 0) {
      			return (*functions[i])(elements);
    		}
  	}

  	return forking(elements); 
}

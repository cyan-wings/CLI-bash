#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "read_command.c"
#include "task1_execute.c"

/**
	@brief function that keeps the bash running.
	1. Reads the user's input.
	2. Split them into elements.
	3. Execute the arguments based on its functionality.
	4. Empties the read command and splited command from the memory.
 */
void matthews_bash()
{
	char* command;
	char** elements;
	int status = 1;

	while (status) {
		printf("$ ");
		command = read_command();
		elements = split_command(command);
		status = execute(elements);
		free(command);
		free(elements);
	}
}

/**
	@brief Main function which starts the program.
	@return status of the program.
 */
int main ()
{
	matthews_bash();

	return 0;
}

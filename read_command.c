#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MATTHEW_COM_SIZE 1024
#define MATTHEW_TOK_SIZE 64
#define MATTHEW_TOK_DELIMITER " \n\t"

/**
   	@brief Reads the command the user inputs. Reads the input character by character, allocating more space to the buffer variable when space exceeds
   	@return The command line from stdin.
 */
char* read_command()
{
	int index = 0;
	int size = MATTHEW_COM_SIZE;
	char* buffer = malloc(sizeof(char) * size);
	int x;
	
	if (!buffer) {
    		fprintf(stderr, "matthew: Allocation error!\n");
    		exit(1);
  	}
	
	//Reading the input string character by character
	while (1) {
		x = getchar();

		if (x == EOF) {
			exit(0);
		} else if (x == '\n') {
			buffer[index] = '\0';
			return buffer;
		} else {
			buffer[index] = x;
		}

		index++;

		//Reallocate when buffer exceeds.
		if (index >= size) {
      			size += MATTHEW_COM_SIZE;
      			buffer = realloc(buffer, size);

      			if (!buffer) {
        			fprintf(stderr, "matthew: Allocation error!\n");
        			exit(1);
      			}
    		}
	}
}

/**
   	@brief Split the command into tokens. Allocates more space when the array of tokens exceed.
   	@param command The command from read_command()
   	@return Null-terminated array of tokens.
 */
char** split_command(char* command)
{
	int size = MATTHEW_TOK_SIZE;
	int index = 0;
	char* tok;
	char** tok_s = malloc(sizeof(char*) * size);

	if (!tok_s) {
    		fprintf(stderr, "matthew: Allocation error!\n");
    		exit(1);
  	}

	tok = strtok(command, MATTHEW_TOK_DELIMITER);

	while (tok != NULL) {
		tok_s[index] = tok;
		index++;

		if (index >= size) {
			size += MATTHEW_TOK_SIZE;
			tok_s = realloc(tok_s, sizeof(char*) * size);
			
			if (!tok_s) {
    				fprintf(stderr, "matthew: Allocation error!\n");
    				exit(1);
  			}
		}

		tok = strtok(NULL, MATTHEW_TOK_DELIMITER);
	}

	tok_s[index] = NULL;

  	return tok_s;
}




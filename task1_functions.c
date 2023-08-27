#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>


/*
  Function declarations for the shell commands
 */
int matthew_clear(char** elements);
int matthew_cd(char** elements);
int matthew_ls(char** elements);
int matthew_echo(char** elements);
int matthew_help(char** elements);
int matthew_quit(char** elements);

/*
  List of function commands.
 */
char* function_str[] = {
	"clear",
  	"cd",
	"ls",
	"echo",
  	"help",
  	"quit"
};

/*
  Array of function pointers accoridng to their respective commands.
 */
int (*functions[]) (char**) = {
	&matthew_clear,
  	&matthew_cd,
	&matthew_ls,
	&matthew_echo,
  	&matthew_help,
  	&matthew_quit
};

/**
   @brief Builtin command: clear.
   @param elements List of elements.  Not examined.
   @return 1, to continue the shell system.
 */
int matthew_clear(char** elements)
{
	printf("\033[H\033[J");
	return 1;
}

/**
   @brief Function command: change directory.
   @param elements List of elements.  elements[0] is "cd".  elements[1] is the directory.
   @return 1, to continue the shell system.
 */
int matthew_cd(char** elements)
{
  	if (elements[1] == NULL) {
    		fprintf(stderr, "matthew: expected argument to \"cd\"\n");
  	} else {
    		if (chdir(elements[1]) != 0) {
      			perror("matthew");
    		}
  	}
  	return 1;
}

//Get current working directory path declaration
char* getcwd(char *buf, size_t size);

/**
   @brief Function command: List files of the <directory>(argument). If elements[1] is NULL, the current working directory path will be displayed.
   @param elements List of elements.  elements[0] is "ls".  elements[1] is the directory. 
   @return 1, to continue the shell system.
 */
int matthew_ls(char** elements)
{
	char cwd[1024];
	DIR *dp;
	struct dirent *ep;
	if (elements[1] == NULL) {
		if (getcwd(cwd, sizeof(cwd)) != NULL) {
			dp = opendir (cwd);
		}
	} else {
		dp = opendir (elements[1]);
	}
	if (dp != NULL) {
		while ((ep = readdir(dp)) != NULL) {
			printf("%s\t", ep->d_name);
		}
		printf("\n");
	} else {
		perror("matthew");
	}
	closedir (dp);
	return 1;
}

/**
   @brief Function command: Echo the elements[1...].
   @param elements List of elements.  elements[0] is "echo".  elements[1...] is the elements to be echoed in the shell.
   @return 1, to continue the shell system.
 */
int matthew_echo(char** elements)
{
	if (elements[1] != NULL) {
    		for (int i = 1; i < 15; i++) {
			if (elements[i] != NULL) {
				printf("%s ",elements[i]);
			}
		}
  	}
	
	printf("\n");
  	return 1;

}

/**
   @brief Builtin command: Manual for the Matthew's Bash.
   @param elements List of elements.  Not examined.
   @return Always 1, to continue the shell system.
 */
int matthew_help(char** elements)
{
	int i;
  	printf("Matthew's Bash\n");
  	printf("Builtin funcitons of this shell:\n");

  	for (i = 0; i < (sizeof(function_str) / sizeof(char*)); i++) {
    		printf("  %s\n", function_str[i]);
  	}

  	return 1;
}

/**
   @brief Builtin command: exit.
   @param elements List of elements.  Not examined.
   @return 0, to terminate the shell system.
 */
int matthew_quit(char** elements)
{
  	return 0;
}

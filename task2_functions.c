#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

/*
  Function declarations for the shell commands
 */
int matthew_clear(char** elements);
int matthew_cd(char** elements);
int matthew_ls(char** elements);
int matthew_echo(char** elements);
int matthew_help(char** elements);
int matthew_quit(char** elements);
int matthew_create(char** elements);
int matthew_copy(char** elements);
int matthew_search(char** elements);
int matthew_run(char** elements);
int matthew_halt(char** elements);

/*
  List of function commands.
 */
char* function_str[] = {
	"clear",
  	"cd",
	"ls",
	"echo",
  	"help",
  	"quit",
	"create",
	"copy",
	"search",
	"run",
	"halt"
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
  	&matthew_quit,
	&matthew_create,
  	&matthew_copy,
	&matthew_search,
	&matthew_run,
  	&matthew_halt
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

/**
   @brief Builtin command: Create file with the name elements[1] (second argument of command input). 
   @param elements elements[0] is "create".  elements[1] is the name of the file to be created. 
   @return 1, to continue the shell system.
 */
int matthew_create(char** elements)
{
	if (elements[1] == NULL) {
		fprintf(stderr, "matthew: expected argument to \"create\"\n");
	} else {
		int f = open(elements[1], O_RDWR | O_CREAT, 0666);
		if (f != -1) {
			printf("matthew: %s created\n",elements[1]);
		} else {
			perror("matthew: Create file fail.");
		}
	}
	return 1;
}

/**
   @brief Builtin command: Copy file contents from elements[1] (second argument of command input) to  elements[2] (third argument of command input).
   @param elements elements[0] is "copy". elements[1] is the name of the file to be read. elements[2] is the name of the file to be written.
   @return 1, to continue the shell system.
 */
int matthew_copy(char** elements)
{
	FILE *rfp, *wfp;
	int c;
	
	if (elements[1] == NULL) {
		fprintf(stderr, "matthew: expected argument to \"copy\"\n");
	} else {
		rfp = fopen(elements[1],"r");
		if (rfp == NULL) {
			printf("matthew: Error reading file %s\n", elements[1]);
		} else {
			wfp = fopen(elements[2],"w");
			if (wfp == NULL) {
				printf("matthew: error in opening file to be written\n");
			} else {
				do {
					c = fgetc(rfp);
					fputc(c, wfp);
				} while (feof(rfp) != 1);
				printf("matthew: Copy file %s to file %s successful.\n",elements[1], elements[2]);
			}
		}
	}

	fclose(rfp);
	fclose(wfp);

	return 1;
}

/**
   @brief Builtin command: Search the sequence of characters in elements[1] in the file (elements[2]) and display it. File full path has to be specified ::EXAMPLE: "search cc /home/matthew/27033074_A1/main2.c" which searches the occurences of string, "cc", in file (elements[2]).
   @param elements elements[1] is "search". elements[1] is the sequence of characters to be read. elements[2] file that will be used for this function command.
   @return 1, to continue the shell system.
 */
int matthew_search(char** elements)
{
	if (elements[1] != NULL) {
		if (elements[2] != NULL) {
			pid_t pid = fork();
			int status;
			if (pid == 0) {
				char* argv[] = { "grep", "-o", elements[1], elements[2],NULL};
				if (execvp(argv[0],argv) == -1) {
      					fprintf (stderr, "matthew: no such file. Enter full path of executable.\n");
    				}
				
    				exit(1);
			} else if (pid < 0) {
				perror("matthew");
				exit(1);
			} else {
				do {
      					waitpid(pid, &status, WUNTRACED);
    				} while (!WIFEXITED(status) && !WIFSIGNALED(status));
			}
		} else {
			fprintf(stderr, "matthew: expected argument to \"search\"\n");
		}
	} else {
		fprintf(stderr, "matthew: expected argument to \"search\"\n");
	}
	
	return 1;
}

/**
   @brief Builtin command: Run file (elements[1]). File full path has to be specified ::EXAMPLE: "run /home/matthew/27033074_A1/task1" which runs the executable program task1. 
   @param elements elements[0] is "run". elements[1] is the file path of the executable.
   @return 1, to continue the shell system.
 */
int matthew_run(char** elements)
{
	if (elements[1] != NULL) {
		pid_t pid = fork();
		int status;
		if (pid == 0) {
			if (execvp(elements[1],elements) == -1) {
      				fprintf (stderr, "matthew: no such file. Enter full path of executable.\n");
    			}
    			exit(1);
		} else if (pid < 0) {
			perror("matthew");
			exit(1);
		} else {
			do {
      				waitpid(pid, &status, WUNTRACED);
    			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	} else {
		fprintf(stderr, "matthew: expected argument to \"run\"\n");
	}

	return 1;
}

/**
   @brief Builtin command: Terminate file (elements[1]). File full path has to be specified ::EXAMPLE: "halt /home/matthew/27033074_A1/task1" which runs the executable program task1. 
   @param elements elements[0] is "halt". elements[1] is the file path of the executable.
   @return 1, to continue the shell system.
 */
int matthew_halt(char** elements)
{
	if (elements[1] != NULL) {
		pid_t pid = fork();
		int status;
		if (pid == 0) {
			char* argv[] = { "pkill", elements[1],NULL};
			if (execvp(argv[0],argv) == -1) {
      				fprintf (stderr, "matthew: no such file. Enter full path of executable.\n");
    			}
    			exit(1);
		} else if (pid < 0) {
			perror("matthew");
			exit(1);
		} else {
			do {
      				waitpid(pid, &status, WUNTRACED);
    			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
		}
	} else {
		fprintf(stderr, "matthew: expected argument to \"halt\"\n");
	}

	return 1;
}

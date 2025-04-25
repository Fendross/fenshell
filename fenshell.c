#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "fenshell.h"

/*
This shell is missing some features, for example:

- Only whitespace separating arguments, no quoting or backslash escaping.
- No piping or redirection.
- Few standard builtins.
- No globbing.

TODO list:

- mkdir builtin --> DONE
- rmdir builtin --> NOT STARTED
*/

// List of builtin commands.
char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "mkdir"
};

// Array of function pointers.
int (*builtin_func[]) (char **) = {
    &fenshell_cd,
    &fenshell_help,
    &fenshell_exit,
    &fenshell_mkdir
};

int fenshell_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// Function implementations.
void fenshell_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        printf("fenshell> ");
        line = fenshell_read_line();
        args = fenshell_split_line(line);
        status = fenshell_execute(args);

        free(line);
        free(args);
    } while (status);
}

// This function can be written using the getline() function of stdio.h instead of getting a single char at a time.
char *fenshell_read_line(void) {
    int bufsize = FENSHELL_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c; // This is because EOF (CTRL-D) is an int.

    check_buffer_allocation(buffer);

    while(1) {
        // Read a character.
        c = getchar();

        // If EOF then replace it with a null char and return.
        if (c == EOF || c == '\n') { 
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        // Reallocate if buffer has been exceeded.
        if (position >= bufsize) {
            bufsize += FENSHELL_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            check_buffer_allocation(buffer);
        }
    }
}

void check_buffer_allocation(char *buffer) {
    if (!buffer) {
        fprintf(stderr, "fenshell: buffer allocation error\n");
        exit(EXIT_FAILURE);
    }
}

char **fenshell_split_line(char *line) {
    int bufsize = FENSHELL_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    check_tokens_allocation(tokens);

    // Returns pointers to within the string, and appends \0 bytes at the end of each token.
    token = strtok(line, FENSHELL_TOK_DELIMITER);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        // Reallocate if tokens has been exceeded.
        if (position >= bufsize) {
            bufsize += FENSHELL_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            check_tokens_allocation(tokens);
        }

        token = strtok(NULL, FENSHELL_TOK_DELIMITER);
    }

    // No token has been returned by strtok().
    tokens[position] = NULL;
    return tokens;
}

void check_tokens_allocation(char **tokens) {
    if (!tokens) {
        fprintf(stderr, "fenshell: tokens allocation error\n");
        exit(EXIT_FAILURE);
    }
}

int fenshell_launch(char **args) {
    pid_t pid, wpid;
    int status;

    // In order to start a new process, we first need to fork an existing one.
    pid = fork();
    if (pid == 0) {
        // Child process.

        // We use a variant of the system call exec(), execvp(), which expects a program name and an array (or vector) of string arguments.
        // The p indicates that the OS is going to search for the program, we don't provide the full path of the program to run.
        if (execvp(args[0], args) == -1) {
            perror("fenshell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking.
        perror("fenshell");
    } else {
        // Parent process.
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

// Builtin function implementations.
int fenshell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "fenshell: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("fenshell");
        }
    }
    return 1;
}

int fenshell_help(char **args) {
    int i;

    printf("Fendross' Fenshell\n");
    printf("Type program names and arguments, and hit enter!\n");
    printf("The following are built in:\n");

    for (i = 0; i < fenshell_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for info on other programs.\n");
    return 1;
}

int fenshell_exit(char **args) {
    return 0;
}

int fenshell_mkdir(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "fenshell: expected argument to \"mkdir\"\n");
    } else {
        if (mkdir(args[1], S_IRWXU) != 0) {
            perror("fenshell");
        }
    }
    return 1;
}

int fenshell_execute(char **args) {
    int i;

    if (args[0] == NULL) {
        // Empty command has been entered.
        return 1;
    }

    for (i = 0; i < fenshell_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return fenshell_launch(args);
}

// Main program.
int main(int argc, char **argv)
{
    // Load config files.
    // No config files to load for now.

    // Loop the Fenshell
    fenshell_loop();

    // Shutdown and cleanup functions.
    // No cleanup/shutdown functionalities for now.

    return EXIT_SUCCESS;
}

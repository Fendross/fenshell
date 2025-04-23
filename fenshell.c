#include <stdio.h>
#include <stdlib.h>

// Custom declarations.
#define FENSHELL_RL_BUFSIZE 1024

#define FENSHELL_TOK_BUFSIZE 64
#define FENSHELL_TOK_DELIMITER " \t\r\n\a"

// Function headers.
void fenshell_loop(void);
char *fenshell_read_line(void);
void check_buffer_allocation(char *buffer);
char **fenshell_split_line(char *line);
void check_tokens_allocation(char **tokens);

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
    tokens[position] = NULL;
    return tokens;
}

void check_tokens_allocation(char **tokens) {
    if (!tokens) {
        fprintf(stderr, "fenshell: tokens allocation error\n");
        exit(EXIT_FAILURE);
    }
}

// Main program.
int main(int argc, char **argv)
{
    // Load config files.
    // TODO - No config files to load for now.

    // Loop Fenshell
    fenshell_loop();

    // Shutdown and cleanup functions.
    // TODO - No cleanup/shutdown functionalities for now.

    printf("End of execution. Exiting...\n");
    return EXIT_SUCCESS;
}

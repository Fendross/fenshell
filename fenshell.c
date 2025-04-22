#include <stdio.h>
#include <stdlib.h>

void fenshell_loop(void);

char *fenshell_read_line(void);

void fenshell_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        printf("$ ");
        line = fenshell_read_line();
        args = fenshell_split_lines(line);
        status = fenshell_execute(args);

        free(line);
        free(args);
    } while (status);
}

#define FENSHELL_RL_BUFSIZE 1024
char *fenshell_read_line(void) {
    int bufsize = FENSHELL_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "fenshell: buffer allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        // Read a character.
        c = getchar();
    }
}

int main(int argc, char **argv)
{
    // Load config files.
    // TODO - No config files to load for now.

    // Loop Fenshell
    fenshell_loop();

    // Shutdown and cleanup functions.
    // TODO - No cleanup/shutdown functionalities for now.

    printf("End of execution.\n");
    return EXIT_SUCCESS;
}

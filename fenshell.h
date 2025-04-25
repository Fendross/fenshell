// Custom declarations.
#define FENSHELL_RL_BUFSIZE 1024

#define FENSHELL_TOK_BUFSIZE 64
#define FENSHELL_TOK_DELIMITER " \t\r\n\a"

// Function declarations.
void fenshell_loop(void);

char *fenshell_read_line(void);
void check_buffer_allocation(char *buffer);

char **fenshell_split_line(char *line);
void check_tokens_allocation(char **tokens);

int fenshell_launch(char **args);

int fenshell_num_builtins();
int fenshell_cd(char **args);
int fenshell_help(char **args);
int fenshell_exit(char **args);
int fenshell_mkdir(char **args);

int fenshell_execute(char **args);

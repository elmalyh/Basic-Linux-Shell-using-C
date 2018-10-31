

int readandparseline(char* args[],char line[],char* pipping_args[]);
void readline(char line[]);
void removeendofline(char line[]);
int parseline(char* temp[],char line[]);
int pipeandredirectionchecking(char* temp[]);
void check_line(char* temp[]);
void clearScreen();
int changeDir(char **args);
void myhandler(int mysignal);


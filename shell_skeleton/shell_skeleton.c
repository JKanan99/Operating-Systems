#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LINE_ARGS  128

int min(int a, int b) { return a < b ? a : b; }
// break a string into its tokens, putting a \0 between each token
//   save the beginning of each string in a string of char *'s (ptrs to chars)
//argc argument count?
int parse(char* s, char* argv[]) {
    int argc = 0;
    while (*s != '\0')
    {
        while (*s == ' ' || *s == '\t' || *s == '\n')
        {
            *s++ = '\0';
        }
        *argv++ = s;
        while (*s != '\0' && *s != ' ' && *s != '\t' && *s != '\n')
        {
            s++;
        }
    }
  argv[argc] = NULL;
  return argc;   // int argc
}

int execute(char* input, int cc) {
  int i = 0;
  char* shell_argv[MAX_CMD_LINE_ARGS];
  memset(shell_argv, 0, MAX_CMD_LINE_ARGS * sizeof(char));
  
  int shell_argc = parse(input, shell_argv);
   //printf("after parse, what is input: %s\n", input);      // check parser
   //printf("argc is: %d\n", shell_argc);
   //while (shell_argc > 0) {
   //  printf("argc: %d: %s\n", i, shell_argv[i]);
   //  --shell_argc;
   //  ++i;
   //}
  
  int status = 0;
  pid_t pid = fork();

  if(pid < 0) { fprintf(stderr, "Fork() failed\n"); }  // send to stderr
  else if (pid == 0) { // child
    int ret = 0;
    // if ((ret = execlp("cal", "cal", NULL)) < 0) {  // can do it arg by arg, ending in NULL
// TODO: fill in code for execvp(...)
    if (execvp(*shell_argv,shell_argv) < 0)
    {
       // printf("*** ERROR: exec failed\n");
       printf("You should have called execvp(...)\n");
       exit(1);
    }
//printf("You should have called execvp(...)\n");
  }
  else { // parent -----  don't wait if you are creating a daemon (background) process
      if (cc == 1){
        wait(NULL);
      }
      else{
      while (wait(&status) != pid) { }
      }
  }
  
  return 0;
}

int main(int argc, const char * argv[]) {
  char input[BUFSIZ];
  char last_input[BUFSIZ];  
  bool finished = false;
  int i=0;
  int concurrent = 0;
  char last_letter[1]; //to check &
  
  while(1){

  memset(last_input, 0, BUFSIZ * sizeof(char));  
  while (!finished) {
    memset(input, 0, BUFSIZ * sizeof(char));
    memset(last_letter, 0, 1 * sizeof(char));

    printf("osh > ");
    fflush(stdout);
    if (strlen(input) > 0) {
      memset(last_input, 0, BUFSIZ * sizeof(char));
    }
    if ((fgets(input, BUFSIZ, stdin)) == NULL) {   // or gets(input, BUFSIZ);
      fprintf(stderr, "no command entered\n");
      exit(1);
    }

    input[strlen(input) - 1] = '\0';          // wipe out newline at end of string

    //printf("Last character : %c", input[strlen(input)-1]);
    //strcpy(last_letter,input[strlen(input)-1]);
    //printf("lastletter was: '%s'\n", last_letter);

    if(strncmp(&input[strlen(input)-1], "&", 1) == 0)
    {
      concurrent = 1;
      input[strlen(input)-1] = '\0'; //delete the &
      input[strlen(input)-1] = '\0'; //delete the space
    }
    

    //printf("input was: '%s'\n", input);
    //printf("last_input was: '%s'\n", last_input);
    
    if (strncmp(input, "exit", 4) == 0) {   // only compare first 4 letters
      finished = true;
    } else if (strncmp(input, "!!", 2) == 0) { // check for history command
      //TODO
      if(strncmp(last_input, "\0", 2) == 0){
        fprintf(stderr, "no last command to execute \n");
      }
      else{
        execute(last_input, concurrent); 
      }

    } else { 
      execute(input, concurrent); 
      strcpy(last_input,input);
      }
  }

  printf("\t\t...exiting\n");
  }
  return 0;
}

/**
 * \file shellter.c
 * \brief Mini Shell.
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date January 17, 2018 
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<stdbool.h>
#include<getopt.h>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

#include"../include/shellter.h"

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096
#define RUN true


#define USAGE_SYNTAX "[options] [command_string | file]"
#define USAGE_PARAMS "OPTIONS:\n\
  -c, --command : commands are read from the first non-option argument command_string\n\
***\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"
/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * \see man 3 getopt_long or getopt
 * \see struct option definition
 */
static struct option binaryOpts[] = 
{
  { "help",    no_argument,       0, 'h' },
  { "verbose", no_argument,       0, 'v' },
  { "command", optional_argument, 0, 'c' },
  { 0,         0,                 0,  0  }
};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */ 
const char* binaryOptstr = "hvc:";

void printUsage(char* binName) {
  dprintf(1, "USAGE: %s %s\n\n%s\n", binName, USAGE_SYNTAX, USAGE_PARAMS);
}

void freeIfNeeded(void* toFree) {
  if (toFree != NULL) free(toFree);  
}

char* dupOptargStr() {
  char* str = NULL;

  if (optarg != NULL) {
    str = strndup(optarg, MAX_PATH_LENGTH);
    // Checking if ERRNO is set
    if (str == NULL) 
      perror(strerror(errno));
  }

  return str;
}

void executeBatch(char* commandParam) {
  
}

void executeShell() {
  while(RUN) {
    printPrompt();
  }

  exit(EXIT_SUCCESS);
}

void printPrompt() {
  char str[1024] = "";

  char hostname[1024];
  gethostname(hostname, 1024);
  
  printf("%s@%s:%s$ ",getUserName(), hostname, getWorkingDirectory());
  fgets(str, sizeof(str), stdin);
  clean(str, stdin);
  getWorkingDirectory();
}

// FREE MEMORY ??
const char *getUserName()
{
  uid_t uid = geteuid();
  struct passwd *pw = getpwuid(uid);
  if (pw)
  {
    return pw->pw_name;
  }

  return "guest";
}

// FREE MEMORY ??
char* getWorkingDirectory() {
  char *cwd = malloc(1024 * sizeof(char));
  getcwd(cwd, sizeof(char) * 1024);
  //printf("Current working dir: %s\n", cwd);
  return cwd;
}

void clean(const char *buffer, FILE *fp){
    
    char *p = strchr(buffer,'\n');

    if (p != NULL)
        *p = 0;
    else {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }

}

/**
 * Binary main loop
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv)
{
  /**
   * Binary variables
   * (could be defined in a structure)
   */
  short int isVerboseMode = 0;
  char* commandParam = NULL;

  // Parsing options
  int opt = -1;
  int optIdx = -1;

  bool shellterMode = true;

  while ((opt = getopt_long(argc, argv, binaryOptstr, binaryOpts, &optIdx)) != -1)
  {
    switch (opt)
    {
      case 'c':
        //command param
        if (optarg)
        {
          shellterMode = false; 
          commandParam = dupOptargStr();      
          executeBatch(commandParam);   
        } 
        break;
      case 'v':
        //verbose mode
        isVerboseMode = 1;
        // Printing params
        dprintf(1, "** PARAMS **\n%-8s: %s\n%-8s: %d\n", 
                "command", commandParam, 
                "verbose", isVerboseMode);
        break;
      case 'h':
        printUsage(argv[0]);

        freeIfNeeded(commandParam);
 
        exit(EXIT_SUCCESS);
      default :
        break;
    }
  } 

  if(shellterMode) {
    executeShell();
  }
  /**
   * Checking binary requirements
   * (could defined in a separate function)
   */
 /* if ()
  {
    dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

    freeIfNeeded(commandParam);

    // Exiting with a failure ERROR CODE (== 1)
    exit(EXIT_FAILURE);
  }*/

  // Freeing allocated data
  freeIfNeeded(commandParam);

  return EXIT_SUCCESS;
}
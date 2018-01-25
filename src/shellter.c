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
#include<unistd.h>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define BOLD  "\x1B[1m"

#include"../include/shellter.h"
#include"../include/manageEnvVar.h"
#include"../include/check.h"

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096

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

/**
 * If the shell need to run
 */ 
bool run = true;

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
  printWelcome();

  while(run) {
    printPrompt();
  }
  
  exit(EXIT_SUCCESS);
}

void printWelcome() {
  printf("=======================================\n");
  printf("=               WELCOME               =\n");
  printf("=               IN THE                =\n");
  printf("=              SHELLTER               =\n");
  printf("=======================================\n");
}

void changeDirectory(char* path) {
  if(path == NULL) {
    chdir(getEnvVar("HOME"));
  } else {
    CHECK(chdir(path));
  }
}

void echo(char* text) {
  printf("%s\n", text);
}

void exitShell() { 
  run = false;
}

void printPrompt() {
  char str [1024];

  printf("%s%s@%s%s:%s%s%s%s%s%s%s ",
    KCYN, getUserName(),
    getUserHostName(), KWHT,
    KYEL, BOLD, getWorkingDirectory(),
    KNRM, KWHT, getRootPermission(),
    KNRM);

  fgets(str, sizeof(str), stdin);
  clean(str, stdin);
}

// FREE MEMORY ??
char* getRootPermission() {
  uid_t uid=getuid(), euid=geteuid();
  if (uid<=0 || uid!=euid) {
      return "#";
  }

  return "$";
}

// FREE MEMORY ??
char* getUserHostName() {
  char* hostname = malloc(1024 * sizeof(char));
  gethostname(hostname, 1024 * sizeof(char)); // WHY ??
  return hostname;
}

// FREE MEMORY ??
char* getUserName() {
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
  getcwd(cwd, sizeof(char) * 1024); // WHY ?

  if(strstr(cwd, getEnvVar("HOME")) != NULL) {
    cwd = str_replace(cwd, getEnvVar("HOME"), "~");
  }
 
  return cwd;
}

// You must free the result if result is non-NULL. CHECK THIS
char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
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
        } else {
          freeIfNeeded(commandParam);
          exit(EXIT_FAILURE);
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
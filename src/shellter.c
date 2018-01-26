/**
 * \file shellter.c
 * \brief Mini Shell.
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date January 17, 2018 
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>

#include "../include/shellter.h"
#include "../include/manageEnvVar.h"
#include "../include/utils.h"
#include "../include/typedef.h"
#include "../include/check.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define BOLD  "\x1B[1m"

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

FILE *fpHistory = NULL;
int commandNumber = 0;

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
    CHECK(str == NULL); 
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
  printf("╔═════════════════════════════════════╗\n");
  printf("║               WELCOME               ║\n");
  printf("║               IN THE                ║\n");
  printf("║              SHELLTER               ║\n");
  printf("╚═════════════════════════════════════╝\n");
}

void parser(char* command) {
  
}

void changeDirectory(char* path) {
  if(path == NULL) {
    CHECK(chdir(getEnvVar("HOME")) != -1);
  } else {
    CHECK(chdir(path) != -1);
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
  writeToFile(str);
}

void writeToFile(char* command){ // TODO CHANGER QUAND ON POURRA EXIT et CTRL+C POUR CLOSE LE FICHIER
  if(fpHistory == NULL) {
    fpHistory = fopen("/tmp/shelterHistory", "a+");
  }

  if(fpHistory != NULL) {
    fprintf(fpHistory, "%d\t%s\n", commandNumber, command);
    commandNumber++;
    fclose(fpHistory);
    fpHistory = NULL;
  }
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

/**
 * Binary main loop
 *
 * \return 1 if it exit successfully 
 */
int main(int argc, char** argv)
{
  Parameters parameters;
  parameters.verbose = false;
  parameters.command = NULL;

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
          parameters.command = dupOptargStr();      
          executeBatch(parameters.command);   
        } else {
          freeIfNeeded(parameters.command);
          exit(EXIT_FAILURE);
        }
        break;
      case 'v':
        //verbose mode
        parameters.verbose = true;
        // Printing params
        dprintf(1, "** PARAMS **\n%-8s: %s\n%-8s: %d\n", 
                "command", parameters.command, 
                "verbose", parameters.verbose);
        break;
      case 'h':
        printUsage(argv[0]);
        freeIfNeeded(parameters.command);
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

  return EXIT_SUCCESS;
}
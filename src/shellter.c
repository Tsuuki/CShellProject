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
#include "../include/parser.h"
#include "../include/handler.h"
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
#define BUFFERSIZE 4096

#define USAGE_SYNTAX "[options] [command_string | file]"
#define USAGE_PARAMS "OPTIONS:\n\
  -c, --command : commands are read from the first non-option argument command_string\n\
***\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"

FILE *fpHistory = NULL;
int commandNumber = -1;

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

char* dupOptargStr() {
  char* str = NULL;

  if (optarg != NULL) {
    str = strndup(optarg, MAX_PATH_LENGTH);
  }

  return str;
}

void executeBatch(char* commandParam) {
  Node* rootNode = parse(commandParam);
  handle(rootNode);

  exit(EXIT_SUCCESS);
}

void executeShell() {
  char* line = malloc(BUFFERSIZE * sizeof(char));
  Node* rootNode;
  printWelcome();

  while(run) {
    prompt(line);
    rootNode = parse(line);
    handle(rootNode);
  }

  freeIfNeeded(line);
  
  exit(EXIT_SUCCESS);
}

void printWelcome() {
  printf("╔═════════════════════════════════════╗\n");
  printf("║               WELCOME               ║\n");
  printf("║               IN THE                ║\n");
  printf("║              SHELLTER               ║\n");
  printf("╚═════════════════════════════════════╝\n");
}

char* prompt(char* str) {
  char* cwd = getWorkingDirectory();

  if(strstr(cwd, getEnvVar("HOME")) != NULL) {
    cwd = str_replace(cwd, getEnvVar("HOME"), "~");
  }
 
  printf("%s%s@%s%s:%s%s%s%s%s%s%s ",
    KCYN, getUserName(),
    getUserHostName(), KWHT,
    KYEL, BOLD, cwd,
    KNRM, KWHT, getRootPermission(),
    KNRM);

  fgets(str, BUFFERSIZE * sizeof(char), stdin);
  clean(str, stdin);
  writeToFile(str);

  return str;
}

void writeToFile(char* command){ // TODO CHANGER QUAND ON POURRA EXIT et CTRL+C POUR CLOSE LE FICHIER

  if(commandNumber == -1) {
    CHECK((fpHistory = fopen("/tmp/shellterHistory", "a+")) != NULL);
    fseek(fpHistory, 0, SEEK_END);

    if(ftell(fpHistory) > 0)
        getCmdNum();
    else
      commandNumber = 1;

    fclose(fpHistory);
  }

  CHECK((fpHistory = fopen("/tmp/shellterHistory", "a")) != NULL);
  fprintf(fpHistory, "%d\t%s\n", commandNumber, command);
  commandNumber++;
  fclose(fpHistory);
}

void getCmdNum() {

  int c;
  int i = -2;
  fseek(fpHistory, i, SEEK_END);
  while((c = fgetc(fpHistory)) != '\n') {
    i--;
    fseek(fpHistory, i, SEEK_END);
  }

  char *nbStr = malloc(BUFFERSIZE * sizeof(char));
  memset(nbStr, 0, BUFFERSIZE * sizeof(char));
  int l = 0;
  while((c = fgetc(fpHistory)) <= '9' && c >= '0' && c != EOF) {
    i++;
    fseek(fpHistory, i, SEEK_SET);
    nbStr[l] = (char)c;
    //*nbStr++ = (char)c; //DOESN'T WORK
    l++;
  }

  nbStr[l] = '\0';
  //*(nbStr) = '\0'; DOESN'T WORK
  
  int number;
  if (sscanf(nbStr, "%d", &number) == 1) {
    commandNumber = number + 1;
    return;
  }

  commandNumber = 1;
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

  return EXIT_SUCCESS;
}
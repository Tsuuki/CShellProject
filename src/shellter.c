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
#include <sys/types.h>
#include <wait.h>
#include <signal.h>
#include <ctype.h>

#include "../include/typedef.h"
#include "../include/check.h"
#include "../include/utils.h"
#include "../include/manageEnvVar.h"
#include "../include/parser.h"
#include "../include/handler.h"
#include "../include/shellter.h"
#include "../include/manageAlias.h"

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
#define BUFFER_SIZE 4096
#define FORKMAP_SIZE 25

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

struct AliasArray *aliases;

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
  handle(parse(commandParam)->rootNode);

  exit(EXIT_SUCCESS);
}

ForkMap **forkMapValues;
void executeShell(bool verbose) {
  pid_t pid;
  char *line = malloc(BUFFER_SIZE * sizeof(char));
  ForkMap **forkMap = malloc(FORKMAP_SIZE * sizeof(ForkMap));
  forkMapValues = malloc(FORKMAP_SIZE * sizeof(ForkMap));
  LinkedList *linkedList;

  printWelcome();

  while(run) {
    prompt(line);
    if(verbose)
      printf("%s\n", line);

    ManageForkMap(forkMap, forkMapValues);
    linkedList = parse(line);
    if(linkedList->isBackgrounded) {
      if((pid = fork()) == 0) {
        setpgid(0, 0);
        handle(linkedList->rootNode);
        exit(EXIT_SUCCESS);
      } else if (pid == -1) {
        perror("Fork failed\n");
        exit(EXIT_FAILURE);
      } else {
        line = trimAmpersand(line);
        FillForkMapValues(forkMapValues, pid, line);
      }
    } else {
      pid = waitpid(-1, NULL, WNOHANG);
      handle(linkedList->rootNode);
    }
    printForkMap(forkMap);
  }

  freeIfNeeded(line);
  freeIfNeeded(linkedList);
  freeIfNeeded(forkMap);
  freeIfNeeded(forkMapValues);
  
  exit(EXIT_SUCCESS);
}

void FillForkMapValues(ForkMap **forkMapValues, int pid, char *value) {
  int i;
  
  for(i = 0; i < FORKMAP_SIZE; i++) {
    if(forkMapValues[i] == NULL) {
      printf("[%d]          %d : %s\n", i + 1, pid, value);
      forkMapValues[i] = malloc(sizeof(ForkMap));
      forkMapValues[i]->index = i + 1;
      forkMapValues[i]->pid = pid;
      forkMapValues[i]->value = malloc(sizeof(char*));
      strcpy(forkMapValues[i]->value, value);
      break;
    }
  }
}

void ManageForkMap(ForkMap **forkMap, ForkMap **forkMapValues) {
  pid_t pid;
  int i = 0;
  int j = 0;  
  
  while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
    while(forkMap[i] != NULL) 
      i++;
    for(j = 0; j < FORKMAP_SIZE; j++) {
      if(forkMapValues[j] != NULL && forkMapValues[j]->pid == pid) 
        break;
    }
    if(forkMapValues[j] != NULL) {
      forkMap[i] = malloc(sizeof(ForkMap));
      forkMap[i]->index = forkMapValues[j]->index;
      forkMap[i]->pid = forkMapValues[j]->pid;
      forkMap[i]->value = malloc(sizeof(char*));
      strcpy(forkMap[i]->value, forkMapValues[j]->value);
      freeForkMap(forkMapValues, j);
    }
  }
}

void printForkMap(ForkMap **forkMap) {
  int i;
  
  for(i = 0; i < FORKMAP_SIZE; i++) {
    if(forkMap[i] != NULL) {
      printf("[%d]+ Done    %d : %s\n", forkMap[i]->index, forkMap[i]->pid, forkMap[i]->value);
      freeForkMap(forkMap, i);
    }
  }
}

void makeForeground(int index) {
  if(forkMapValues[index] == NULL) 
    return;

  printf("%s\n", forkMapValues[index]->value);
  waitpid(forkMapValues[index]->pid, NULL, 0);
  freeForkMap(forkMapValues, index);
}

void freeForkMap(ForkMap **forkMap, int index) {
  freeIfNeeded(forkMap[index]->value);
  freeIfNeeded(forkMap[index]);
  forkMap[index] = NULL;
}

char *trimAmpersand(char *str) {
  char *end;

  end = str + strlen(str) - 1;
  while(end > str && (isspace((unsigned char)*end) || (unsigned char)*end == '&')) 
    end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
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

  fgets(str, BUFFER_SIZE * sizeof(char), stdin);
  clean(str, stdin);

  if(strlen(str) > 0)
    writeToFile(str);

  return str;
}

void writeToFile(char* command){

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

  char *nbStr = malloc(BUFFER_SIZE * sizeof(char));
  memset(nbStr, 0, BUFFER_SIZE * sizeof(char));
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

  initAliases();

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
    executeShell(parameters.verbose);
  }

  return EXIT_SUCCESS;
}
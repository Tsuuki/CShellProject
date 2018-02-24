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
#include <termios.h>


#include "../include/typedef.h"
#include "../include/check.h"
#include "../include/utils.h"
#include "../include/manageEnvVar.h"
#include "../include/parser.h"
#include "../include/handler.h"
#include "../include/shellter.h"
#include "../include/manageAlias.h"
#include "../include/history.h"

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
#define PID_ARRAY_SIZE 4096

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

int linux_getch(void) 
{
  struct termios oldstuff;
  struct termios newstuff;
  int    inch;
  
  tcgetattr(STDIN_FILENO, &oldstuff);
  newstuff = oldstuff;                  /* save old attributes               */
  newstuff.c_lflag &= ~(ICANON | ECHO); /* reset "canonical" and "echo" flags*/
  tcsetattr(STDIN_FILENO, TCSANOW, &newstuff); /* set new attributes         */
  inch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldstuff); /* restore old attributes     */

  return inch;
}

void executeShell(bool verbose) {
  pid_t pid;
  pid_t pidArray[PID_ARRAY_SIZE];
  char *line = malloc(BUFFER_SIZE * sizeof(char));
  int i = 0;
  int j = 0;
  ForkMap *forkMap[PID_ARRAY_SIZE];
  LinkedList *linkedList;

  printWelcome();

  while(run) {
    prompt(line);
    if(verbose)
      printf("%s\n", line);

    linkedList = parse(line);
    if(linkedList->isBackgrounded) {
      if((pid = fork()) == 0) {
        handle(linkedList->rootNode);
        exit(EXIT_SUCCESS);
      } else if (pid == -1) {
        perror("Fork failed\n");
        exit(EXIT_FAILURE);
      } else {
        line = trimAmpersand(line);
        printf("[%d]          %s\n", pid, line);
        while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
          printf("[%d]+ Done    \n", pidArray[j]);//, getForkMapValue(forkMap, pid));
        }
      }
    } else {
      while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        pidArray[i] = pid;
        i++;
      }
      pid = waitpid(-1, NULL, WNOHANG);
      handle(linkedList->rootNode);
      while (j != i) {
        printf("[%d]+ Done    \n", pidArray[j]);//, getForkMapValue(forkMap, pid));
        j++;
      }
      i = 0;
      j = 0;
    }
  }

  freeIfNeeded(line);
  freeIfNeeded(linkedList);
  freeIfNeeded(forkMap);
  
  exit(EXIT_SUCCESS);
}



char *getForkMapValue(ForkMap *forkMap[], int index) {
  char *value = "";
  int i = 0;

  while(forkMap[i] != NULL) {
    if(forkMap[i]->index == index) {
      value = forkMap[i]->value;
      break;
    }
  } 

  return value;
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

  resetHistoryCounter(commandNumber);
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

  int kb_char;
  int i = 0;
  while ((kb_char = linux_getch()) != 0x0A) {
    if (kb_char == 127 || kb_char == 8) {
      if(i != 0) {
        printf("\b \b"); //TODO SUPPRIMER LES CHAR DANS STR
        i--;
      }
    }
    else {
      
      if('\033' == kb_char) {
        kb_char = linux_getch();
        switch(linux_getch()) { // the real value
          case 'A': //ARROW UP
            readHistory(0, &str);
            break;
          case 'B': //ARROW DOWN
            readHistory(1, &str);
            break;
          case 'C':
            if(i < strlen(str)) {
              printf("\033[1C"); // Move right column;
              i++;
            }
            break;
          case 'D':
            if(i > 0) {
              printf("\033[1D"); // Move left column;
              i--;
            }
            break;
          default:
            break;
        }
      }
      else {
        printf("%c", kb_char);
        str[i] = kb_char;
        i++;
      }
    }
  }
  str[i] = '\0';
  printf("\n");

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
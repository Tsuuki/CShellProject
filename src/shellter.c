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
#include "../include/define.h"

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
  
  clearStr(str, BUFFER_SIZE);
  resetHistoryCounter(commandNumber - 1);
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
  int cursor_position = 0;
  while ((kb_char = linux_getch()) != 0x0A) {
    if (kb_char == 127 || kb_char == 8) {
      if(cursor_position > 0) {
        removeCharString(cursor_position - 1, &str);
        moveCursor(1, strlen(str) - cursor_position);
        clearPrompt(strlen(str) + 1);
        printf("%s", str);
        moveCursor(0, strlen(str) - cursor_position);
        cursor_position--;
      }
    }
    else {
      
      if('\033' == kb_char) {
        kb_char = linux_getch();
        switch(linux_getch()) { // the real value
          case 'A': //ARROW UP
            clearPrompt(strlen(str));
            clearStr(str, BUFFER_SIZE);
            readHistory(0, &str, commandNumber - 1);
            printf("%s", str);
            cursor_position = strlen(str);
            break;
          case 'B': //ARROW DOWN
            clearPrompt(strlen(str));
            clearStr(str, BUFFER_SIZE);
            readHistory(1, &str, commandNumber - 1);
            printf("%s", str);
            cursor_position = strlen(str);
            break;
          case 'C':
            if(cursor_position < strlen(str)) {
              printf("\033[1C"); // Move right column;
              cursor_position++;
            }
            break;
          case 'D':
            if(cursor_position > 0) {
              printf("\033[1D"); // Move left column;
              cursor_position--;
            }
            break;
          default:
            break;
        }
      }
      else {
        printf("%c", kb_char);
        str[cursor_position] = kb_char;
        cursor_position++;
      }
    }
  }
  printf("\n");

  if(strlen(str) > 0)
    writeToFile(str);

  return str;
}

void moveCursor(int way, int length) {
  char *move;
  if(way == 1)
    move = "\033[1C";
  else
    move = "\033[1D";

  for(int i = 0; i <= length; i++) {
    printf("%s", move);
  }
}

void removeCharString(int position, char **str) {
  while((*str)[position] != '\0'){
    (*str)[position] = (*str)[position + 1];
    position++;
  }
}

void clearPrompt(int nbCharToDelete) {
  while(nbCharToDelete-- > 0) {
    printf("\b \b");
  }  
}

void clearStr(char *str, int size) {
  memset(str, 0x00, size);
}

void writeToFile(char* command){
  CHECK((fpHistory = fopen("/tmp/shellterHistory", "a")) != NULL);
  fprintf(fpHistory, "%d\t%s\n", commandNumber, command);
  commandNumber++;
  fclose(fpHistory);
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
  commandNumber = retrieveCommandNumber();

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
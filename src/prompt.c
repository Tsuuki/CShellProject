/**
 * \file prompt.c
 * \brief Mini Shell.
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date February 25, 2018 
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "../include/define.h"
#include "../include/typedef.h"
#include "../include/utils.h"
#include "../include/history.h"
#include "../include/environment.h"
#include "../include/prompt.h"

extern int commandNumber;

void printWelcome() {
  printf("╔═════════════════════════════════════╗\n");
  printf("║               WELCOME               ║\n");
  printf("║               IN THE                ║\n");
  printf("║              SHELLTER               ║\n");
  printf("╚═════════════════════════════════════╝\n");
}

char* prompt(char* str) {
  
  // Erase all memory in the string
  clearStr(str, BUFFER_SIZE);

  // Put history at the end for arrow key 
  resetHistoryCounter(commandNumber - 1); 

  // Write in terminal the current user, system and path
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

  // While keyboard code is different to enter char
  while ((kb_char = linux_getch()) != 0x0A) {

    // If this is keyboard backspace code
    if (kb_char == 127 || kb_char == 8) {
      if(cursor_position > 0) {

        // Delete char from String
        removeCharString(cursor_position - 1, &str);

        // Move cursor to the end
        moveCursor(1, strlen(str) - cursor_position);

        // Clear all the prompt section
        clearPrompt(strlen(str) + 1);
        printf("%s", str);

        // Then put the cursor at the right place
        moveCursor(0, strlen(str) - cursor_position);
        cursor_position--;
      }
    }
    else {
      
      // If we catch arrow key
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
          case 'C': //ARROW RIGHT
            if(cursor_position < strlen(str)) {
              printf("\033[1C"); // Move right column;
              cursor_position++;
            }
            break;
          case 'D': //ARROW LEFT
            if(cursor_position > 0) {
              printf("\033[1D"); // Move left column;
              cursor_position--;
            }
            break;
          default:
            break;
        }
      }

      // Else this is a char to write
      else {
        if(cursor_position != strlen(str)) {
          writeToString(kb_char, cursor_position, &str);
          moveCursor(1, strlen(str) - cursor_position);
          clearPrompt(strlen(str) + 1);
          printf("%s", str);
          moveCursor(0, strlen(str) - cursor_position - 2);
        } else {
          str[cursor_position] = kb_char;
          printf("%c", kb_char);
        }
        cursor_position++;
      }
    }
  }
  printf("\n");

  if(strlen(str) > 0)
    writeToFile(str);
  return str;
}

int linux_getch() 
{
  struct termios old;
  struct termios new;
  int    inch;
  
  tcgetattr(STDIN_FILENO, &old);
  new = old;                    /* save old attributes               */
  new.c_lflag &= ~(ICANON | ECHO); /* reset "canonical" and "echo" flags*/
  tcsetattr(STDIN_FILENO, TCSANOW, &new); /* set new attributes         */
  inch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &old); /* restore old attributes     */

  return inch;
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

void clearPrompt(int nbCharToDelete) {
  while(nbCharToDelete-- > 0) {
    printf("\b \b");
  }  
}
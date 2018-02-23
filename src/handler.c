/**
 * \file handler.c
 * \brief handle the commands in the tree
 * \author Quentin D. Jordan H.
 * \version 1.0
 * \date February 6, 2018 
 * 
 * Execute all the command in the node tree 
 */

#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "../include/typedef.h"
#include "../include/check.h"
#include "../include/utils.h"
#include "../include/executer.h"
#include "../include/handler.h"

#define BUFFERSIZE 4096 //TODO A FACTO

void handle(Node *rootNode) {
  bool run = true;
  int number;
  Node *node = rootNode;

  // If command is empty
  if(rootNode == NULL || (memcmp(rootNode, node, sizeof(Node)) == 0 && strcmp("", node->action->command) == 0)){
    return;
  }

  if(node->next == NULL) {
      execute(node, true);
  } else {
    while(node != NULL && run) {
      if(strcmp("&&", node->operator) == 0) {
        run = execute(node, true);
      } else if (strcmp("||", node->operator) == 0) {
        run = !execute(node, true);
      } else if (strcmp("|", node->operator) == 0) {
        number = handlePipe(node);
        while(number > 1) {
          node = node->next;
          number--;
        }
      } else if (strcmp(">", node->operator) == 0) {
        handleRedirection(node, node->next->action->command, "w", 1);
      } else if (strcmp(">>", node->operator) == 0) {
        handleRedirection(node, node->next->action->command, "a+", 1);
      } else if (strcmp("<", node->operator) == 0) {
        handleRedirection(node, node->next->action->command, "r", 0);
        node = node->next;
      } else if (strcmp("<<", node->operator) == 0) {
        handleProgressivReading(node, node->next->action->command);
        node = node->next;
      } else {
        execute(node, true);
      }
      node = node->next;
    }
  }
  
  freeIfNeeded(rootNode);
}

void handleProgressivReading(Node *node, char *endWord) {
  if(strlen(endWord) > 0) {

    pid_t pidNode;
    int status = 0;
    FILE *fp;
    char *str = malloc(sizeof(char) * 4096);

    if((pidNode = fork()) == 0) {

      CHECK((fp = tmpfile()) != NULL);

      while(strcmp(str,endWord) != 0) {
        printf("> ");
        fgets(str, 4096 * sizeof(char), stdin);
        str[strlen(str)-1] = '\0';
        
        if(strcmp(str,endWord) != 0) {
          fprintf(fp, "%s\n", str);
        }
      }

      fseek(fp, 0, SEEK_SET);
      dup2(fileno(fp), 0);
      close(fileno(fp));
      execute(node, false);
      fclose(fp);
    } else if(pidNode == -1) {
      perror("Input fork failed\n");
      exit(EXIT_FAILURE);
    }

    waitpid(pidNode, &status, 0);
  }

  
}

void handleRedirection(Node *node, char *file, char *mode, int descripteur) {

  pid_t pidNode;
  int status = 0;
  FILE *fp = NULL;

  if(strlen(file) > 0) {

    if((pidNode = fork()) == 0) {
      CHECK((fp = fopen(file, mode)) != NULL);
      dup2(fileno(fp), descripteur);
      close(fileno(fp));
      execute(node, false);
      fclose(fp);
    } else if(pidNode == -1) {
      perror("Input fork failed\n");
      exit(EXIT_FAILURE);
    }

    waitpid(pidNode, &status, 0);
  }
}

int handlePipe(Node *node) {
  pid_t pid;
  int i;
  int number = getPipeNumber(node);
  int status = 0;
  Node *nodeArray[number+1];
  
  for(i = 0; i < number; i++) {
    nodeArray[i] = node;
    node = node->next;
  }
  nodeArray[number] = NULL;

  if((pid = fork()) == 0) {
    handlePipeArray(nodeArray, STDIN_FILENO, 0);
  } else if(pid == -1) {
    perror("Fork failed\n");
    exit(EXIT_FAILURE);
  } 
    
  waitpid(pid, &status, 0);

  return number;
}

void handlePipeArray(Node *nodeArray[], int inputFileDescriptor, int position) {
  pid_t pid;
  int fileDescriptor[2];

  if(nodeArray[position+1] == NULL) {
    dup2(inputFileDescriptor, STDIN_FILENO);
    execute(nodeArray[position], false);
  } else {
    CHECK(pipe(fileDescriptor) == 0);

    if((pid = fork()) == 0) {
      close(fileDescriptor[0]);
      dup2(inputFileDescriptor, STDIN_FILENO);
      dup2(fileDescriptor[1],STDOUT_FILENO);
      execute(nodeArray[position], false);
    } else if(pid == -1) {
      perror("Fork failed\n");
      exit(EXIT_FAILURE);
    } else { 
      close(fileDescriptor[1]);      
      close(inputFileDescriptor);
      handlePipeArray(nodeArray, fileDescriptor[0], position+1);
    }
  }
  return;
}

int getPipeNumber(Node *node) {
  int number = 1;

  while(node->next != NULL && strcmp("|", node->operator) == 0) {
    node = node->next;
    number++;
  }

  return number;
}
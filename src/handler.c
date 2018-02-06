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

#include "../include/typedef.h"
#include "../include/utils.h"
#include "../include/check.h"
#include "../include/executer.h"
#include "../include/handler.h"

void handle(struct Node* rootNode) {
  Node* node = rootNode;

  if(rootNode != NULL && !(memcmp(rootNode, node, sizeof(Node)) == 0 && strcmp(node->action->command, "") == 0)){
    while(node != NULL) {
      if(node->next == NULL) {
        execute(node);
      }
      
      node = node->next;
    }
  }
  
  freeIfNeeded(rootNode);
}
/*** Definition section ***/
%{
  #include <stdio.h>
  #include "../include/parser.tab.h"

  extern int yylex();

  void  yyerror (char *s){
    printf ("\nErreur : %s\n", s);
  }

  int yyparse();

%}

%union{
  char* string;
  int value;
}

/*** Token ***/
%token PIPE
%token GREAT
%token GREATGREAT
%token LESS
%token LESSLESS
%token AMPERSAND
%token AMPERSANDAMPERSAND
%token PIPEPIPE
%token <string> ARGUMENT
%token NEWLINE
%token <string> WORD

/*** Rules section ***/
%%

command_list:
  command_list command_line { printf("command_list 1\n");}
  | command_line            { printf("command_list 2\n");}
  ;

command_line:
  pipe_command io background NEWLINE          { printf("command_line 1\n"); }
  | pipe_command control background NEWLINE   { printf("command_line 2\n"); }
  | pipe_command background NEWLINE           { printf("command_line 3\n"); }
  | NEWLINE                                       { printf("command_line empty command\n");}
  | error NEWLINE {yyerrok;}
  ;

pipe_command:
  pipe_command PIPE command     { printf("pipe_command 1\n"); }
  | command                     { printf("pipe_command 2\n"); }
  ;

command:
  WORD               { printf("command 1 %s\n", $1); } 
  | WORD list_args    { printf("command 2 %s\n", $1); } 
  ;

list_args:
  list_args ARGUMENT     {printf("list_args 1 args %s\n", $2);}
  | ARGUMENT              {printf("list_args 2 args %s\n", $1);}
  ;

io:
  GREAT WORD          {printf("io great word %s\n", $2);}
  | GREATGREAT WORD   {printf("io greatgreat word %s\n", $2);}
  | LESS WORD         {printf("io less word %s\n", $2);}
  | LESSLESS WORD     {printf("io lessless word %s\n", $2);}
  ;

control:
  AMPERSANDAMPERSAND WORD   {printf("control && word %s\n", $2);}
  | PIPEPIPE WORD           {printf("control || word %s\n", $2);}
  ;

background:
  AMPERSAND                 {printf("background \n");}
  |                         {printf("background empty \n");}
  ;

%%
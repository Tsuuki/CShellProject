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
%token MINUS
%token NEWLINE
%token WORD

%left MINUS
/*** Rules section ***/
%%

command_list:
  command_list command_line { printf("command_list 1\n");}
  | command_line            { printf("command_list 2\n");}
  ;

command_line:
  pipe_command io_list background NEWLINE         { printf("command_line 1\n"); }
  | pipe_command control_list background NEWLINE  { printf("command_line 2\n"); }
  | NEWLINE                                       { printf("command_line empty command\n");}
  | error NEWLINE {yyerrok;}
  ;

pipe_command:
  pipe_command PIPE command     { printf("pipe_command 1\n"); }
  | command                     { printf("pipe_command 2 1\n"); }
  ;

command:
  WORD list_args    { printf("command 1\n"); } 
  ;

list_args:
  list_args arg     {printf("list_args 1\n");}
  | arg             {printf("list_args 2\n");}
  ;

arg:
  MINUS WORD          {printf("arg 1\n");}
  | MINUS MINUS WORD  {printf("arg 2\n");}
  | WORD              {printf("arg 3\n");}
  |                   {printf("arg nothing\n");}
  ;

io_list:
  io_list io          {printf("io_list 1\n");}
  |                   {printf("io_list empty\n");}
  ;

io:
  GREAT WORD          {printf("io great word\n");}
  | GREATGREAT WORD   {printf("io greatgreat word\n");}
  | LESS WORD         {printf("io less word\n");}
  | LESSLESS WORD     {printf("io lessless word\n");}
  ;

control_list:
  control_list control    {printf("control_list 1\n");}
  |                       {printf("control_list empty\n");}
  ;

control:
  AMPERSANDAMPERSAND WORD   {printf("control && word\n");}
  | PIPEPIPE WORD           {printf("control || word\n");}
  ;

background:
  AMPERSAND
  |
  ;

%%
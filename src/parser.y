/*** Definition section ***/
%{
  #include <stdio.h>
%}

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

/*** Rules section ***/
argument:
  argument WORD
  |
  ;

arguments:
  MINUS argument
  | arguments argument
  ;

command_args:
  WORD arguments
  ;

pipe_command:
  pipe_command PIPE command_args
  | command_args
  ;

io:
  GREAT WORD
  | GREATGREAT WORD
  | LESS WORD
  | LESSLESS WORD
  ;

io_list:
  io_list io
  |
  ;

background:
  AMPERSAND
  |
  ;

control:
  AMPERSANDAMPERSAND WORD
  | PIPEPIPE WORD
  ;

control_list:
  control_list control
  |
  ;

command:
  pipe_command io_list background NEWLINE
  | pipe_command control_list background NEWLINE
  | NEWLINE
  | error NEWLINE {yyerrok;}

command_list:
  command_list command
  ;
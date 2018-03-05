/*** Definition section ***/
%{
  #include <stdio.h>

  void yyerror(char *s) {
      fprintf(stderr, "ERROR : Line %d --> %s\n", yylineno, s);
      exit(SYNTAX_ERROR);
  }

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
%%

command_list:
  command_list command { printf("Get a command");}
  | command
  ;

command:
  pipe_command io_list background NEWLINE
  | pipe_command control_list background NEWLINE
  | NEWLINE
  | error NEWLINE {yyerrok;}
  ;

pipe_command:
  pipe_command PIPE command_args
  | command_args
  ;

command_args:
  WORD arguments
  | WORD
  ;

arguments:
  | MINUS argument
  ;


argument:
  argument WORD
  | WORD
  ;

io_list:
  io_list io
  |
  ;

// io:
//   GREAT WORD
//   | GREATGREAT WORD
//   | LESS WORD
//   | LESSLESS WORD
//   ;

// background:
//   AMPERSAND
//   |
//   ;

// control:
//   AMPERSANDAMPERSAND WORD
//   | PIPEPIPE WORD
//   ;

// control_list:
//   control_list control
//   |
//   ;

%%
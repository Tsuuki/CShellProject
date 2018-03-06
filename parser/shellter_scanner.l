%option nounput
%option noinput
%option noyywrap

/*** Definition section ***/
%{

  #include <string.h>
  #include "../include/parser.tab.h"
%}

%%

"|"       { printf("|\n"); return PIPE; }
">"       { printf(">\n"); return GREAT; }
">>"      { printf(">>\n"); return GREATGREAT; }
"<"       { printf("<\n"); return LESS; }
"<<"      { printf("<<\n"); return LESSLESS; }
"&"       { printf("&\n"); return AMPERSAND; }
"&&"      { printf("&&\n"); return AMPERSANDAMPERSAND; }
"||"      { printf("||\n"); return PIPEPIPE; }
"-"       { printf("-\n"); return MINUS; }
"\n"      { printf("NEWLINE\n"); return NEWLINE; }
[ \t]     { printf("TAB\n"); /* Remove space and tabs */ }
[[:alnum:]]* {
  /** Words contains only alpha chars */
  yylval.string=strdup(yytext);
  printf("word : %s\n", yytext); 
  return WORD;
}
[.]*         { printf("Unknown token"); }

%%
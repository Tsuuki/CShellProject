/*** Definition section ***/
%{

  #include <string.h>
  #define SAVE_TOKEN yylval.string_val = strdup(yytext)
%}

%%

"|"       { return PIPE; }
">"       { return GREAT; }
">>"      { return GREATGREAT; }
"<"       { return LESS; }
"<<"      { return LESSLESS; }
"&"       { return AMPERSAND; }
"&&"      { return AMPERSANDAMPERSAND; }
"||"      { return PIPEPIPE; }
"-"       { return MINUS; }
"\n"      { return NEWLINE; }
[ \t]     { /* Remove space and tabs */ }
[^ \t\n][^ \t\n]* {
  /** Words contains only alpha chars */
  SAVE_TOKEN;
  return WORD;
}
[.]*         { printf("Unknown token"); }

%%
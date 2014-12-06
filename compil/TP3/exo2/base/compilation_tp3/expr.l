%{
  #include <stdlib.h>
  #include <string.h>
  #include "y.tab.h"
  #include "symbol.h" 
  
  extern symbol_t* symbol_table;
%}

identifier [a-zA-Z_]+[0-9a-zA-Z_]*
number     [1-9][0-9]*

%%

[=]          { return ASSIGN; }
"if"         { return IF; }
"while"      { return WHILE; }
"done"       { return DONE; }
"do"         { return DO; }
"then"       { return THEN; }
"else"       { return ELSE; }
"endif"      { return ENDIF; }
"true"       { return TRUE; }
"false"      { return FALSE; }
{identifier} { yylval.string = strdup(yytext);
               symbol_t* sym=symbol_alloc();
               symbol_set(sym,yylval.string,0,0,NULL);
               symbol_add(&symbol_table,sym);
               return ID; }
{number}     { yylval.value = atoi(yytext); 
               symbol_t* sym=symbol_alloc();
               symbol_set(sym,strdup(yytext),1,atoi(yytext),NULL);
               symbol_add(&symbol_table,sym);
              return NUM; }
"=="         { return EQUAL; }
"&&"         { return AND; }
"||"         { return OR; }
"!"          { return NOT; }
[()]         { return yytext[0]; }
\n           { return yytext[0]; }
[\t ]        { }
.            { printf("[Lex] Unknown character: %s\n", yytext); }

%%

// Free the memory allocated for Lex when we are done.
void lex_free() {
  yy_delete_buffer(YY_CURRENT_BUFFER);
  free(yy_buffer_stack);
}

/* University of Strasbourg - Master ILC-ISI-RISE
 * Compilation Lab - Intermediate Code Generation
 * Generation of quad code for control expressions
 */
%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "symbol.h" 
  #include "quad.h" 

  void yyerror(char*);
  int yylex();
  void lex_free();

  symbol_t* symbol_table = NULL;
  quad_t2* quad_list = NULL;
  int next_quad = 0;
%}

%union {
  char* string;
  int value;
}

%token ASSIGN WHILE DO DONE IF THEN ELSE ENDIF
%token EQUAL TRUE FALSE OR AND NOT
%token <string> ID
%token <value> NUM

%left EQUAL
%left OR AND
%left NOT
%%

axiom:
  statement_list '\n'
    {
      printf("Match !!!\n");
      return 0;
    }

statement_list:
    statement_list statement
  | statement
  ;

statement:
    ID ASSIGN expression
    {
      quad_t2* quad=quad_gen(&next_quad,'=',symbol_lookup(symbol_table,"toto"),symbol_lookup(symbol_table,"30"),NULL);
      quad_add(&quad_list,quad);
    }
  | WHILE condition DO statement_list DONE
  | IF condition THEN statement_list ENDIF
  | IF condition THEN statement_list ELSE statement_list ENDIF
  ;

condition:
    ID EQUAL NUM
  | TRUE
  | FALSE
  | condition OR condition
  | condition AND condition
  | NOT condition
  | '(' condition ')'
  ;

expression:
    ID
  | NUM
  ;

%%

void yyerror (char *s) {
    fprintf(stderr, "[Yacc] error: %s\n", s);
}

int main() {
  printf("Enter your code:\n");
  yyparse();
  printf("-----------------\nSymbol table:\n");
  symbol_printf(symbol_table);
  printf("-----------------\nQuad list:\n");
  quad_print(quad_list);

  // Be clean.
  lex_free();
  //quad_free(quad_list);
  //symbol_free(symbol_table);
  return 0;
}

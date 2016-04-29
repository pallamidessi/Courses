%{
  #include <stdio.h>
  #include "symbol.h" 
  #include "quad.h" 
  int yyerror();
  
  struct symbol* symbol_table = NULL;
  struct quad* quad_list = NULL;
  int next_quad = 0;
%}

%union{
  char* string; 
  int value;
}

%token <string> IDENTIFIER
%token <value> INTEGER
%token NOT EQUAL OR AND AFFEC
%token IF THEN ELSE ENDIF WHILE DO DONE
%token TRUE FALSE

%left EQUAL
%left OR AND
%left NOT
%%


stmt:           IDENTIFIER AFFEC expr'\n'                           {}
                | WHILE condition DO stmtlist DONE                  {}
                | IF condition THEN stmtlist ENDIF                  {}
                | IF condition THEN stmtlist ELSE stmtlist ENDIF    {};

stmtlist:       stmtlist stmt             {}
                | stmt                    {};

expr:           IDENTIFIER                {}
                | INTEGER                 {};

condition:      IDENTIFIER EQUAL INTEGER  {}
                | TRUE                    {}
                | FALSE                   {}
                | condition OR condition  {}
                | condition AND condition {}
                | NOT condition           {}
                | '('condition')'         {};
%%


int main(int argc, char** argv){
  printf("Coucou\n");
  //yylex();
  
  while(1){
    yyparse();
  }

  return 0;
}

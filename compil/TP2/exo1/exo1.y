%{
  #include <stdio.h>
  int yyerror();
%}


%token entier
%left '+' '-'
%left '*' '/' 
/*
%type integer 

%union{
  entier  
}
*/

%%


Axiom:E '\n'{ printf("Result:%d\n",$1); };
E: E'+'E      { $$=$1+$3; };
E: E'*'E      { $$=$1*$3; };
E: E'/'E      { $$=$1/$3; };
E: '(' E ')'  { $$=$2; };
E: E'-'E      { $$=$1-$3; };
E: '-'E       { $$=-$2; };
E: entier     { $$=$1; }; 

%%


int main(int argc, char** argv){
  printf("Coucou\n");
  //yylex();
  return yyparse();
}

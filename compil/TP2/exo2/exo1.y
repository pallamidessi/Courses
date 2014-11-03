%{
  #include <stdio.h>
  int yyerror();
%}


%token entier
%token identifier
%token operator
%token UNION
%token INTER
%token COMP
%token DIFF
%token AFFEC 
%left '+' '-'
%left '*' '/' 
/*
%type integer 

%union{
  entier  
}
*/

%%

liste: liste instruction \n ;
instruction: identifier AFFEC expression ;
expression:   operande 
            | operande UNION operande
            | operande INTER operande
            | operande DIFF operande 
            | COMP operande ;
operande: identifier | ensemble ;
ensemble: '{' '}'| liste-elements ;
liste-elements: entier ',' liste-elements;

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

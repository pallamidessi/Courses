/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program.  If not, see
<http://www.gnu.org/licenses/>.
*/

%{
  #include <stdio.h>
  int yyerror();
  
  //Static initialization to zero by the compiler 
  unsigned int set[26];
  

  //Intersection operation
  unsigned int inter_set(unsigned int set1, unsigned int set2){
    unsigned int result=0;
    return result= set1 & set2;
  }

  //Union operation
  unsigned int union_set(unsigned int set1, unsigned int set2){
    unsigned int result=0;
    return result= set1 | set2;
  }

  //Difference operation
  unsigned int diff_set(unsigned set1,unsigned int set2 ){
    unsigned int result=0;
    return result= set1 ^ set2;
  }

  //Complementary operation
  unsigned int comp_set(unsigned set){
    unsigned int result=0;
    return result= ~set;
  }
  
  //Print a set to stdout 
  void print_set(unsigned int set){
    unsigned int i;

    printf("\n");
    printf("{ ");
    
    for(i=0;i<31;i++){
      if(set & 1 << i){
        printf("%d, ",i+1);
      }
    }

    printf("}\n");
  }
  
  //Set a bit in an unsigned int 
  unsigned int set_element(unsigned int set, unsigned int element){
    return set | (1 << (element-1)); 
  }

%}


%token INTEGER
%token IDENTIFIER
%token UNION INTER COMP DIFF
%token AFFEC 
%token EMPTY_SET 

%%
Axiome:         liste'\n'                       {};
liste:          liste instruction'\n'           {}
                | instruction'\n'               {};

instruction:    IDENTIFIER AFFEC expression     {set[$1]=$3;}
                | IDENTIFIER                    {print_set(set[$1]);};

expression:     operande                        {$$=$1;}
                | operande UNION operande       {$$=union_set($1,$3);}
                | operande INTER operande       {$$=inter_set($1,$3);}
                | operande DIFF operande        {$$=diff_set($1,$3);} 
                | COMP operande                 {$$=comp_set($2);};

operande:       IDENTIFIER                      {$$=set[$1];}
                | ensemble                      {$$=$1;};

ensemble:       EMPTY_SET                       {$$=0;}
                | '{'liste_elements'}'          {$$=$2;};

liste_elements: INTEGER                         {$$= set_element(0,$1);}
                | INTEGER','liste_elements      {$$= set_element($3,$1);};

%%


int main(int argc, char** argv){
  printf("Opération ensembliste:\n");
  
  yyparse();
  return 0;
}

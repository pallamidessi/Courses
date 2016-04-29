
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
  #include<map>
  #include<cstring>
  #include<cstdio>
  #include<cstdlib>
  
  int current_index=0;
 
  /* Comparaison functor for the map */
  struct cmp_str{
     bool operator()(char const *a, char const *b)const{
      return std::strcmp(a, b) < 0;
     }
  };
  
  /* The symbole table as an hash map using the symbole as key*/
  std::map<const char*, int, cmp_str> symbole_table;

  void action(char* yytext, char* type){
    /*New symbole*/
    if(!symbole_table.count((const char*)(yytext))){
      char* token=new char[std::strlen(yytext)+1]; 
      std::strcpy(token,yytext);
      symbole_table[token]=current_index;
      printf("%s %s avec l'index: %d\n",yytext,type,symbole_table[yytext]);
      current_index++;
    }
    /* Existing symbole*/
    else{
      printf("%s %s déjà dans la table avec l'index: %d\n",yytext,type,symbole_table[yytext]);
    }
  }

%}

digit         [0-9]
letter        [a-zA-Z]
identifier    {letter}({letter}|{digit})* 
symbole       "+"|"-"|"*"|"/"|"%"|"!"|"||"|"&&"|"<"|"<="|">"|">="|"=="|"!="|"<<"|">>"|"&"|"="|"("|")"|"["|"]"|"{"|"}"|","|"'"|";" 
integer       {digit}+ 
float         {digit}+"."{digit}* 
char          "'"."'"
type          "char"|"int"|"float"|"void"|"main"
keyword       "if"|"else"|"for"|"while"|"cin"|"cout"|"typedef"|"const" 

%%

{keyword}    {action(yytext,"keyword");}
{type}       {action(yytext,"type");}
{symbole}    {action(yytext,"symbole");}
{integer}    {action(yytext,"integer litteral");}
{float}      {action(yytext,"float litteral");}
{char}       {action(yytext,"char litteral");}
{identifier} {action(yytext,"identifier");}


"/*".*"*/"   {}
[ \t\n\r]+   {}
.            {printf("Parsing error!");exit(1);}

%%


int main( int argc, char **argv )
{
  yyin = fopen( argv[1] , "r" ); 
  yylex();
  fclose( yyin );
  /* Must destroy the map and free all its keys */
  return 0;
}

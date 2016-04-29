%{
  #include<stdio.h>
  #include<stdlib.h>
  #include"y.tab.h"
  #include <string.h>
%}

digit         [0-9]
integer       [1-9]+{digit}*
boolean       "TRUE"|"FALSE"
id            [a-zA-Z]+[0-9a-zA-Z]*
affectation   "ASSIGN"
bracket       "("|")"


%%
{id}            {
                  yylval.string=strdup(yytext);
                  return IDENTIFIER;
                }
{integer}       {
                  yylval.value=atoi(yytext);
                  return INTEGER;
                }


{bracket}       {return yytext[0];}

"TRUE"|"true"   {return TRUE;}
"FALSE"|"false" {return FALSE;}
"not"|"NOT"     {return NOT;}
"EQUAL"|"equal" {return EQUAL;}
"OR"|"or"       {return OR;}
"AND"|"and"     {return AND;} 
"IF"|"if"       {return IF;} 
"THEN"|"then"   {return THEN;} 
"ELSE"|"else"   {return ELSE;} 
"ENDIF"|"endif" {return ENDIF;} 
"WHILE"|"while" {return WHILE;} 
"DO"|"do"       {return DO;} 
"DONE"|"done"   {return DONE;} 
{affectation}   {return AFFEC;}

[\n]            {return yytext[0];}
[ \t]           {}
.               {printf("Parsing error!");exit(1);}

%%

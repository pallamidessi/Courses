%{
  #include<stdio.h>
  #include<stdlib.h>
  #include"y.tab.h"
%}

digit         [0-9]
integer       {digit}+ 
special       [()+*-/]

%%
{integer}    {  yylval=atoi(yytext);
                return entier;
             }
{special}    {  return yytext[0]; }
[\n]         {  return yytext[0]; }
.            {  printf("Parsing error!");exit(1);}

%%

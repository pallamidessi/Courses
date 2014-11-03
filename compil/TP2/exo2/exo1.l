%{
  #include<stdio.h>
  #include<stdlib.h>
  #include"y.tab.h"
%}

digit         [0-9]
integer       {digit}|1{digit}|2{digit}|3(0|1|2) 
id            [a-zA-Z]
operator      "union"|"UNION"|"inter"|"INTER"|"comp"|"COMP"|"diff"|"DIFF" 
affectation   ":="
bracket       [{}]
separator     ','

%%
{integer}    {  yylval=atoi(yytext);
                return entier;
             }
{id}         {  yylval=yytext[0];
                return identifier;
             }
{bracket}    {  return yytext[0];}
{separator}  {  return yytext[0];}

"union"|"UNION" {return UNION}
"inter"|"INTER" {return INTER}
"comp"|"COMP"   {return COMP}
"diff"|"DIFF" {return DIFF} 
{affectation} {
                return AFFEC;
              }
[\n]         {  return yytext[0]; }
.            {  printf("Parsing error!");exit(1);}

%%

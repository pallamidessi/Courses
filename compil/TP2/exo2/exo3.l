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
separator     ","
empty         "{}"|"{ }"

%%
{integer}       {
                  yylval=atoi(yytext);
                  return INTEGER;
                }

{id}            {
                  if(yytext[0]<'a')
                    yylval=yytext[0]-'A';
                  else
                    yylval=yytext[0]-'a';
                  
                  return IDENTIFIER;
                }

{bracket}       {return yytext[0];}
{separator}     {return yytext[0];}

"union"|"UNION" {return UNION;}
"inter"|"INTER" {return INTER;}
"comp"|"COMP"   {return COMP;}
"diff"|"DIFF"   {return DIFF;} 
{affectation}   {return AFFEC;}
{empty}         {return EMPTY_SET;}
[\n]            {return yytext[0];}
[ \t]           {}
.               {printf("Parsing error!");exit(1);}

%%

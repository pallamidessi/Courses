ALL:analyzer

analyzer: y.tab.c lex.yy.c
	gcc y.tab.c lex.yy.c -o exo3 -ly -lfl

lex.yy.c: exo3.l 
	lex exo3.l

y.tab.c: exo3.y
	yacc -v -d exo3.y

clean:
	rm exo3 y.output lex.yy.c y.tab.* 

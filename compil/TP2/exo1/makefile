ALL:analyzer

analyzer: y.tab.c lex.yy.c
	gcc y.tab.c lex.yy.c -o exo1 -ly -lfl

lex.yy.c: exo1.l 
	lex exo1.l

y.tab.c: exo1.y
	yacc -v -d exo1.y

clean:
	rm exo1 y.tab.c

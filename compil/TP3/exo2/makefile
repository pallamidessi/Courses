ALL:analyzer

analyzer: y.tab.c lex.yy.c
	gcc y.tab.c lex.yy.c -o exo1 -ly -lfl

lex.yy.c: exo1.l 
	lex exo1.l

y.tab.c: exo1.y
	yacc -v -Wconflicts-sr -d exo1.y

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	/bin/rm $(EXEC) *.o y.tab.c y.tab.h lex.yy.c

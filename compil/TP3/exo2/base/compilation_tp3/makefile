CC = gcc
LEX = lex
YACC = yacc -d
CFLAGS = -O2 -Wall
LDFLAGS = -ly -ll # Linux: -lfl / OSX: -ll
EXEC = expr
SRC = #symbol.c quad.c
OBJ = $(SRC:.c=.o)

all: $(OBJ) y.tab.c lex.yy.c quad.c symbol.c
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

y.tab.c: $(EXEC).y
	$(YACC) $(EXEC).y

lex.yy.c: $(EXEC).l
	$(LEX) $(EXEC).l

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	/bin/rm $(EXEC) *.o y.tab.c y.tab.h lex.yy.c

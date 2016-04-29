

SRCS = $(wildcard Question*.c)
EXEC = $(SRCS:.c=)

CC = gcc

CFLAGS = -g -O0 -Wall
LDFLAGS = -lglut -lGL -lGLU -lm

all : $(EXEC)

%: %.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(EXEC) *~ *.o

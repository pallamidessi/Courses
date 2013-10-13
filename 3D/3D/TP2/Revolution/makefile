

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXEC = Revolution

CC = gcc

CFLAGS = -I/usr/X11R6/include -IGL -Wall
LDFLAGS = -L/usr/X11R6/lib -lglut -lGL -lGLU -lm

all : $(EXEC)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC) : $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXEC) *~ *.o

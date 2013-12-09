
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXEC = Extrusion

CC = gcc

CFLAGS = -O0 -g -I/usr/X11R6/include -IGL -Wall -fopenmp
LDFLAGS = -L/usr/X11R6/lib -lglut -lGL -lGLU -lm -fopenmp

all : $(EXEC)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC) : $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(EXEC) *~ *.o

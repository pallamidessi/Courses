CC=gcc
CFLAGS=-Wall -g -O0
TARGET=browser

ALL:$(TARGET) 

$(TARGET):exo3.o
	$(CC) -o $@ $< $(CFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm *.o $(TARGET)

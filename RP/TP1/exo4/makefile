CC=gcc
CFLAGS=-Wall -g -O0
TARGET=MT_server
TARGET2=MT_client

ALL:$(TARGET) $(TARGET2)

$(TARGET):server-tcp.o
	$(CC) -o $@ $< $(CFLAGS)

$(TARGET2):client-tcp.o
	$(CC) -o $@ $< $(CFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm *.o $(TARGET) $(TARGET2)

CFLAGS=-g -Wall -std=c99 -pedantic
EXEC=TP2
CC=gcc
SRC=$(wildcard *.c)
OBJS=$(SRC:.c=.o)

all: clean $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

clean:
	rm -f *.o $(EXEC)

OBJS = lex.o term.o parse.tab.o constants.o arena.o
CC = gcc
CFLAGS = -Wall

lc: $(OBJS) main.c
	$(CC) $(CFLAGS) main.c $(OBJS) -lm -o lc

lex.o: parse.tab.h

term.o: term.c term.h
	$(CC) $(CFLAGS) -c term.c

parse.tab.o: parse.tab.c
	$(CC) $(CFLAGS) -c parse.tab.c 

constants.o: constants.c constants.h
	$(CC) $(CFLAGS) -c constants.c

arena.o: arena.c arena.h
	$(CC) $(CFLAGS) -c arena.c

parse.tab.h: parse.y
	bison -d parse.y

clean:
	rm -f $(OBJS) parse.tab.* lc

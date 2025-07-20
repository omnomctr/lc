OBJS = arena.o lex.o term.o parse.tab.o
CC = gcc

lc: $(OBJS) main.c
	cc main.c $(OBJS) -lm -o lc

lex.o: parse.tab.h

term.o: term.c term.h
	$(CC) -c term.c

arena.o: arena.c arena.h
	$(CC) -c arena.c

parse.tab.o: parse.tab.c
	$(CC) -c parse.tab.c 

parse.tab.h: parse.y
	bison -d parse.y

clean:
	rm -f $(OBJS) parse.tab.* lc

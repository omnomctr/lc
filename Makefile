OBJS = lex.o term.o parse.tab.o constants.o
CC = gcc

lc: $(OBJS) main.c
	cc main.c $(OBJS) -lm -o lc

lex.o: parse.tab.h

term.o: term.c term.h
	$(CC) -c term.c

parse.tab.o: parse.tab.c
	$(CC) -c parse.tab.c 

constants.o: constants.c constants.h vendored/stb_ds.h
	$(CC) -c constants.c


parse.tab.h: parse.y
	bison -d parse.y

clean:
	rm -f -r $(OBJS) parse.tab.* lc vendored

vendored/stb_ds.h:
	@mkdir -p vendored
	wget https://raw.githubusercontent.com/nothings/stb/40adb995abeea13612ad73bda031c90e3c0cf821/stb_ds.h --directory-prefix=vendored/

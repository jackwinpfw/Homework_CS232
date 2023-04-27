CC = gcc
CFLAGS = -g -Wall
OBJS = crawler.o indexPage.o

all: webSearch
webSearch: webSearch.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm *.o -f webSearch
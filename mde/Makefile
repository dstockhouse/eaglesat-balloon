CC=gcc
CFLAGS=-g -Wall -lwiringPi
LIBS=
DEPS=mde.h es_control.h
SRCS=mde.c main.c ../es_control.c
OBJS=$(SRCS:.c=.o)
MAIN=mde_test

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(MAIN) $(OBJS)

.c.o: $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) *.o $(MAIN)

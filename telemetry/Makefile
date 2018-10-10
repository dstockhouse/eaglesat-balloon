CC=gcc
CFLAGS=-g -Wall -lwiringPi -lm
LIBS=
DEPS=telemetry.h
SRCS=telemetry.c main.c
OBJS=$(SRCS:.c=.o)
MAIN=telemetry_test

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(MAIN) $(OBJS)

.c.o: $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) *.o $(MAIN)

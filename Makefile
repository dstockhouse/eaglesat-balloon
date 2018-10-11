CC=gcc
CFLAGS=-g -Wall -lwiringPi -lm -lpthread
LIBS=
#DEPS=crp/crp.h crp/crp_generateFilename.h comms/comms.h mde/mde.h telemetry/telemetry.h es_control.h
#SRCS=crp/crp.c crp/crp_generateFilename.c comms/comms.c mde/mde.c telemetry/telemetry.c es_control.c main.c 
DEPS=crp/crp_generateFilename.h comms/comms.h mde/mde.h telemetry/telemetry.h es_control.h
SRCS=crp/crp_generateFilename.c comms/comms.c mde/mde.c telemetry/telemetry.c es_control.c main.c 
OBJS=$(SRCS:.c=.o)
MAIN=balloon_test

$(MAIN): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) $(LIBS) -o $(MAIN) $(OBJS)

.c.o: $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(MAIN)

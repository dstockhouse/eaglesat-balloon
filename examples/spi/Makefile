CC=gcc
CFLAGS=-g -Wall -lwiringPi

ex_telemetry_test: wiringPi_telemetry_test.c
	$(CC) $(CFLAGS) -o ex_telemetry_test wiringPi_telemetry_test.c

ex_adc: adc.c
	$(CC) $(CFLAGS) -o ex_adc adc.c

clean:
	rm ex_telemetry_test ex_adc

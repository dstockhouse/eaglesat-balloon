/*****************************************************************************
 *
 * File:
 * 	telemetry_test.c
 *
 * Description:
 * 	Demonstration of reading temperature and pressure sensors connected to
 * 	the MCP3008 ADC using SPI and wiringPi
 *
 * Revision 1.0
 * 	Date: 10/6/2018
 *	Author: David Stockhouse
 *	Changes: Created file from adc.c
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define ES_DEBUG_MODE

#define SPI_DEVICE	"/dev/spidev0.0"
#define ADC_CHAN 100
#define SPI_CHAN 0
#define NUM_TEMP_SENSORS	7

// Maximum range of the pressure sensor, in PSI
#define MAX_PRESSURE_RANGE_PSI	15

int spiInit(char *deviceName) {

	int fd, rc, mode = 3;

	if(deviceName == NULL) {
#ifdef	ES_DEBUG_MODE
		printf("NULL SPI device\n");
		return -1;
#endif
	}

	fd = open(deviceName, O_RDWR);
	if(fd < 0) {
#ifdef	ES_DEBUG_MODE
		perror("Failed to open SPI device");
		return fd;
#endif
	}

	rc = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if(rc < 0) {
#ifdef	ES_DEBUG_MODE
		perror("Failed to set SPI mode");
		return rc;
#endif
	}

	rc = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if(rc < 0) {
#ifdef	ES_DEBUG_MODE
		perror("Failed to get SPI mode");
		return rc;
#endif
	}

	return fd;
}

int spiClose(int fd) {

	int rc;

	rc = close(fd);
	if(rc < 0) {
#ifdef	ES_DEBUG_MODE
		printf("Failed to close SPI device %d", fd);
		return rc;
#endif
	}

	return 0;
}

int analogRead(int fd, int chan) {

	int rc;
	int analogVal; // Output

	char rxBuf[8] = {0}, txBuf[8] = {0};
	struct spi_ioc_transfer transfer;

	txBuf[0] = 0x01;
	txBuf[1] = 0x80 | ((chan & 0x7) << 4);
	txBuf[2] = 0x00;

	transfer.tx_buf = (unsigned long) txBuf;
	transfer.rx_buf = (unsigned long) rxBuf;
	transfer.len = 3;

	// Speed to 10 MHZ
	transfer.speed_hz = 10000000;

	transfer.bits_per_word = 8;
	transfer.delay_usecs = 0;

	rc = ioctl(fd, SPI_IOC_MESSAGE(4), &transfer);
	printf("%d is rc from ioctl\n", rc);
	if(rc < 0) {
#ifdef	ES_DEBUG_MODE
		perror("SPI transfer failed");
		return rc;
#endif
	}

	// Parse inputs to get data
	analogVal = ((rxBuf[1] & 0x03) << 8) | rxBuf[0];

	return analogVal;
}

int main()
{
	int temperature[NUM_TEMP_SENSORS];
	int pressureValue; 
	float pressurePercent, pressurePSI, pressure_mBar;
	int i, fd;

	printf("Starting ADC test:\n\n");

	// Initialize SPI driver
	fd = spiInit(SPI_DEVICE);
	if(fd < 0) {
#ifdef	ES_DEBUG_MODE
		printf("Failed to initialize SPI device\n");
		return fd;
#endif
	}
#ifdef	ES_DEBUG_MODE
	printf("%d is fd from spiInit\n", fd);
#endif

	// Infinite loop. Use ctrl-c to exit
	// while (1)
	// {
		printf("Starting read cycle\n");

		// Read all temperature sensors connected on first channels
		for(i = 0; i < NUM_TEMP_SENSORS; i++) {

			temperature[i] = analogRead(fd, i);
			printf("\tTemp %d: %d\n", i, temperature[i]);

		}

		// Read pressure on the next channel (i is one higher than temperature sensors)
		pressureValue = analogRead(fd, i);

		// According to the pressure sensor datasheet, the pressure
		// analog output ranges from 10% to 90% the analog range, so the
		// analog input needs to be adjusted to reflect the actual
		// pressure
		pressurePercent = pressureValue * 100.0 / 1023;

		// Mapping analog range to pressure range
		// 10% -> 0% (0 PSI)
		// 90% -> 100% (15 PSI, or MAX_PRESSURE_RANGE_PSI)
		// Subtract 10, divide by 80%, then multiply by max PSI range
		pressurePSI = (pressurePercent - 10) / 80 * MAX_PRESSURE_RANGE_PSI;
		
		// Get pressure in mBar, easier conversion
		pressure_mBar = pressurePSI * 68.9476;

		// Print pressure as ADC value, percentage, and PSI
		printf("\tPressure: %d, %.2f PSI, %.2f mBar (%.2f%% ADC range)\n",
				pressureValue, pressurePSI, pressure_mBar, pressurePercent);

		// Wait 200 ms (5 Hz)
		usleep(200000);
	// }

		// Clean up
		spiClose(fd);

	// Will never reach here
	return 0;
}


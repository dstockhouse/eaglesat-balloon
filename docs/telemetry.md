# Description of telemetry sensors for the balloon test

The sensors being flown on the balloon test are used to detect pressure and
temperature of the environment in-flight. The pressure sensor being used is a
Honeywell SSC analog pressure sensor with a 5V power supply. The analog output
voltage ranges from 10% to 90% the supply voltage. The temp sensor hasn't been
determined yet, but I believe it will be as simple as a thermistor voltage
divider.

We will be using the MCP3008 SPI ADC because we have some on hand. It has 8
analog input channels, which is enough for one pressure sensor and 7 temperature
sensor distributed throughout the structure. The MCP3008 has drivers in the
wiringPi library, it has a simple interface for reading from the analog
channels. It also has Linux drivers configurable through the device tree, so
both options are possible but wiringPi is simpler to understand, so we will use
it.

## Needs from OBC

The OBC is responsible for interfacing to all telemetry sensing hardware. It
will use an SPI ADC to interface to an analog pressure sensor (Honeywell SSC)
and 7 thermistor temperature sensors distributed in important places throughout
the structure.

## Contact

David Stockhouse, On-Board Computer Subsystem Lead  
[stockhod@my.erau.edu](mailto:stockhod@my.erau.edu)

Connect on [Facebook](https://www.facebook.com/eaglesaterau/).


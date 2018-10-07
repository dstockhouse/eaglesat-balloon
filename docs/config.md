# Necessary Raspberry Pi configurations for balloon test

Many of the default configurations need to be changed for operating this design.
The configurations are primarily in software modules that need to be installed
and device tree parameters that need to be changed from defaults.

## Device Tree

The configuration for the RPi is modified indirectly using the file
`/boot/config.txt` (as root). This is a method of changing the Linux device tree
parameters. Too much information about the device tree can be found from the
Raspberry Pi website documentation. A wordy device tree overview can be found
[here](https://www.raspberrypi.org/documentation/configuration/device-tree.md)
and more relevant information about configuration options can be found
[here](https://www.raspberrypi.org/documentation/configuration/config-txt/README.md)
and in the file `/boot/overlays/README`.

### Modifications
- Uncomment (or add) the line `dtparam=spi=on` to enable SPI on GPIO pins 7, 8,
  9, 10, and 11 for the MCP3008 ADC to telemetry sensors
- Change `/boot/cmdline.txt` and remove the section `console=serial0,115200` to
  disable using the UART pins as a serial console. Add the line `enable_uart=1`
to `/boot/config.txt` to enable serial port for custom use again.
- Disable Bluetooth using the PL011 UART and restore it to GPIO pins 14 and 15.
  Add the line `dtoverlay=pi3-disable-bt`. It is also necessary to disable the
modem using the UART from systemctl: `systemctl disable hciuart`

### Other notes

I discovered reading `/boot/overlays/README` that the Pi has a driver for the
MCP3008 ADC onboard, configurable with the device tree. I think the wiringPi
solution is sufficient and easy to understand, but for future use the ADC can be
accessed conveniently from the command line with the Linux driver
([tips](https://jumpnowtek.com/rpi/Using-mcp3008-ADCs-with-Raspberry-Pis.html)).

## Module installations

- Software UART for COMMS:
[SoftUART](http://codeintherightway.blogspot.com/2017/09/soft-uart-implementation-for-raspberry.html)
and [GitHub](https://github.com/adrianomarto/soft_uart/)

## Contact

David Stockhouse, On-Board Computer Subsystem Lead  
[stockhod@my.erau.edu](mailto:stockhod@my.erau.edu)

Connect on [Facebook](https://www.facebook.com/eaglesaterau/).


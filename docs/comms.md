# Description of COMMS for the balloon test

The COMMS system will be responsible for communicating telemetry and status
information to the ground, but no science data will be generated.

## Needs from OBC

The interface to COMMS is a UART at 1200 baud.  Because the Raspberry Pi only
has two hardware UARTs, it is possible that this UART will need to be created in
software, especially since it is running at such a slow baud rate. One link I
found that will help for this use is
[here](http://codeintherightway.blogspot.com/2017/09/soft-uart-implementation-for-raspberry.html).

When the radio starts up, it sends through UART a string containing product
version and licensing information, followed by the string "cmd: ". At this
point, to get the radio to enter transmit mode, the OBC needs to send "K\r"
through the UART. Any packets sent after this terminated by '\r' will be sent
through the transceiver to the ground. As packets are sent to the radio they are
echoed back through the UART. If the radio is ever reset, "K\r" needs to be sent
again to enter transmit mode, so the OBC needs a way of detecting if that string
is ever received through the UART and respond by sending back the start string.

To terminate a packet to be sent, I believe only a carriage return '\r'
character is needed. In PuTTY, the radio seems to send a '\n\r' whenever it
echoes back to the terminal or sends a any other data. No other delimiters are
necessary for the protocol, but it would be useful for understanding data on the
ground if packets included delimiters between unrelated data components. Austin
has recommended using a comma. 

Health data will be sent to the ground every 30 seconds after starting the
mission, with a heartbeat flood character sent once every second. The health
data to be sent to the ground will include:
- Elapsed time since the start of the mission
- Data from all 8 telemetry sensors (7 temp and 1 pressure)
- Metadata from CRP and MDE (cycle count, amount of data generated)

The balloon will also receive a few commands from the ground:
- Hard reset
- Enter low-power mode
- Enter normal operating mode from low-power mode

Low-power mode will be triggered by the temperature on the DC voltage regulators
reaching a high level yet to be determined.

## Contact

David Stockhouse, On-Board Computer Subsystem Lead  
[stockhod@my.erau.edu](mailto:stockhod@my.erau.edu)

Connect on [Facebook](https://www.facebook.com/eaglesaterau/).


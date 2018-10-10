import serial

#Open Serial Port
serialPort = serial.Serial('/dev/ttyS0', timeout = 1)

#Open Txt File
output = open('data.raw', 'wb')

i = 0
while i < 1000:
    i = i + 1
    text = serialPort.readline()
    line = 'line: ' + str(i)
    print(line)
    print(text)
    output.write(text)
output.close()

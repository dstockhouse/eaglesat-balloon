import serial

def crp_imageGet(filename):

    #Open Serial Port
    serialPort = serial.Serial('/dev/ttyAMA0', timeout = 1)

    #Open Txt File
    output = open(filename, 'wb')

    text = serialPort.readline()
    line = 'line: ' + str(i)
    print(line)
    print(text)
    output.write(text)

    output.close()


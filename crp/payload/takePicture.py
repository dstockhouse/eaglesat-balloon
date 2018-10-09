#Wakes Camera and Takes a Picture
import serial
import time

def takePic(num):
        i = 0
        while i < num:
                #Take Picture
                port.write(b'\xAA\x04\x02\x00\x00\x00')

                #Wait for Data
                time.sleep(10)

                #Thanks Camera
                port.write(b'\xAA\x0E\x0D\x00\x00\x00')
        
                i = i + 1

#Opens port
port = serial.Serial('/dev/ttyS0')

#Reset Camera
port.write(b'\xAA\x08\x00\x00\x00\xFF')
time.sleep(1)

#Syncs Camera
syncCount = 0
while syncCount < 60:
        port.write(b'\xAA\x0D\x00\x00\x00\x00')
        syncCount = syncCount + 1
        port.write(b'\xAA\x0E\x0D\x00\x00\x00')

#Wait 2 Sec
time.sleep(2)

#Initalize Camera
port.write(b'\xAA\x01\x00\x08\x09\x07')
time.sleep(1)

takePic(1)

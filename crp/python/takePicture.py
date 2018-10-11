#Wakes Camera and Takes a Picture
import serial
import time

def takePic(num,delay):
        i = 0
        while i < num:
                #Take Picture
                port.write(b'\xAA\x04\x02\x00\x00\x00')
                print("Taking Image")
                
                #Wait for Data
                time.sleep(10)

                #Thanks Camera
                port.write(b'\xAA\x0E\x0D\x00\x00\x00')
                print("Data Recived")
                i = i + 1
                time.sleep(delay)

#Python serial device

#Opens port
port = serial.Serial('/dev/ttyAMA0')
print("Serial Open")

#Software Reset Camera
port.write(b'\xAA\x08\x00\x00\x00\xFF')
time.sleep(1)
print("Camera Reset")

#Syncs Camera
syncCount = 0
while syncCount < 60:
        port.write(b'\xAA\x0D\x00\x00\x00\x00')
        syncCount = syncCount + 1
port.write(b'\xAA\x0E\x0D\x00\x00\x00')
print("Synced")

#Wait 2 Sec
time.sleep(2)
print("Warmed")

#Initalize Camera
port.write(b'\xAA\x01\x00\x03\x09\x07')
time.sleep(1)
port.write(b'\xAA\x15\x00\x00\x00\x00')
time.sleep(1)
print("Initalized")

takePic(1,1)

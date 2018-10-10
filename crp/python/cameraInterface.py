### cameraInterface.py
#
#   Functions:
#       serialInit
#           Initilises Serial Connection and Listener
#       serialCmd
#           Sends Serial Command, Resets Listener
#       serialGet
#           Gets Image, Postpones Listener
#       serialClose
#           Closes Serial Connection and Listener
#       listener
#           Reads and Uses Data from Device
#
#   Serial Ports:
#       GPIO Pin 14 - uCam Pin 3 - Data     '/dev/ttyAMA0'
#       GPIO Pin 15 - uCam Pin 2 - Control  '/dev/ttyAMA0'
#       GPIO Pin 18 - uCam Pin 5 - Reset    
#
#   Commands:
#       -Inital                          }cmd_init{
#       -Get Picture                     }get_pic{
#       Snapshot                        }get_snap{
#       Set Package Size                }set_pksz{
#       Set Baud Rate                   }set_baud{
#       -Reset                           }cmd_rest{
#       Data                            }set_data{
#       -Sync                            }cmd_sync{
#       Ack                             }cmd_ack{
#       Nck                             }cmd_nak{
#       Light                           }set_light{
#       Contrast/Brightness/Exposure    }set_cbe{
#       Sleep                           }cmd_slp{
#
###

#Import Packages
import serial
import time
import os

#Define Serial Connection, Automaticly Set to 9600 Baud, 8 Bits, No Parity,
#and One Stopbit
serialC = serial.Serial('/dev/ttyAMA0')

#Activate Read Script
os.system('python serialRead.py')

#Reset Camera
serialC.write(b'\xAA\x08\x00\x00\x00\xFF')
time.sleep(1)
print("Camera Reset")

#Define Functions
def serialInit():

    #State Open Channels
    print(serialC.name," Open as I/O")

    #Sync Device
    serialCmd('cmd_sync')

    #Warm Camera
    time.sleep(2)
    print("Camera Warmed")
    
    #Initalise Device
    serialCmd('cmd_init')
    pass


def serialCmd(command):

    if command == "help":
        print("Help in Module")
        
    elif command == "manual":
        serialCommand = input("Command: ")
        serialC.write(command)
        
    elif command == "cmd_init":
        #Initalize
        serialC.write(b'\xAA\x01\x00\x08\x09\x07')
        time.sleep(1)
        
        #Stop Sleep
        serialC.write(b'\xAA\x15\x00\x00\x00\x00')
        time.sleep(1)
        
        print("Initalized")

    elif command == "get_pic":
            #Take Picture
            serialC.write(b'\xAA\x04\x02\x00\x00\x00')
            print("Taking Image")
                
            #Wait for Data
            time.sleep(140)

            #Thanks Camera
            serialC.write(b'\xAA\x0E\x0D\x00\x00\x00')
            print("Data Recived")
            time.sleep(1)
            
    elif command == "get_snap":
        print("Not yet Implemented")
        
    elif command == "set_pksz":
        print("Not yet Implemented")
        
    elif command == "set_baud":
        print("Not yet Implemented")
        
    elif command == "cmd_rest":
        print("Not yet Implemented")
        
    elif command == "set_data":
        print("Not yet Implemented")
        
    elif command == "cmd_sync":
        syncCount = 0
        while syncCount < 60:
            serialC.write(b'\xAA\x0D\x00\x00\x00\x00')
            syncCount = syncCount + 1
        serialC.write(b'\xAA\x0E\x0D\x00\x00\x00')
        
    elif command == "cmd_ack":
        print("Not yet Implemented")
    elif command == "cmd_nak":
        print("Not yet Implemented")
    elif command == "set_light":
        print("Not yet Implemented")
    elif command == "set_cbe":
        print("Not yet Implemented")
    elif command == "cmd_slp":
        print("Not yet Implemented")
    else:
        print("Invalid Command")
    pass

def takePic(num,delay, filename):
        i = 0
        open(filename, 'wb');
        while i < num:
                #Take Picture
                serialC.write(b'\xAA\x04\x02\x00\x00\x00')
                print("Taking Image " + str(i + 1) + " Out of " + str(num))
                
                #Wait for Data
                #time.sleep(120)

                #Thanks Camera
                serialC.write(b'\xAA\x0E\x0D\x00\x00\x00')
                i = i + 1
                print("Data Recived for Image: " + str(i) + " Out of " + str(num))
                time.sleep(delay)

def serialClose():
    #End Dataloop
    loop_datacheck = False

    #Close Serial Connection
    serialC.close()
    pass

if __name__ == "__main__":
    #Run Camera
    serialInit()
    takePic(3,1)
    serialClose()

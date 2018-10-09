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
#       GPIO Pin 14 - uCam Pin 3 - Data     '/dev/ttyS0'
#       GPIO Pin 15 - uCam Pin 2 - Control  '/dev/ttyS0'
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

#Define Serial Connection, Automaticly Set to 9600 Baud, 8 Bits, No Parity,
#and One Stopbit
serialC = serial.Serial('/dev/ttyS0')

#Define Functions
def serialInit():

    #State Open Channels
    print(serialC.name," Open as I/O")

    #Sync Device
    serialCmd('cmd_sync')
    
    #Initalise Device
    serialCmd('cmd_init')

    #Start Dataloop
    
    pass


def serialCmd(command):

    if command == "help":
        print("Help in Module")
        
    elif command == "manual":
        serialCommand = input("Command: ")
        serialC.write(command)
        
    elif command == "cmd_init":
        serialC.write(b'\xAA\x01\x00\x08\x09\x07')
        
    elif command == "get_pic":
        serialC.write(b'\xAA\x04\x02\x00\x00\x00')
        
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


def serialClose():
    #End Dataloop
    loop_datacheck = False

    #Close Serial Connection
    serialC.close()
    pass


def serialListen(serial_port):
    while loop_datacheck == True:
        read = serialC.read(4)
        if read == "AA0A":
            loop_dl = True
        else:
            loop_dl = False
        while loop_dl == True:
            data =  data + serialC.read()
    pass


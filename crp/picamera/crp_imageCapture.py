import picamera
from datetime import datetime
from time import sleep, time
from fractions import Fraction

def crp_runContinuous():

    camera = picamera.PiCamera(resolution=(2592, 1944), framerate=Fraction(1, 6))
    #print "setting shutter"
    camera.shutter_speed = 5000000
    #print "iso"
    #camera.iso = 800
    #print "setting exposure mode"
    camera.exposure_mode = 'off'

    captureCount = 0

    #print "Starting capture " + str(i)
    while True:

        now = datetime.now()
        filename = "/home/pi/captures/capture" + str(captureCount) + "-" + str(now.month) + "." + str(now.day) + "." + str(now.year) + "_" + str(now.hour) + ":" + str(now.minute) + ":" + str(now.second) + ".png"

        # Get lots of images
        camera.capture(filename, format = 'png')

        captureCount += 1

    #print "Closing camera module"
    camera.close()

def crp_imageCaptureExposure(filename, exposure):

    camera = picamera.PiCamera(resolution=(2592, 1944), framerate=Fraction(1, 6))
    print "setting shutter"
    camera.shutter_speed = exposure * 1000000
    #print "iso"
    #camera.iso = 800
    print "setting exposure mode"
    camera.exposure_mode = 'off'

    if not isinstance(filename, basestring):
        print "Input is not a string"
        return -1

    for i in range(0, 3):
        print "Starting capture " + str(i)
        camera.capture(str(i) + filename, format = 'png')


    print "Closing camera module"
    camera.close()

def crp_imageCaptureLong(filename):

    camera = picamera.PiCamera(resolution=(2592, 1944))

    print "Setting exposure mode"
    camera.exposure_mode = 'verylong'

    if not isinstance(filename, basestring):
        print "Input is not a string"
        return -1

    print "Starting long capture"
    camera.capture(filename, format='png')

    camera.close()


def crp_imageCaptureShort(filename):

    camera = picamera.PiCamera(resolution=(2592, 1944))

    if not isinstance(filename, basestring):
        print "Input is not a string"
        return -1

    camera.capture(filename, format='png')

    camera.close()


if __name__ == '__main__':
    # print "Running as main file"

    crp_runContinuous()

#    crp_imageCaptureExposure('templong.png', 1)
#
#    print "Short capture:"
#    for i in range(10):
#        print "Starting capture " + str(i)
#        crp_imageCaptureShort('tempShort' + str(i) + '.png')
#        print "End of capture"
#        sleep(1)


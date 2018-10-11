from picamera import PiCamera
from time import sleep, time
from fractions import Fraction

camera = PiCamera(resolution=(2592, 1944), framerate=Fraction(1, 1))
camera.shutter_speed = 600000
camera.iso = 800
#sleep(30)
sleep(2)
camera.exposure_mode = 'off'
print(camera.shutter_speed)
start = time()
camera.capture('dark.png', format = 'png')
print(time() - start)

camera.close()


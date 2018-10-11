from picamera import PiCamera
from time import sleep, time
from fractions import Fraction

camera = PiCamera(resolution=(2592, 1944), framerate=Fraction(1, 6))
camera.shutter_speed = 6000000
camera.iso = 800
sleep(30)
camera.exposure_mode = 'off'
print(camera.shutter_speed)
start = time()
camera.capture('dark.png', format = 'png')
print(time() - start)

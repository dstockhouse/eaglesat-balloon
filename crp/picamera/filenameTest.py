from datetime import datetime

captureCount = 4
now = datetime.now()

filename = "capture" + str(captureCount) + "-" + str(now.month) + "." + str(now.day) + "." + str(now.year) + "_" + str(now.hour) + ":" + str(now.minute) + ":" + str(now.second) + ".png"

print filename

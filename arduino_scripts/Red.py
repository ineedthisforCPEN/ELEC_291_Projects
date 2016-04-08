import serial, sys
import subprocess as sub
import time

#path = "/dev/"
#dirs = os.listdir(path)
listdev = sub.Popen(["ls", "/dev/"], stdout = sub.PIPE)
path = sub.check_output(("grep", "ttyACM"), stdin = listdev.stdout)
ser = serial.Serial("/dev/{}".format(path[:-1]), 9600)

time.sleep(2)
ser.write('32')

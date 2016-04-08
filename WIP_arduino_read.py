import os
import serial
import subprocess
from time import sleep
from sys import argv, exit

#--------------------------------------------------
# INFORMATION SENT TO THE ARDUINO
#
# This script sends only one byte of data at a time
# to the arduino. The byte represents a very
# specific set on instructions for the arduino
#
# BYTE:
#   Each bit, shown below, represents one part of
#   the arduino instruction
#
#   Bit number:   7   6   5   4   3   2   1   0
#   Instruction:  x   rg  l   b   a   u   h   t
#
# INSTRUCTION DECODING
#   Bit number - purpose
#
#   t  - Bit 0 is 1 if the arduino should send
#        temperature data, 0 otherwise
#   h  - Bit 1 is 1 if the arduino should send
#        humidity data, 0 otherwise
#   u  - Bit 2 is 1 if the arduino should send
#        ultrasonic sensor data, 0 otherwise
#   a  - Bit 3 is 1 if the arduino should send
#        accelerometer data, 0 otherwise
#   b  - Bit 4 is 1 if the arduino should send
#        button data, 0 otherwise
#   l  - Bit 5 is 1 if the arduino should turn the
#        LED light on, 0 to turn off
#   rg - Bit 6 is 1 if the arduino should make the
#        LED green, 0 to make it red
#   x  - Bit 7 does not matter. It can be either 0
#        or 1`
#--------------------------------------------------

#--------------------------------------------------
# MAIN FUNCTION
#--------------------------------------------------

# See if the script is being used properly (requires a second argument)
#if len(argv) < 2:
#    exit("Invalid usage of {}. Must have one argument between 0 and 255 inclusive".format(argv[0]))

# See if the second argument is valid (must be a number between 0 and 255 - a byte)
#if int(argv[1]) < 0 or int(argv[1]) > 255:
#    exit("Ivalid second argument {}. Must be between 0 and 255 inclusive".format(argv[1]))

# Get path to arduino
listdev = subprocess.Popen(['ls', '/dev/'], stdout = subprocess.PIPE)
path = subprocess.check_output(('grep', 'ttyACM'), stdin = listdev.stdout)

# Create serial object for arduino
ser = serial.Serial('/dev/{}'.format(path[:-1]), 9600)
sleep(2)
ser.write(argv[1])

# Wait for arduino to be ready
sleep(2)

# Read values from arduino
arduino_output = ser.readline()
output_list = arduino_output.split('|')
print(arduino_output)
print(output_list)
a_value = ""    # Value for the accelerometer
b_value = ""    # Value for the button 

# Determine which data should go where
if int(argv[1]) & 1 and len(output_list) > 0:
    try:
        t_file = open('/var/www/wwwdata/temperature.txt', 'w')
    except:
        exit('ERROR: Could not open temperature file - make sure you have the appropriate permission')

    t_file.write(output_list.pop(0))
    t_file.close()

if int(argv[1]) & 2 and len(output_list) > 0:
    try:
        h_file = open('/var/www/wwwdata/humidity.txt', 'w')
    except:
        exit('ERROR: Could not open humidity file - make sure you have the appropriate permission')

    h_file.write(output_list.pop(0))
    h_file.close()


#if int(argv[1]) & 4 and len(output_list) > 0:
    #try:
        #u_file = open('/var/www/wwwdata/ultrasonicsensor.txt', 'w')
    #except:
        #exit('ERROR: Could not open ultrasonicsensor file - make sure you have the appropriate permission')

    #u_file.write(output_list.pop(0))
    #u_file.close()

#check to see if the user is at the distance threshold from the mirror
if len(output_list) > 0:
	screen_wake = int(output_list.pop()[:-1])
	if screen_wake: os.system('')

if int(argv[1]) & 8 and len(output_list) > 0:
    try:
        a_file = open('/var/www/wwwdata/accelerometer.txt', 'w')
    except:
        exit('ERROR: Could not open accelerometer file - make sure you have the appropriate permission')

    a_value = output_list.pop(0)
    a_file.write(a_value)
    a_file.close()

if int(argv[1]) & 16 and len(output_list) > 0:
    b_value = output_list.pop(0)

# Check to see if music should be stopped (stop if button read is 1)
if len(output_list) > 0:
    music_stop = int(output_list.pop()[:-1])
    if music_stop: os.system('kill $(pgrep omxplayer)')

# Change the boot config.txt file based on accelerometer value
if a_value == '4':  # Set screen orientation to be vertical
    os.system("sudo sed -i '61s/.*/display_rotate=3/' /boot/config.txt")
else:               # Set screen orientation to be horizontal
    os.system("sudo sed -i '61s/.*/#display_rotate=3/' /boot/config.txt")

# Based on the value of the push-button, change the orientation of the screen
# Screen orientation will only change when the raspberry pi reboots
if b_value == '1':    # Set screen orientation to be vertical
    rbtfile = open('/var/www/wwwdata/rbtflag', 'w')
    rbtfile.close()
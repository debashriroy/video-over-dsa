import os
import sys
import math
import struct
import threading
from datetime import datetime
import time

class ThreadClass(threading.Thread):
    def run(self):
        return

framerate=25
filesink_location="/home/droy/Documents/GNURadio_Files/video1.ts"
enc_bitrate= 10
time= 5

enc_width=640
enc_height=480

#Name without .c extension
cprog_name= "working"


command= "./"+str(cprog_name)+" "+str(framerate)+" "+filesink_location;

print command
os.system(command)


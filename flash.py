#heavily inspired on https://github.com/Fri3dCamp/Fri3dBadge/blob/master/bin/defaultFirmware/flash.py
import sys
import glob
import subprocess

from serial.tools import list_ports

VID = 4292
PID = 60000

def get_serial():
	device_list = list_ports.comports()
	devices = list()
	for device in device_list:
		if (device.vid != None or device.pid != None):
			if (device.vid == VID and
				device.pid == PID):
					#print('found port ' + device.device)
					devices.append(device)
					
	return devices
	
last_run = list()
while True:
	this_run = get_serial()
	for port in this_run:
		if port not in last_run:
			subprocess.Popen([r'C:\Users\ward\.platformio\penv\Scripts\pio.exe', "run", "-t", "upload", "--upload-port" , port.device])
	last_run = this_run
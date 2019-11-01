from __future__ import print_function, division
from pynput import keyboard
import serial
import time


command = ["Stopped", 0]
incrament = 4

def on_press(key):
	try:
		character = key.char

		if character == "w":
			command[0] = "Forward"
		elif character == "s":
			command[0] = "Reverse"

		
	except AttributeError:
		if key == keyboard.Key.up:
			command[1] += incrament
		elif key == keyboard.Key.down:
			command[1] -= incrament


def on_release(key):
	try: 
		key.char
		command[0] = "Stopped"

	except AttributeError:
		return 0

listener = keyboard.Listener(
    on_press=on_press,
    on_release=on_release)
listener.start()

arduino = serial.Serial('/dev/tty.usbmodem14301', 9600)
while 1:
    print(command[0], command[1])

    if command[0] == "Forward":
        arduino.write(bytes(str(2), 'utf-8'))
        arduino.write(bytes(str(command[1]), 'utf-8'))
    elif command[0] == "Reverse":
        arduino.write(bytes(str(3), 'utf-8'))
    else:
        arduino.write(bytes(str(5), 'utf-8'))


    time.sleep(0.02)
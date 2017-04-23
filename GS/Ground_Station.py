import serial
from PyQt4 import QtGui, QtCore
from time import sleep

CMD_RELEASE = 'r'
CMD_BUZZER = 'b'
CMD_RESET = 'z'
CLOSE = 'c'

commands = [CMD_RELEASE, CMD_BUZZER, CMD_RESET]

ser = serial.Serial('COM3')

packet_limit = 0

while True:


    telem = ser.readline()
    print(telem)
    packet_limit += 1

    if packet_limit == 10:
        break

ser.close()
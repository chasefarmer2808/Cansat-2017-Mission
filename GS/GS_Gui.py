import sys
import serial
import csv
from PyQt4 import QtGui, QtCore

CMD_RELEASE = 'r'
CMD_BUZZER = 'b'
CMD_RESET = 'z'
CMD_LAND = 'l'

CSV_FILE_NAME = 'telemetry.csv'

"""
try:
    xbee = serial.Serial('COM3', timeout=0.1)  #initialize a serial object on the com port
except:
    pass

#function read_packet()
'''
    this function is called by the timer in order to have pseudo parallelism between
    drawing the GUI and monitoring the serial port.
'''
def read_packet():
    packet = xbee.readline()  #read chars until '\r\n'

    if packet: #if any chars were read
        print(packet)
        write_csv(packet)  #save telemetry packet to csv
"""

def write_csv(packet):
    with open(CSV_FILE_NAME, 'ab') as csvfile:
        writer = csv.writer(csvfile, delimiter=',')
        writer.writerow([packet])
        csvfile.close()

'''
    this is a handler class for PyQt.  It handles drawing the window, buttons, and graphs
'''
class Window(QtGui.QMainWindow):

    def __init__(self):
        super(Window, self).__init__()
        self.setGeometry(50, 50, 500, 300)
        self.setWindowTitle("GS")
        self.cmd_zero_btn()
        self.cmd_buzz_btn()
        self.cmd_release_btn()
        self.show()

    def cmd_zero_btn(self):
        btn = QtGui.QPushButton("Reset Save Data", self)
        btn.clicked.connect(lambda: self.send_cmd(CMD_RESET))
        btn.resize(200, 50)

    def cmd_buzz_btn(self):
        btn = QtGui.QPushButton("Sounds Buzzer", self)
        btn.clicked.connect(lambda: self.send_cmd(CMD_BUZZER))
        btn.resize(200, 50)
        btn.move(0, 50)

    def cmd_release_btn(self):
        btn = QtGui.QPushButton("RELEASE", self)
        btn.clicked.connect(lambda: self.send_cmd(CMD_RELEASE))
        btn.resize(200, 50)
        btn.move(0, 100)

    def send_cmd(self, cmd):
        xbee.write(cmd)



app = QtGui.QApplication(sys.argv)
GUI = Window()

timer = QtCore.QTimer()
timer.timeout.connect(read_packet)
timer.start(10)

sys.exit(app.exec_())
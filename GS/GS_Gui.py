import sys
import serial
from PyQt4 import QtGui, QtCore

xbee = serial.Serial('COM3')

class Window(QtGui.QMainWindow):

    def __init__(self):
        super(Window, self).__init__()
        self.setGeometry(50, 50, 500, 300)
        self.setWindowTitle("GS")
        self.cmd_zero_btn()
        self.show()

    def cmd_zero_btn(self):
        btn = QtGui.QPushButton("Reset Save Data", self)
        btn.clicked.connect(self.send_cmd)
        #btn.resize(100, 100)
        #btn.move(100, 100)
        #self.show()

    def send_cmd(self):
        print('here')
        xbee.write('z')

app = QtGui.QApplication(sys.argv)
GUI = Window()
sys.exit(app.exec_())
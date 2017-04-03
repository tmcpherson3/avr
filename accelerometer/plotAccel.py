# import matplotlib, sys
# matplotlib.use('TkAgg')
# from numpy import arange, sin, pi
# from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
# from matplotlib.figure import Figure
# from Tkinter import *
# 
# top = Tk()
# top.title("Hello World!")
# #-------------------------------------------------------------------------------
# 
# f = Figure(figsize=(5,4), dpi=100)
# a = f.add_subplot(111)
# t = arange(0.0,3.0,0.01)
# s = sin(2*pi*t)
# a.plot(t,s)
# 
# 
# dataPlot = FigureCanvasTkAgg(f, master=top)
# dataPlot.show()
# dataPlot.get_tk_widget().pack(side=TOP, fill=BOTH, expand=1)
# #-------------------------------------------------------------------------------
# top.mainloop()

#---------------------------
#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
ZetCode PyQt4 tutorial 

In this example, we connect a signal
of a QtGui.QSlider to a slot 
of a QtGui.QLCDNumber. 

author: Jan Bodnar
website: zetcode.com 
last edited: October 2011
"""

import sys
import serial
from PyQt4.QtCore import QObject, pyqtSignal, pyqtSlot, QTimer
from PyQt4 import QtGui, QtCore


class Example(QtGui.QWidget):
    
    def __init__(self):
        super(Example, self).__init__()
        
        self.initUI()
        
    def initUI(self):
        
        self.lcd = QtGui.QLCDNumber(self)
        self.sld = QtGui.QSlider(QtCore.Qt.Horizontal, self)
        self.timer = QTimer()
        self.timer.timeout.connect(self.readSerial)
        self.ser = serial.Serial('/dev/tty.usbmodemfd131', 9600, timeout=1)
		
        vbox = QtGui.QVBoxLayout()
        vbox.addWidget(self.lcd)
        vbox.addWidget(self.sld)

        self.setLayout(vbox)
        self.sld.valueChanged.connect(self.lcd.display)
        
        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Signal & slot')
        self.show()
        
        self.timer.start(1000)
    
    def readSerial(self):
    	n = self.ser.inWaiting()
    	if n > 0:
    		line = self.ser.read(self.ser.inWaiting())
    		self.lcd.display(line.strip())
    
    def closeEvent(self, event):
    	self.ser.close()
    	
        
def main():
    
    app = QtGui.QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()


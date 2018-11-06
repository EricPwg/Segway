# -*- coding: utf-8 -*-
"""
Created on Mon Oct 22 23:00:06 2018

@author: user
"""

comnum = 'COM3'
increase_num = 5

import tkinter as tk
import math
import serial


ser = serial.Serial(comnum, 115200, timeout = 0.5)
print(ser.name)
print(ser.port)
#ser.open()


Len1 = 114
Len2 = 80
Del = 12.5

win = tk.Tk()
win.title('tk test')

win.geometry('300x500')
win.resizable(1, 0)

t = 0

c_a0 = 90
c_a1 = 90
c_t1 = 90
c_t2 = 90
c_t3 = 90

def send_serial():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Langle
    Langle.config(text = 'theta1: '+str(c_t3)+'\ntheta2: '+str(c_t2)+'\ntheta3: '+str(c_t1)+'\ntheta4: '+str(c_a1)+'\ntheta5: '+str(c_a0))
    s = str(c_a0)+' '+str(c_a1)+' '+str(c_t3)+' '+str(c_t2)+' '+str(c_t1)+' '
    s = s.encode()
    ser.write(s)
    

def cal(x, y, z):
    len1 = math.sqrt(x**2+z**2)
    pre_theta1 = math.acos(x/len1)*180/math.pi
    theta1 = -pre_theta1+115
    
    Len3 = math.sqrt(y**2+(len1-Del)**2)
    angPDL = math.acos((len1-Del)/Len3)*180/math.pi
    phi2 = math.acos( (Len1**2+Len3**2-Len2**2)/(2*Len1*Len3) )*180/math.pi
    phi3 = math.acos( (Len1**2+Len2**2-Len3**2)/(2*Len1*Len2) )*180/math.pi
    pre_theta2 = angPDL-phi2
    pre_theta3 = 180-phi3
    theta2 = -pre_theta2+90
    theta3 = pre_theta3+90
    print(theta1)
    print(theta2)
    print(theta3)
    theta1 = int(theta1)
    theta2 = int(theta2)
    theta3 = int(theta3)
    return [theta1, theta2, theta3]

def on_click():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Lnote
    x = int(e1.get())
    y = int(e2.get())
    z = int(e3.get())
    try:
        aa = cal(x, y, z)
        if aa[0] < 0 or aa[0] > 180 or aa[1] < 0 or aa[1] > 180 or aa[2] < 0 or aa[2] > 180:
            Lnote.config(text='OUT OF RANGE')
        else:
            c_t1 = aa[0]
            c_t2 = aa[1]
            c_t3 = aa[2]
            send_serial()
            Lnote.config(text='success')
    except:
        Lnote.config(text='OUT OF RANGE')
        
def xu_click():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Lnote, e1
    x = int(e1.get())+increase_num
    print(x)
    y = int(e2.get())
    z = int(e3.get())
    try:
        aa = cal(x, y, z)
        if aa[0] < 0 or aa[0] > 180 or aa[1] < 0 or aa[1] > 180 or aa[2] < 0 or aa[2] > 180:
            Lnote.config(text='OUT OF RANGE')
        else:
            c_t1 = aa[0]
            c_t2 = aa[1]
            c_t3 = aa[2]
            send_serial()
            e1.delete(0, 10)
            e1.insert(0, x)
            Lnote.config(text='success')
    except:
        Lnote.config(text='OUT OF RANGE')
        
def xd_click():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Lnote, e1
    x = int(e1.get())-increase_num
    y = int(e2.get())
    z = int(e3.get())
    try:
        aa = cal(x, y, z)
        if aa[0] < 0 or aa[0] > 180 or aa[1] < 0 or aa[1] > 180 or aa[2] < 0 or aa[2] > 180:
            Lnote.config(text='OUT OF RANGE')
        else:
            c_t1 = aa[0]
            c_t2 = aa[1]
            c_t3 = aa[2]
            send_serial()
            e1.delete(0, 10)
            e1.insert(0, str(x))
            Lnote.config(text='success')
    except:
        Lnote.config(text='OUT OF RANGE')
        
def yu_click():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Lnote, e2
    x = int(e1.get())
    y = int(e2.get())+increase_num
    z = int(e3.get())
    try:
        aa = cal(x, y, z)
        if aa[0] < 0 or aa[0] > 180 or aa[1] < 0 or aa[1] > 180 or aa[2] < 0 or aa[2] > 180:
            Lnote.config(text='OUT OF RANGE')
        else:
            c_t1 = aa[0]
            c_t2 = aa[1]
            c_t3 = aa[2]
            send_serial()
            e2.delete(0, 10)
            e2.insert(0, y)
            Lnote.config(text='success')
    except:
        Lnote.config(text='OUT OF RANGE')
        
def yd_click():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Lnote, e2
    x = int(e1.get())
    y = int(e2.get())-increase_num
    z = int(e3.get())
    try:
        aa = cal(x, y, z)
        if aa[0] < 0 or aa[0] > 180 or aa[1] < 0 or aa[1] > 180 or aa[2] < 0 or aa[2] > 180:
            Lnote.config(text='OUT OF RANGE')
        else:
            c_t1 = aa[0]
            c_t2 = aa[1]
            c_t3 = aa[2]
            send_serial()
            e2.delete(0, 10)
            e2.insert(0, y)
            Lnote.config(text='success')
    except:
        Lnote.config(text='OUT OF RANGE')
        
def zu_click():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Lnote, e3
    x = int(e1.get())
    y = int(e2.get())
    z = int(e3.get())+increase_num
    try:
        aa = cal(x, y, z)
        if aa[0] < 0 or aa[0] > 180 or aa[1] < 0 or aa[1] > 180 or aa[2] < 0 or aa[2] > 180:
            Lnote.config(text='OUT OF RANGE')
        else:
            c_t1 = aa[0]
            c_t2 = aa[1]
            c_t3 = aa[2]
            send_serial()
            e3.delete(0, 10)
            e3.insert(0, z)
            Lnote.config(text='success')
    except:
        Lnote.config(text='OUT OF RANGE')
        
def zd_click():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Lnote, e3
    x = int(e1.get())
    y = int(e2.get())
    z = int(e3.get())-increase_num
    try:
        aa = cal(x, y, z)
        if aa[0] < 0 or aa[0] > 180 or aa[1] < 0 or aa[1] > 180 or aa[2] < 0 or aa[2] > 180:
            Lnote.config(text='OUT OF RANGE')
        else:
            c_t1 = aa[0]
            c_t2 = aa[1]
            c_t3 = aa[2]
            send_serial()
            e3.delete(0, 10)
            e3.insert(0, z)
            Lnote.config(text='success')
    except:
        Lnote.config(text='OUT OF RANGE')
    
def open_click():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Lnote
    c_a0 = 125
    send_serial()
    Lnote.config(text='success')
    
def CW_click():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Lnote
    t = c_a1 + 10
    if t > 180 or t < 0:
        Lnote.config(text='OUT OF RANGE')
    else:
        c_a1 = t
        send_serial()
        Lnote.config(text='success')
        
def CCW_click():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Lnote
    t = c_a1 - 10
    if t > 180 or t < 0:
        Lnote.config(text='OUT OF RANGE')
    else:
        c_a1 = t
        send_serial()
        Lnote.config(text='success')
    
def close_click():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Lnote
    c_a0 = 65
    send_serial()
    Lnote.config(text='success')

def home_click():
    global c_a0, c_a1, c_t1, c_t2, c_t3, Lnote, e1, e2, e3
    c_a1 = 90
    c_t1 = 90
    c_t2 = 90
    c_t3 = 90
    send_serial()
    e1.delete(0, 10)
    e2.delete(0, 10)
    e3.delete(0, 10)
    Lnote.config(text='success')

def en_click():
    global ser
    ser.close()
    win.destroy()



b1 = tk.Button(win, text='open', width=10, height=2, bg='green', command=open_click)
b1.grid(row = 4, column = 0)
b2 = tk.Button(win, text='close', width=10, height=2, bg='green', command=close_click)
b2.grid(row = 4, column = 1)

bcw = tk.Button(win, text='CW', width=10, height=2, bg='green', command=CW_click)
bcw.grid(row = 5, column = 0)
bccw = tk.Button(win, text='CCW', width=10, height=2, bg='green', command=CCW_click)
bccw.grid(row = 5, column = 1)

bh = tk.Button(win, text='home', width=10, height=2, command=home_click)
bh.grid(row = 6, column = 0)

be = tk.Button(win, text='exit', width=10, height=2, command=en_click)
be.grid(row = 6, column = 1)

b = tk.Button(win, text='GOTO', width=10, height=2, bg='green', command=on_click)
b.grid(row = 3, column = 1)
xu = tk.Button(win, text='+', width=2, command=xu_click)
xd = tk.Button(win, text='-', width=2, command=xd_click)
xu.grid(row = 0, column = 2)
xd.grid(row = 0, column = 3)
yu = tk.Button(win, text='+', width=2, command=yu_click)
yd = tk.Button(win, text='-', width=2, command=yd_click)
yu.grid(row = 1, column = 2)
yd.grid(row = 1, column = 3)
zu = tk.Button(win, text='+', width=2, command=zu_click)
zd = tk.Button(win, text='-', width=2, command=zd_click)
zu.grid(row = 2, column = 2)
zd.grid(row = 2, column = 3)

L1 = tk.Label(win, text='X', width=5)
L2 = tk.Label(win, text='Y', width=5)
L3 = tk.Label(win, text='Z', width=5)
L1.grid(row=0, column=0)
L2.grid(row=1, column=0)
L3.grid(row=2, column=0)

Lnote = tk.Label(win, text='', width=20, bg='yellow')
Lnote.grid(row=10, column=0, columnspan = 3)

Langle = tk.Label(win, text='', width=20, bg='yellow')
Langle.grid(row=11, column=0, columnspan = 5, rowspan = 3)

e1 = tk.Entry(win)
e1.grid(row=0, column=1)
e2 = tk.Entry(win)
e2.grid(row=1, column=1)
e3 = tk.Entry(win)
e3.grid(row=2, column=1)

win.mainloop()
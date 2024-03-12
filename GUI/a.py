import tkinter as tk
from tkinter import *
import numpy as np
import fileinput

filename = "xd.txt"

a0 = [[0] * 1 for i in range(784)]
a1 = [[0] * 1 for i in range(16)]
a2 = [[0] * 1 for i in range(16)]
a3 = [[0] * 1 for i in range(10)]
w01 = [[0] * 784 for i in range(16)]
w12 = [[0] * 16 for i in range(16)]
w23 = [[0] * 16 for i in range(10)]
b1 = [[0] * 16 for i in range(1)]
b2 = [[0] * 16 for i in range(1)]
b3 = [[0] * 10 for i in range(1)]
t = [0] * 16

def rec(xpos, ypos):
    xpos = np.floor(xpos * 28 / 300)
    ypos = np.floor(ypos * 28 / 300)
    pos = ypos * 28 + xpos
    a0[int(pos)][0] = 1

def reset():
    for i in range(784):
        a0[i][0] = 0
    for i in range(16):
        a1[i][0] = 0
    for i in range(16):
        a2[i][0] = 0
    for i in range(10):
        a3[i][0] = 0
    ca.delete("all")
    res.delete("all")

def savePos(event):
    global lastx, lasty
    lastx, lasty = event.x, event.y

def addLine(event):
    ca.create_line((lastx, lasty, event.x, event.y), width = 10)
    if event.x >= 0 and event.x < 300 and event.y >= 0 and event.y < 300:
        rec(event.x, event.y)
    savePos(event)

def sigmoid(a):
    c = [[0] * 1 for i in range(len(a))]
    for i in range(len(a)):
        c[i][0] = (-1) * a[i][0]
    return (1 / (1 + np.exp(c)))

def add(a, b):
    m = len(b[0])
    c = [[0] * m for i in range(1)]
    d = [[0] * 1 for i in range(m)]
    for i in range(m):
        c[0][i] = b[0][i] + a[i][0]
        d[i][0] = c[0][i]
    return d

def calc():
    res.delete("all")
    a1 = sigmoid(add(np.matmul(w01, a0), b1))
    a2 = sigmoid(add(np.matmul(w12, a1), b2))
    a3 = sigmoid(add(np.matmul(w23, a2), b3))
    global ans1, ans2, prob1, prob2
    s = 0
    ans1 = ans2 = 0
    prob1 = prob2 = a3[0][0]
    for i in range(10):
        s = s + a3[i][0]
        if a3[i][0] > prob1:
            ans2 = ans1
            prob2 = prob1
            ans1 = i
            prob1 = a3[i][0]
    res.create_text(50, 10, text = ans1, fill = "black")
    res.create_text(150, 10, text = round(prob1 / s * 100, 2), fill = "black")
    res.create_text(50, 20, text = ans2, fill = "black")
    res.create_text(150, 20, text = round(prob2 / s * 100, 2), fill = "black")


window = tk.Tk()

window.columnconfigure(0, weight = 1)
window.rowconfigure(0, weight = 1)
window.configure(background = "light gray")
window.geometry("400x500")

ca = tk.Canvas(window, width = 300, height = 300)
ca.grid(column = 0, row = 0, sticky = (tk.N, tk.W, tk.E, tk.S))
res = tk.Canvas(window, width = 300, height = 30)

ca.bind("<Button-1>", savePos)
ca.bind("<B1-Motion>", addLine)

but1 = tk.Button(window, width = 20, height = 3, command = reset, text = "RESET")
but2 = tk.Button(window, width = 20, height = 3, command = calc, text = "CALCULATE")

ca.pack()
but1.pack()
but2.pack()
res.pack()

idx = 1
for line in fileinput.input(files = filename):
    line = line.rstrip(line[-1])
    if(idx <= 16):
        w01[idx - 1] = line.split(" ")
    elif(idx >= 18 and idx <= 33):
        w12[idx - 18] = line.split(" ")
    elif(idx >= 35 and idx <= 44):
        w23[idx - 35] = line.split(" ")
    elif(idx == 46):
        b1[0] = line.split(" ")
    elif(idx == 47):
        b2[0] = line.split(" ")
    elif(idx == 48):
        b3[0] = line.split(" ")
    idx = idx + 1

for i in range(784):
    for j in range(16):
        w01[j][i] = float(w01[j][i])

for i in range(16):
    for j in range(16):
        w12[j][i] = float(w12[j][i])

for i in range(16):
    for j in range(10):
        w23[j][i] = float(w23[j][i])

for i in range(16):
    b1[0][i] = float(b1[0][i])

for i in range(16):
    b2[0][i] = float(b2[0][i])
        
for i in range(10):
    b3[0][i] = float(b3[0][i])

window.mainloop()



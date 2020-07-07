import matplotlib.pyplot as plt
import numpy as np
import serial
import time

data_num = 100

t = np.arange(0, 10, 10/data_num) # time vector; create Fs samples between 0 and 1.0 sec.
X = np.arange(0, 10, 10/data_num)
Y = np.arange(0, 10, 10/data_num)
Z = np.arange(0, 10, 10/data_num)
is_blink = np.arange(0, 10, 10/data_num)

serdev = '/dev/ttyACM0'

s = serial.Serial(serdev, baudrate=115200)

i = 0
j = 0
k = 0
l = 0

for x in range(0, int(4*data_num)):
    line = s.readline() 

    # print line
    if x%4 == 0 :
        X[i] = float(line)
        i = i + 1
    elif x%4 == 1 :
        Y[j] = float(line)
        j = j + 1
    elif x%4 == 2 :
        Z[k] = float(line)
        k = k + 1
    else :
        is_blink[l] = float(line)
        l = l + 1

fig, ax = plt.subplots(2, 1)

ax[0].plot(t, X, 'b')
ax[0].plot(t, Y, 'r')
ax[0].plot(t, Z, 'g')


ax[0].set_xlabel('Time')
ax[0].set_ylabel('Acc Vector')

ax[1].stem(t, is_blink)

ax[1].set_xlabel('Time')
ax[1].set_ylabel('Tilt')

plt.show()

s.close()

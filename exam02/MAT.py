import matplotlib.pyplot as plt
import numpy as np
import serial
import time

data_num = 100

t = np.arange(0, 10, 10/data_num) # time vector
X = np.arange(0, 10, 10/data_num)
Y = np.arange(0, 10, 10/data_num)
Z = np.arange(0, 10, 10/data_num)
displacement = np.arange(0, 10, 10/data_num)
larger_than_5cm = np.arange(0, 10, 10/data_num)

serdev = '/dev/ttyACM0'

s = serial.Serial(serdev, baudrate=115200)

i = 0
j = 0
k = 0
l = 0
h = 0

for x in range(0, int(5*data_num)):
    line = s.readline() 

    # print line
    if x%5 == 0 :
        X[i] = float(line)
        i = i + 1
    elif x%5 == 1 :
        Y[j] = float(line)
        j = j + 1
    elif x%5 == 2 :
        Z[k] = float(line)
        k = k + 1
    elif x%5 == 3 :
        displacement[l] = float(line)
        l = l + 1
    else :
        larger_than_5cm[h] = float(line)
        h = h + 1

fig, ax = plt.subplots(3, 1)

ax[0].plot(t, X, 'b')
ax[0].plot(t, Y, 'r')
ax[0].plot(t, Z, 'g')


ax[0].set_xlabel('Time')
ax[0].set_ylabel('Acc Vector')

ax[1].plot(t, displacement)

ax[1].set_xlabel('Time')
ax[1].set_ylabel('Displacement')

ax[2].stem(t, larger_than_5cm)

ax[2].set_xlabel('Time')
ax[2].set_ylabel('Larger Than 5 CM')

plt.show()

s.close()

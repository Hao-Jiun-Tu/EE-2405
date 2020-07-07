import matplotlib.pyplot as plt
import numpy as np
import paho.mqtt.client as paho
import time
mqttc = paho.Client()

# Settings for connection
host = "localhost"
topic= "Mbed"
port = 1883

x = np.arange(0, 18, 1)
y = np.arange(0, 18, 1)
z = np.arange(0, 18, 1)
t = np.arange(0, 18, 1)
tilt = np.arange(0, 18, 1)

i = 0
j = 0
k = 0
l = 0
# Callbacks
def on_connect(self, mosq, obj, rc):
    print("Connected rc: " + str(rc))

def on_message(mosq, obj, msg):
    global a
    print("[Received] Topic: " + msg.topic + ", Message: " + str(msg.payload) + "\n")
    print(float(msg.payload))
    print(a)
    if a%4 == 0:
        x[i] = float(msg.payload)
        i += 1
    else if a%4 == 1:
        y[j] = float(msg.payload)
        j += 1
    else if a%4 == 2:
        z[k] = float(msg.payload)
        k += 1
    else if a%4 == 3:
        t[l] = float(msg.payload)
        l += 1

    a += 1

def on_subscribe(mosq, obj, mid, granted_qos):
    print("Subscribed OK")

def on_unsubscribe(mosq, obj, mid, granted_qos):
    print("Unsubscribed OK")

# Set callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe
mqttc.on_unsubscribe = on_unsubscribe

# Connect and subscribe
print("Connecting to " + host + "/" + topic)
mqttc.connect(host, port=1883, keepalive=60)
mqttc.subscribe(topic, 0)

while True:
    mqttc.loop()
    if a >= 18:
        fig, ax = plt.subplots(2, 1)
        ax[0].stem(t, tilt)
        ax[0].set_xlabel('Time')
        ax[0].set_ylabel('Tilt')
        ax[1].plot(t, x)
        ax[1].plot(t, y)
        ax[1].plot(t, z)
        ax[1].set_xlabel('Time')
        ax[1].set_ylabel('Acc Vector')
        plt.show()
        s.close()
        break
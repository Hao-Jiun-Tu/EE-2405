import serial
import paho.mqtt.client as paho
import time

mqttc = paho.Client()
# Settings for connection
host = "192.168.2.5" 
topic= "velocity"

# Callbacks
def on_connect(self, mosq, obj, rc):
    print("Connected rc: " + str(rc))

def on_message(mosq, obj, msg):
    print("[Received] Topic: " + msg.topic + ", Message: " + str(msg.payload) + "\n");

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


# XBee setting
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 115200)

s.write("+++".encode())
char = s.read(2)
print("Enter AT mode.")
print(char.decode())

s.write("ATMY 0x223\r\n".encode())
char = s.read(3)
print("Set MY 0x223.")
print(char.decode())

s.write("ATDL 0x123\r\n".encode())
char = s.read(3)
print("Set DL 0x123.")
print(char.decode())

s.write("ATWR\r\n".encode())
char = s.read(3)
print("Write config.")
print(char.decode())

s.write("ATMY\r\n".encode())
char = s.read(4)
print("MY :")
print(char.decode())

s.write("ATDL\r\n".encode())
char = s.read(4)
print("DL : ")
print(char.decode())

s.write("ATCN\r\n".encode())
char = s.read(3)
print("Exit AT mode.")
print(char.decode())


while 1:
    s.write("/getAcc/run\r".encode())
    mesg = s.readline()
    mqttc.publish(topic, mesg)
    print(mesg)
    time.sleep(0.1)

s.close()
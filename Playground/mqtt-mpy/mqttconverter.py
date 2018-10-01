from umqtt.simple import MQTTClient
import binascii
import time
import sys
MESSAGE_LENGTH = 15
MQTT_SERVER = 'test.mosquitto.org'
MQTT_PORT = 1883

try:
    c = MQTTClient('hytech_car', MQTT_SERVER, MQTT_PORT)
    c.connect()
    #c.subscribe('hytech_car/telemetry')
except OSError as e:
    c.disconnect()

def read_uart():
    incomingFrame = ''
    while True:
        data = sys.stdin.read(1)
        if len(data) > 0:
            incomingFrame += binascii.hexlify(data, sep=' ') + ' '
            if ('00' in incomingFrame):
                frame = incomingFrame[0:incomingFrame.find("00")]
                timestamp = str(time.localtime())
                send_mqtt(timestamp, frame)
                incomingFrame = incomingFrame[incomingFrame.find('00 ') + 3:]

def send_mqtt(timestamp, data):
    msg = timestamp + ',' + data
    print(msg)
    c.publish('hytech_car/telemetry', msg)

read_uart()
from umqtt.robust import MQTTClient
import binascii
import time
import sys
MESSAGE_LENGTH = 15
MQTT_SERVER = 'test.mosquitto.org'
MQTT_PORT = 1883
try:
    c = MQTTClient('hytech_car', MQTT_SERVER, MQTT_PORT)
    c.connect()
    print('connected to MQTT broker')
    c.publish('hytech_car/telemetry', 'Xbee Connected')
    #c.subscribe('hytech_car/telemetry')
except OSError as e:
    c.disconnect()
def read_uart():
    incomingFrame = b''
    while True:
        data = sys.stdin.buffer.read(-1)
        if data is not None:
            incomingFrame += data
            print(binascii.hexlify(incomingFrame))
            if (b'\x00' in incomingFrame):
                frame = incomingFrame[0:incomingFrame.find(b'\x00')]
                timestamp = str.encode(str(time.time()))        # epoch time
                print('sending message...', type(frame))
                print(timestamp, binascii.hexlify(frame))
                send_mqtt(timestamp, frame)
                incomingFrame = incomingFrame[incomingFrame.find(b'\x00') + 1:]
def send_mqtt(timestamp, data):
    msg = timestamp + b',' + data
    print('sent message')
    print(msg)
    c.publish('hytech_car/telemetry', msg)
read_uart()
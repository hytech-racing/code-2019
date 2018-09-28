from umqtt.simple import MQTTClient
import binascii
import time
MESSAGE_LENGTH = 15
MQTT_SERVER = 'mqtt://test.mosquitto.org'
MQTT_PORT = 1883

c = MQTTClient('hytech_car', MQTT_SERVER, MQTT_PORT)
c.connect()
c.subscribe('hytech_car/telemetry')

def read_uart():
    incomingFrame = ''
    while True:
        data = sys.stdin.read(1)
        if len(data) > 0:
            incomingFrame += binascii.hexlify(data, sep=' ') + ' '
            if ('00' in incomingFrame):
                frame = incomingFrame[0:incomingFrame.find("00")]
                msg = unpack(frame)
                if msg != -1:
                    size = ord(msg[4])
                    raw = binascii.hexlify(msg[0]).upper() + "," + binascii.hexlify(msg[5:5 + size]).upper()
                    timestamp = str(time.localtime())
                    send_mqtt(timestamp, raw)
                incomingFrame = incomingFrame[incomingFrame.find('00 ') + 3:]

def unpack(frame):
    frame = ''.join(char for char in frame if char.isalnum())
    if (len(frame) != 32):
        return -1
    try:
        decoded = cobs_decode(binascii.unhexlify(frame.strip("\n\r")))
    except Exception as e:
        #print("Decode failed: " + str(e))
        return -1
    # Calculate checksum
    checksum = fletcher16(decoded[0:13])
    cs_calc = binascii.hexlify(chr(checksum >> 8)).upper() + " " + binascii.hexlify(chr(checksum & 0xFF)).upper()
    cs_rcvd = binascii.hexlify(decoded[14]).upper() + " " + binascii.hexlify(decoded[13]).upper()
    if cs_calc != cs_rcvd:
        return -1
    return decoded

def cobs_decode(in_bytes):
    """Decode a string using Consistent Overhead Byte Stuffing (COBS).

    Input should be a byte string that has been COBS encoded. Output
    is also a byte string.

    A cobs.DecodeError exception will be raised if the encoded data
    is invalid."""
    out_bytes = []
    idx = 0

    if len(in_bytes) > 0:
        while True:
            length = ord(in_bytes[idx])
            if length == 0:
                raise DecodeError("zero byte found in input")
            idx += 1
            end = idx + length - 1
            copy_bytes = in_bytes[idx:end]
            if '\x00' in copy_bytes:
                raise DecodeError("zero byte found in input")
            out_bytes.append(copy_bytes)
            idx = end
            if idx > len(in_bytes):
                raise DecodeError("not enough input bytes for length code")
            if idx < len(in_bytes):
                if length < 0xFF:
                    out_bytes.append('\x00')
            else:
                break
    return ''.join(out_bytes)

def fletcher16(data):
    d = map(ord,data)
    index = 0
    c0 = 0
    c1 = 0
    i = 0
    length = len(d)
    while length >= 5802:
        for i in range(5802):
            c0 += d[index]
            c1 += c0
            index += 1
        c0 %= 255
        c1 %= 255
        length -= 5802
    
    index = 0
    for i in range(len(data)):
        c0 += d[index]
        c1 += c0
        index += 1
    c0 %= 255
    c1 %= 255
    return (c1 << 8 | c0)

def send_mqtt(timestamp, data):
    msg = timestamp + ',' + data
    c.publish('hytech_car/telemetry', msg)

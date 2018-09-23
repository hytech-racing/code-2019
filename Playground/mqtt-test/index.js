const mqtt = require("mqtt")
const xbt = require("./xbtools")
const client = mqtt.connect("mqtts://iot.eclipse.org:8883")
const MESSAGE_LENGTH = 15

// let connected = false

// client.on("connect", () => {
//     client.subscribe("hytech/connected")
//     console.log("Subscribed to hytech/connected")
// })

// client.on("message", (topic, message) => {
//     if (topic === "hytech/connected") {
//         connected = (message.toString() === "true")
//         console.log("received connected true")
//         client.subscribe("hytech/message")
//     }
//     else if (topic === "hytech/message") {
//         console.log("Received message %s", message)
//     }
// })

let connected = false

client.on("connect", () => {
    client.subscribe("hytech/car")
    console.log("Connected and subscribed to hytech/car")
})

client.on("message", (topic, message) => {
    if (topic === "hytech/car") {
        // COBS and checksum decode
        let cobsBuf = []
        let id = 0
        let message = []
        let decoded = xbt.cobsDecode(message, MESSAGE_LENGTH + 2, uncobsed)
        if (decoded > 0) {
            // COBS decoded some data, now check the checksum
            let checksum = cobsBuf[MESSAGE_LENGTH - 1] << 8 | cobsBuf[MESSAGE_LENGTH - 2]
            let rawMsg = decoded.slice(0, MESSAGE_LENGTH - 2)
            
            let calcChecksum = xbt.fletcher16(rawMsg, MESSAGE_LENGTH - 2)
            if (calcChecksum === checksum) {
                // checksum matched, so we can do stuff with the data
                // figure out how to do the memcpy stuff from xbee_rcv.cpp
                console.log('Checksum matched')
            }
        }
    }
})
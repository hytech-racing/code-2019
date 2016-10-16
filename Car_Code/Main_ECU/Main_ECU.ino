#include <FlexCAN.h> // import teensy library

FlexCAN CAN(500000);
static CAN_message_t msg;
int OKHS = 0; // voltage after calculation
int DISCHARGE_OK = 0; // voltage after calculation
boolean startPressed = false; //
int timer = 0; // needed to check timer
boolean startupDone = false; // true when reached drive state

enum State { GLVinit=0, waitIMD, waitBMS, waitStartButton, closeLatch, openLatch, AIROpen, AIRClose, waitInverter, readySoundOn, drive };
State curState = GLVinit; // curState is current state
// setup code
void setup() {
    Serial.begin(115200); // init serial for PC communication

    CAN.begin(); // init CAN system
    Serial.println("CAN system and serial communication initialized");
}

// loop code
void loop() {
    if (!startupDone) {
        switch (state) {
            case GLVinit:
            case waitIMD:
            case waitBMS:
            case waitStartButton:
            case closeLatch:
            case openLatch:
            case AIROpen:
            case AIRClose:
            case waitInverter:
            case readySoundOn:
            case drive:

        }
    } else {

    }
}
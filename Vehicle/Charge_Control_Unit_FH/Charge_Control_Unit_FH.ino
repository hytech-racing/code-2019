/*
 * HyTech 2018 Formula Hybrid Vehicle Charge Control Unit
 */

#include <FlexCAN.h>
#include "HyTech_CAN.h"
#include <Metro.h>

#define CHARGE_ENABLE 10

/*
 * Global variables
 */
CCU_status ccu_status;

static CAN_message_t msg;
Metro timer_update_CAN = Metro(265);

void setup() {
    pinMode(CHARGE_ENABLE, OUTPUT);

    Serial.begin(115200); // Init serial for PC communication
    Can0.begin(500000); // Init CAN for vehicle communication
    delay(100);
    Serial.println("CAN system and serial communication initialized");

    ccu_status.set_charger_enabled(false);
}

void loop() {
    /*
     * Handle incoming CAN messages
     */
    while (Can0.read(msg)) {
        if (msg.id == ID_BMS_STATUS) {
            BMS_status bms_status = BMS_status(msg.buf);
            Serial.print("BMS state: ");
            Serial.println(bms_status.get_state());
            ccu_status.set_charger_enabled(bms_status.get_state() == BMS_STATE_CHARGING);
            digitalWrite(CHARGE_ENABLE, ccu_status.get_charger_enabled());
        }
    }
    
    /*
     * Send status over CAN
     */
    if (timer_update_CAN.check()) {
        ccu_status.write(msg.buf);
        msg.id = ID_CCU_STATUS;
        msg.len = sizeof(CAN_message_ccu_status_t);
        Can0.write(msg);

        Serial.print("Charge enable: ");
        Serial.println(ccu_status.get_charger_enabled());
    }
}
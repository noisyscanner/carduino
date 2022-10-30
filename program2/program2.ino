#include <SPI.h>
#include "df_can.h"

///////////////////
// Configuration //
///////////////////

// CS pin for CAN bus shield.
// The default pin depends on the shield's version:
//  - 1.0: digital pin 10
//  - 1.1 and newer: digital pin 9
const int CS_PIN = 10;

// Serial port data rate
const long SERIAL_SPEED = 500000;

// CAN bus data rate
const byte CAN_SPEED = CAN_125KBPS;

///////////////////

MCPCAN CAN(CS_PIN);
unsigned char len = 0;
byte buffer[8];
INT32U canID;

void setup() {
    Serial.begin(SERIAL_SPEED);
    Serial.println("Serial init");
    byte canSpeed = CAN_SPEED;

    int count = 50;
  do {
    CAN.init();
    if (CAN_OK == CAN.begin(CAN_125KBPS)) {
      break;
    } else {
      delay(100);
//      if (count <= 1)
//        Serial.println("It's not happening");
    }

  } while (count--);
}

void loop() {
    if (CAN.checkReceive() == CAN_MSGAVAIL) {
//        if (canID != 0x2D5) return;
        CAN.readMsgBuf(&len, buffer);
        canID = CAN.getCanId();

        Serial.print("FRAME:ID=");
        Serial.print(canID);
        Serial.print(":LEN=");
        Serial.print(len);

        char tmp[3];
        for(int i = 0; i<len; i++) {
            Serial.print(":");

            snprintf(tmp, 3, "%02X", buffer[i]);

            Serial.print(tmp);
        }

        Serial.println();
    }
}

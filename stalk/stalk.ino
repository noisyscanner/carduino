#include <SPI.h>
#include "df_can.h"

// To monitor:
// screen /dev/cu.usbserial-1420 230400
// canmonitor /dev/cu.usbserial-1420 230400
// screen -L to log to file

const int SPI_CS_PIN = 10; // Uno
//const int SPI_CS_PIN = 53; // Mega 2560
MCPCAN CAN(SPI_CS_PIN);

unsigned char len = 0;
byte buffer[8];

void setup() {
  Serial.begin(230400);
  Serial.println("Serial init");

  setupBt();

  int count = 50;
  do {
    CAN.init();
    if (CAN_OK == CAN.begin(CAN_125KBPS)) {
      Serial.println("CAN connected");
      break;
    } else {
      delay(100);
    }

  } while (count--);
}

void loop() {
  if (CAN.checkReceive() != CAN_MSGAVAIL) {
    return;
  }

  CAN.readMsgBuf(&len, buffer);
  int canID = CAN.getCanId();

//    if (canID == 0x2D5 || canID == 486) {
//  log (canID, len, buffer);
//    }

  parse(canID, buffer);
}

#include <SPI.h>
#include "df_can.h"

// To monitor:
// screen /dev/cu.usbserial-1420 57600

const int SPI_CS_PIN = 10;
MCPCAN CAN(SPI_CS_PIN);

void setup() {
  Serial.begin(38400);
  int count = 50;
  do {
    CAN.init();
    if (CAN_OK == CAN.begin(CAN_500KBPS)) {
      Serial.println("Connected");
      break;
    } else {
      Serial.println("Failed to connect");

      delay(100);
      if (count <= 1)
        Serial.println("It's not happening");
    }

  } while (count--);
}


void loop() {
  unsigned char len = 0;
  unsigned char buf[8];
  char str[2];

  if (CAN_MSGAVAIL == CAN.checkReceive()) {
    sprintf(str, "%x", CAN.getCanId());
    Serial.write(str);
    Serial.print("\t");
    
    CAN.readMsgBuf(&len, buf);

    for (int i = 0; i < len; i++) {
      sprintf(str, "%x", buf[i]);
      Serial.write(str);
//      Serial.write(buf[i]);
      Serial.print("\t");
    }
    Serial.println();
  }
}

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

const char HI = 0;
const char LO = 1;

const INT32U IGNITION_ID = 0x285;

const INT32U MUTE_ID = 0x2D9;
const short MUTE_MUTE_IDX = 0;
const char MUTE = 1;
const char NOT_MUTE = 4;

const INT32U STALK_ID = 0x2D5;
const short STALK_SEEK_IDX = 1; // next/prev toggle
const short STALK_MODE_IDX = 2; // mode button
const unsigned char SEEK_PREV = 0x8;
const unsigned char SEEK_NEXT = 0x4;
const unsigned char MODE = 0x1;

byte lastOnState = -1;

boolean isMuted = false;
char lastIgnitionPos = 0xA;

unsigned long lastNew = millis();

char getNibble(byte* buf, const short idx, const char nibble) {
  byte b = buf[idx];
  if (nibble == HI) {
    return HI_NIBBLE(b);
  }
  return LO_NIBBLE(b);
}

void parse(INT32U canId, byte* buf) {
  switch (canId) {
    case STALK_ID:
      return processStalk(buf);
    case 0x2DC:
      byte val = buf[0];
      // if there is a change, wait a bit and parse the next one
      // to make sure the state is the same in 1s
      // to avoid restart when starting engine
      if (lastOnState != val) {
        Serial.println("New state");
        Serial.println(val);
        lastOnState = val;
        lastNew = millis();
        return;
      }
      if (millis() < lastNew + 2500) {
        Serial.println("Same value, wait");
        return;
      }
      Serial.println("Waiting done, act");
      lastNew = millis();
//      Serial.println(isOn);
      // This is 0 when radio is off, or other values when on
      if ((val == 0 && isOn) || (val != 0 && !isOn)) {
        Serial.println("Turn");
        turnOnOff();
      }
//    case MUTE_ID:
//      return processMute(buf);
//    case IGNITION_ID:
//      return processIgnition(buf);
  }
}

void processStalk(byte* buf) {
  // TODO: Pattern matching?
  char seekNibble = getNibble(buf, STALK_SEEK_IDX, HI);
  char modeNibble = getNibble(buf, STALK_MODE_IDX, HI);

  if (seekNibble == SEEK_NEXT) {
    next();
    Serial.println("Next track");
  } else if (seekNibble == SEEK_PREV) {
    prev();
    Serial.println("Prev track");
  } else if (modeNibble == MODE) {
    Serial.println("Play/pause");
  }
}

void processMute(byte* buf) {
  char b = getNibble(buf, MUTE_MUTE_IDX, LO);

  if (b == MUTE) {
//    Serial.println("Mute");
    if (!isMuted) {
      turnOnOff();
    }
    isMuted = true;
  } else if (b == NOT_MUTE) {
//    Serial.println("Not mute");
    isMuted = false;
  }
}
//
//void processIgnition(byte* buf) {
//  char ignitionPos = getNibble(buf, 0, HI);
//
//  // Play/pause if switching on or off ignition
//  if (ignitionPos == 0 && lastIgnitionPos != 0) {
//    Serial.println("Turn off");
//    hold(ON, 4000);
//    isOn = false;
//        
//    lastIgnitionPos = ignitionPos;
////    turnOnOff();
//  } else if (ignitionPos > 0 && lastIgnitionPos == 0) {
//    turnOnOff();
//    isOn = true;
//    lastIgnitionPos = ignitionPos;
//  }
//}

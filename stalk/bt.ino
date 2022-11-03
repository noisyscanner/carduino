const byte ON = 2;
const byte NEXT = 4;
const byte PREV = 5;

boolean isOn = false;

void setupBt() {
  pinMode(ON, OUTPUT);
  pinMode(NEXT, OUTPUT);
  pinMode(PREV, OUTPUT);

 // Will turn on as this is startup
  turnOnOff();
}

void press(byte pin) {
  hold(pin, 500);
}

void longPress(byte pin) {
  if (isOn) {
    Serial.println("Turn off");
    hold(pin, 3200);
  } else {
    Serial.println("Turn on");
    hold(pin, 1500);
  }  
}

void hold(byte pin, int forMs) {
  digitalWrite(pin, HIGH);
  delay(forMs);
  digitalWrite(pin, LOW);
}

void next() {
  longPress(NEXT);
}


void prev() {
  longPress(PREV);
}

void playPause() {
  press(ON);
}

void turnOnOff() {
  Serial.println("turnOnOff");
  longPress(ON);
  isOn = !isOn;
}

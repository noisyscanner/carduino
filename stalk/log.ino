void log (int canId, unsigned char len, byte* buf) {
  Serial.print("FRAME:ID=");
  Serial.print(canId);
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

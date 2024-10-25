#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9); // RX, TX
void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
  delay(1000);
  Serial.println("Goodnight  moon2");
  mySerial.println("AT+GMR");
}
void loop() {
  while (mySerial.available()) {
    mySerial.write(Serial.read());
    Serial.println("Goodnight  moon1");
  }
}

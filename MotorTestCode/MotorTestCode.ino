#include "AM2861.h"

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 8191 from 2 ^ 13 - 1
  uint32_t duty = (8191 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

AM2861 am2861(0, 12, 1, 13, 5000, 12);

void setup() {
  // Setup timer and attach timer to a led pin
  //ledcSetup(0, 5000, 13);
  //ledcAttachPin(12, 0);
  //ledcSetup(1, 5000, 13);
  //ledcAttachPin(13, 1);
}

void loop() {
  //stop
  //ledcAnalogWrite(0, 0, 100);
  //ledcAnalogWrite(1, 0, 100);
  am2861.control(false, 0);
  delay(1000);

  //cw
  //ledcAnalogWrite(0, 100, 100);
  //ledcAnalogWrite(1, 30, 100);
  am2861.control(false, 63);
  delay(5000);

  //stop
  //ledcAnalogWrite(0, 0, 100);
  //ledcAnalogWrite(1, 0, 100);
  am2861.control(false, 0);
  delay(1000);

  //ccw
  //ledcAnalogWrite(0, 30, 100);
  //ledcAnalogWrite(1, 100, 100);
  am2861.control(true, 63);
  delay(5000);
}

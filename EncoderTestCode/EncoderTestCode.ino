#include <Arduino.h>
#include <ESP32Encoder.h>

static IRAM_ATTR void enc_cb(void* arg) {
  ESP32Encoder* enc = (ESP32Encoder*) arg;
  //Serial.println("enc cb: count: %d\n", enc->getCount());
  static bool leds = false;
  digitalWrite(2, (int)leds);
  leds = !leds;
}

ESP32Encoder encoder(true, enc_cb);

void setup(){
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  // Encoder A and B pins connected with 1K series resistor to pins 4 and 5, common pin to ground.
  //         |- A   --- 1K --- pin 4
  //  >=[enc]|- GND
  //         |- B   --- 1K --- pin 5

  ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder.attachSingleEdge(4, 5);
  encoder.clearCount();
  encoder.setFilter(1023);

  Serial.println("display init failed");
}

void loop(){
}

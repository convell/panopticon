#include "client-mqtt.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  setupCloudIoT();
}
 
unsigned long lastMillis = 0;
void loop() {
  mqtt->loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!mqttClient->connected()) {
    connect();
  }
}

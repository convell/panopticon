#include <WiFiNINA.h> // Using the IoT board so use WifiNINA
#include <WiFiSSLClient.h>

#include <Arduino_JSON.h> // To deserialize from mqtt
#include <MQTT.h> // Means to communicate from

#include <CloudIoTCore.h>
#include <CloudIoTCoreMqtt.h>
#include "ciotc_config.h" // Contains cloud config and wifi settings

void messageReceived(String &topic, String &payload) {

  JSONVar json_obj = JSON.parse(payload);
  int pin;
  int output;

  // {"pin":12, "high":1}
  if (json_obj.hasOwnProperty("pin")) {
    Serial.println("Pin json found");
    pin = json_obj["pin"];
    Serial.println(pin);
  };
  if (json_obj.hasOwnProperty("high")) {
    output = json_obj["high"];
  };


  pinMode(pin, OUTPUT);
  digitalWrite(pin, output);

}

Client *netClient;
CloudIoTCoreDevice *device;
CloudIoTCoreMqtt *mqtt;
MQTTClient *mqttClient;
unsigned long iat = 0;
String jwt;

String getJwt() {
  // Disable software watchdog as these operations can take a while.
  Serial.println("Refreshing JWT");
  iat = WiFi.getTime();
  jwt = device->createJWT(iat, jwt_exp_secs);
  return jwt;
}

void setupWifi() {
  Serial.println("Starting wifi");

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  Serial.println("Waiting on time sync...");
  while (WiFi.getTime() < 1510644967) {
    delay(10);
  }
}

void connectWifi() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
}

void connect() {
  connectWifi();
  mqtt->mqttConnect();
}

void setupCloudIoT() {
  // Connect to google cloud and setup mqtt client
  device = new CloudIoTCoreDevice(
    project_id, location, registry_id, device_id,
    private_key_str);

  setupWifi();
  netClient = new WiFiSSLClient();

  mqttClient = new MQTTClient(512);
  mqttClient->setOptions(180, true, 1000);
  mqtt = new CloudIoTCoreMqtt(mqttClient, netClient, device);
  mqtt->startMQTT();
}

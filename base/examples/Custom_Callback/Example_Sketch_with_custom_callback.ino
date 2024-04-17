//libs
#ifdef __AVR__
#include <avr/power.h> 
#endif

//self
#include "MQTTManager.h"

char *ssid = ""; 
char *password = "";  
char *mqtt_broker = "";
char *topic = "";
char *mqtt_username = "";
char *mqtt_password = "";
char *name = ""; //name vom gerät
int mqtt_port = 1883;
int debug = true;

MQTTManager mqttManager(ssid, password, mqtt_broker, mqtt_username, mqtt_password, mqtt_port, topic, name, debug);

void customCallback(char* topic, byte* payload, unsigned int length) {
  processMessage(topic, (char*)payload); 
}

void processMessage(const char* topic, const char* payload) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.println("Failed to deserialize JSON");
    return;
  }

  const char* command = doc["command"];
  if (!command) {
    Serial.println("Command not found in JSON payload");
    return;
  }

  if (strcmp(command, "toggle_mode") == 0) {
    // Beispiel: effekt toggeln
    Serial.println("Toggle light mode command received");
  }
  //genauso für weitere commands
}

void setup() {
  Serial.begin(115200);

  mqttManager.connect();
  mqttManager.setCallback(customCallback);
}

void loop() {
  mqttManager.loop();
  mqttManager.publish("test", "{\"command\":\"toggle_mode\"}"); //durch den custom callback, können commands dieser art verarbeitet werden
  delay(500);
}
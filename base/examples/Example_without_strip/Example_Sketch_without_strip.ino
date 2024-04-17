//libs
#ifdef __AVR__
#include <avr/power.h> 
#endif

//self
#include "MQTTManager.h" //on github: https://github.com/ShadowRock345/MQTTManager (installation tutorial)

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

void setup() {
  Serial.begin(115200);

  mqttManager.connect();
}

void loop() {
  mqttManager.loop();
  mqttManager.publish("test", "Hello World");
  delay(500);
}

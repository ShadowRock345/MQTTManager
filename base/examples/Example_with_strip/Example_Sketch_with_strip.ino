//libs
#include <Adafruit_NeoPixel.h>
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

#define LED_PIN    6
#define LED_COUNT 60

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  mqttManager.connect();

  strip.begin();
  strip.show();
  strip.setBrightness(100);
}

void loop() {
  mqttManager.loop();
  //mqttManager.publish("test", "Hello World");
  //delay(500);

  static unsigned long lastUpdate = 0;
  unsigned long now = millis();

  //lichteffekte alle 100ms aktualisieren

  if (now - lastUpdate >= 100) {

    colorWipe(strip.Color(255,   0,   0), 50);

    lastUpdate = now;
  }
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { 
    strip.setPixelColor(i, color);         
    strip.show();                          
    delay(wait);                           
  }
}



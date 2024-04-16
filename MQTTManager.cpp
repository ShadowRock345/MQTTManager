/*
 * MQTTManager.cpp - Bibliothek zur Verwaltung von MQTT-Verbindungen
 * Erstellt von [P_Seminar], [16.04.2024]
 * Copyright (c) [2024] [P_Seminar]
 * Freigegeben unter der MIT-Lizenz. All rights reserved.
 */

#include "MQTTManager.h"

MQTTManager::MQTTManager(char* ssid, char* password, char* mqtt_broker, char* mqtt_username, char* mqtt_password, int mqtt_port, char* topic, char* name, int& lightmode, bool debug, int qos, bool retain)
  : ssid(ssid), password(password), mqtt_broker(mqtt_broker), mqtt_username(mqtt_username), mqtt_password(mqtt_password), mqtt_port(mqtt_port), topic(topic), name(name), client(espClient), lightmode(lightmode), debug_mode(debug), qos_level(qos), retain_flag(retain), update_available(false) {
    current_version = "1.0.0";
    
  }

MQTTManager::~MQTTManager() {
  client.disconnect();
  espClient.stop();
}

void MQTTManager::connect() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (debug_mode) { Serial.println("Connecting to WiFi.."); }
  }
  if (debug_mode) { Serial.println("Connected to the Wi-Fi network"); }
  checkForUpdate();
  
  // mit broker verbinden
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback([this](char* topic, byte* payload, unsigned int length) {
    this->callback(topic, payload, length);
  });
  reconnect();
}

void MQTTManager::checkForUpdate() {
  HTTPClient http;
  http.begin("https://api.jsonbin.io/v3/b/661eb7beacd3cb34a83980cc"); 

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    StaticJsonDocument<200> doc;
    deserializeJson(doc, response);
    latest_version = doc["record"]["version"].as<String>();

    
    if (latest_version != current_version) {
      update_available = true;
      debugPrint("Update available! Please update the library.");
    } else {
      update_available = false;

      debugPrint("Library is up to date.");
    }
  } else {
    debugPrint("Failed to check for updates.");
  }

  http.end();
}

void MQTTManager::publish(const char* topic, const char* payload) {
  if (!client.connected()) {
    if (debug_mode) { Serial.println("Error: MQTT client is not connected! Reconnecting..."); }
    reconnect();
  }

  if (!client.publish(topic, payload)) {
    if (debug_mode) { Serial.println("Error: Failed to publish message"); }
    return;
  }

  Serial.println("Message published successfully");
}

void MQTTManager::loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void MQTTManager::callback(char* topic, byte* payload, unsigned int length) {
  if (debug_mode) { Serial.print("Message arrived in topic: "); }
  if (debug_mode) { Serial.println(topic); }
  if (debug_mode) { Serial.print("Message: "); }
  for (int i = 0; i < length; i++) {
    if (debug_mode) { Serial.print((char) payload[i]); }
    
  }
  if (debug_mode) { Serial.println(); }
  if (debug_mode) { Serial.println("-----------------------"); }

  //processMessage(topic, (char*)payload);
}

void MQTTManager::processMessage(const char* topic, const char* payload) {
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* command = doc["command"];

  if (strcmp(command, "toggle_mode") == 0) {
    // Beispiel: effekt toggeln
    if (debug_mode) { Serial.println("Toggle light mode command received"); }
    if (lightmode >= 10) {
      lightmode = 0;
    } else {
      lightmode += 1;
    }
  }
  //genauso für weitere commands
}

void MQTTManager::reconnect() {
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    if (debug_mode) { Serial.printf("The client %s connects to the MQTT broker\n", client_id.c_str()); }
    
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password, NULL, qos_level, retain_flag, NULL, true)) {
      if (debug_mode) { Serial.println("MQTT broker connected"); }
      String message = "Client [";
      message += name;
      message += "] connected!"; 
      client.publish(topic, message.c_str());
      client.subscribe(topic);
    } else {
      handleErrors(client.state());
      delay(2000);
    }
  }
}

void MQTTManager::handleErrors(int error) {
  //fehler auswerten
  if (debug_mode) {
    switch (error) {
      case -1:
        Serial.println("Connection failed: Timeout");
        break;
      case -2:
        Serial.println("Connection failed: Wrong server");
        break;
      case -3:
        Serial.println("Connection failed: MQTT unavailable");
        break;
      case -4:
        Serial.println("Connection failed: Bad credentials");
        break;
      case -5:
        Serial.println("Connection failed: Not connected");
        break;
      default:
        Serial.print("Connection failed with error code ");
        Serial.println(error);
        break;
    }
  }
}




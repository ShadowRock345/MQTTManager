/*
 * MQTTManager.h - Bibliothek zur Verwaltung von MQTT-Verbindungen
 * Erstellt von [Ihr Name], [16.04.2024]
 * Copyright (c) [2024] [Ihr Name]
 * Freigegeben unter der MIT-Lizenz. All rights reserved.
 */

#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>


class MQTTManager {
  private:
    char* ssid;
    char* password;
    char* mqtt_broker;
    char* mqtt_username;
    char* mqtt_password;
    int mqtt_port;
    char* topic;
    char* name;
    WiFiClient espClient;
    PubSubClient client;
    bool debug_mode;
    int qos_level;
    bool retain_flag;
    String current_version; 
    String latest_version; 
    bool update_available;

    int& lightmode; //beispielhaft

  public:
    MQTTManager(char* ssid, char* password, char* mqtt_broker, char* mqtt_username, char* mqtt_password, int mqtt_port, char* topic, char* name, int& lightmode, bool debug = true, int qos = 0, bool retain = false);
    MQTTManager(const MQTTManager&) = delete; //kopieren unterbinden
    MQTTManager& operator=(const MQTTManager&) = delete; //zuweisen unterbinden
    MQTTManager(MQTTManager&&) = delete; //verschieben unterbinden
    MQTTManager& operator=(MQTTManager&&) = delete; //verschiebezuweisung unterbinden
    ~MQTTManager(); //destruktor

    void setCredentials(char* new_username, char* new_password) {
      mqtt_username = new_username;
      mqtt_password = new_password;
    }

    void setBroker(char* new_broker, int new_port) {
      mqtt_broker = new_broker;
      mqtt_port = new_port;
    }

    void setWiFi(char* new_ssid, char* new_password) {
      ssid = new_ssid;
      password = new_password;
    }

    void setDebugMode(bool mode) {
      debug_mode = mode;
    }

    void setRetain(bool retain) {
      retain_flag = retain;
    }

    void setQoS(int qos) {
      qos_level = qos;
    }

    String getLatestVersion() {
      return latest_version;
    }

    bool isUpdateAvailable() {
      return update_available;
    }

    void setCallback(void (*callback)(char*, byte*, unsigned int)) {
        client.setCallback(callback);
    }

    void connect();
    void publish(const char* topic, const char* payload);
    void loop();

    void checkForUpdate();

  private:
    void callback(char* topic, byte* payload, unsigned int length);
    void processMessage(const char* topic, const char* payload);
    void reconnect();
    void handleErrors(int error);

    void debugPrint(const char* message) {
      if (debug_mode) {
        Serial.println(message);
      }
    }
};

#endif
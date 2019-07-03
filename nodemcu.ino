#include <SimpleTimer.h>
#include <Adafruit_seesaw.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/// WiFi Network credentials
const char* WIFI_SSID = "______";
const char* WIFI_PASSWORD = "______";

// MQTT Info: Obtained from cloudMQTT
const char *MQTT_SERVER = "______";
const int MQTT_PORT = 0;
const char *MQTT_USER = "______";
const char *MQTT_PASSWORD = "______";

// MQTT Topics
const char *PUB_TOPIC = "moisture";
const char *SUB_TOPIC = "pump";

// Declarations
int timerId;
SimpleTimer timer;
Adafruit_seesaw ss;
WiFiClient espClient;
PubSubClient client(MQTT_SERVER, MQTT_PORT, espClient);

int MOISTURE_POLL_MS = 5000;
int PUMP_PIN = 10;

void setup() {
  Serial.begin(115200);
  timerId = timer.setInterval(MOISTURE_POLL_MS, readMoisture);
  ss.begin(0x36);
  pinMode(PUMP_PIN, OUTPUT);
}

void loop() {
  connectWifi();
  connectPubSub();
  if (WiFi.status() == WL_CONNECTED && client.connected()) {
    client.loop();
    timer.run();
  }
}

void connectWifi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to wifi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      return;
    }
    Serial.println("WiFi connected");
  }
}

void connectPubSub() {
  if (!client.connected()) {
    Serial.println("Connecting to MQTT server...");
      
    if (client.connect("iot_garden_demo", MQTT_USER, MQTT_PASSWORD)) {
       Serial.println("Connected to MQTT server");       
       client.setCallback(callback);
       client.subscribe(SUB_TOPIC);   
    } else {
        Serial.println("Could not connect to MQTT server");   
    }
  }
}

void callback(char* topic, byte* payload, unsigned int len) {  
    char value[5] = "";
    for (int i = 0; i < len; i++) {
      value[i] = (char)payload[i]; 
    }
    int delayMs = atoi(value);
    triggerPump(delayMs);
}

void readMoisture() {
  uint16_t capread = ss.touchRead(0);
  char msg[4] = "";
  sprintf(msg, "%d", capread);
  client.publish(PUB_TOPIC, msg); 
}

void triggerPump(int delayMs) {
  digitalWrite(PUMP_PIN, HIGH);
  delay(delayMs);
  digitalWrite(PUMP_PIN, LOW);
}

#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "DHT.h"


#define PIR_PIN 12        
#define DHT_PIN 13        
#define DHT_TYPE DHT11    

DHT dht(DHT_PIN, DHT_TYPE);

#define AWS_IOT_PUBLISH_TOPIC "/telemetry"
#define AWS_IOT_SUBSCRIBE_TOPIC "/downlink"

long sendInterval = 10000;  

String THINGNAME = "";

WiFiClientSecure net;
PubSubClient client(net);

void connectAWS() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // get the MAC address
  THINGNAME = WiFi.macAddress();
  for (int i = 0; i < THINGNAME.length(); i++) {
    if (THINGNAME.charAt(i) == ':') {
      THINGNAME.remove(i, 1);
      i--;
    }
  }

  Serial.println();
  Serial.print("MAC Address: ");
  Serial.println(THINGNAME);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.setServer(AWS_IOT_ENDPOINT, 8883);

  Serial.print("Connecting to AWS IoT");

  while (!client.connected()) {
    if (client.connect(THINGNAME.c_str())) {
      Serial.println("Connected to AWS IoT");
    } else {
      Serial.print(".");
      delay(100);
    }
  }

  client.subscribe((THINGNAME + AWS_IOT_SUBSCRIBE_TOPIC).c_str());
  Serial.println("AWS IoT Connected!");
}

void setupShadow() {
  client.subscribe(("$aws/things/" + THINGNAME + "/shadow/get/accepted").c_str());
  client.subscribe(("$aws/things/" + THINGNAME + "/shadow/get/rejected").c_str());
  client.subscribe(("$aws/things/" + THINGNAME + "/shadow/update/delta").c_str());

  client.publish(("$aws/things/" + THINGNAME + "/shadow/get").c_str(), "");
}

bool publishTelemetry(String payload) {
  Serial.print("Publishing: ");
  Serial.println(payload);
  return client.publish((THINGNAME + AWS_IOT_PUBLISH_TOPIC).c_str(), payload.c_str());
}

void messageHandler(char* topic, byte* payload, unsigned int length) {
  String topicStr = topic;
  String payloadStr = "";

  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }

  Serial.println("incoming: " + topicStr + " - " + payloadStr);

  StaticJsonDocument<512> doc;
  deserializeJson(doc, payloadStr);

  if (topicStr.endsWith("/shadow/get/accepted")) {
    updateSettings(doc["state"]["desired"]);
  } else if (topicStr.endsWith("/shadow/update/delta")) {
    updateSettings(doc["state"]);
  }
}

void updateSettings(JsonDocument settingsObj) {
  sendInterval = settingsObj["sendIntervalSeconds"].as<int>() * 1000;

  StaticJsonDocument<512> docResponse;
  docResponse["state"]["reported"] = settingsObj;
  String jsonBuffer;
  serializeJson(docResponse, jsonBuffer);

  Serial.print("Sending reported state to AWS: ");
  Serial.println(jsonBuffer);

  client.publish(("$aws/things/" + THINGNAME + "/shadow/update").c_str(), jsonBuffer.c_str());
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(PIR_PIN, INPUT); 
  dht.begin(); 

  client.setCallback(messageHandler);
  connectAWS();
  setupShadow();
}

void loop() {
  static unsigned long previousMillis = -sendInterval;
  static int lastMotionState = LOW;

  client.loop();

  int motionDetected = digitalRead(PIR_PIN);
  if (motionDetected == HIGH && lastMotionState == LOW) {
    Serial.println("Motion detected!");

    StaticJsonDocument<128> motionDoc;
    motionDoc["motionDetected"] = true;

    String motionBuffer;
    serializeJson(motionDoc, motionBuffer);

    bool motionSendResult = publishTelemetry(motionBuffer);
    if (!motionSendResult) ESP.restart();

    lastMotionState = HIGH;
  } else if (motionDetected == LOW && lastMotionState == HIGH) {
    lastMotionState = LOW;
  }

  if (millis() - previousMillis >= sendInterval) {
    previousMillis = millis();

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    StaticJsonDocument<256> doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;

    String jsonBuffer;
    serializeJson(doc, jsonBuffer);

    bool sendResult = publishTelemetry(jsonBuffer);
    if (!sendResult) ESP.restart();
  }
}

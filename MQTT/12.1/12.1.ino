
#if defined(ESP32)
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <PubSubClient.h>
#include <ArduinoJson.h>

// ===== CAU HINH =====
const char* ssid = "H2SO4+2NH3=Pass";
const char* password = "DuyenLinh";

const char* mqtt_server = "192.168.1.10";
const int mqtt_port = 1883;

const char* mqtt_user = "mqtt-user";
const char* mqtt_pass = "thao256600";

const char* student_id = "10124295";
const char* mqtt_topic = "iot/lab1/10124295/sensor";

WiFiClient espClient;
PubSubClient client(espClient);

// ===== KET NOI WIFI =====
void setup_wifi() {
  Serial.print("Connecting WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// ===== KET NOI MQTT =====
void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP-Lab1-";
    clientId += student_id;

    Serial.print("Connecting MQTT...");

    if (client.connect(clientId.c_str(),
                       mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

// ===== GUI DU LIEU =====
void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  StaticJsonDocument<200> doc;
  doc["temp"] = random(250, 350) / 10.0;
  doc["hum"] = random(400, 800) / 10.0;

  char payload[128];
  serializeJson(doc, payload);

Serial.print("Topic: ");
Serial.println(mqtt_topic);

Serial.print("Payload: ");
Serial.println(payload);

if (client.publish(mqtt_topic, payload)) {
  Serial.println("Publish thanh cong");
} else {
  Serial.println("Publish that bai");
}

delay(5000);
}


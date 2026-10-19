#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

const char* ssid = "H2SO4+2NH3=Pass";
const char* password = "DuyenLinh";
const char* mqtt_server = "192.168.1.10";
const int mqtt_port = 1883;

const char* mqtt_user = "mqtt-user";
const char* mqtt_pass = "thao256600";
const char* student_id = "10124295";

String topic_cmd = "iot/lab2/10124295/cmd";
String topic_state = "iot/lab2/10124295/state";

WiFiClient espClient;
PubSubClient client(espClient);

// Cấu hình LED tích hợp NeoPixel trên GPIO 48 của ESP32-S3
#define PIN_RGB 48
#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN_RGB, NEO_GRB + NEO_KHZ800);

bool ledState = false;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Đang kết nối tới WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nĐã kết nối WiFi thành công!");
  Serial.print("Địa chỉ IP của ESP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  message.trim();

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(message);

  if (message == "ON") {
    ledState = true;
    pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Bật LED màu Xanh lá
    pixels.show();
  } else if (message == "OFF") {
    ledState = false;
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Tắt LED
    pixels.show();
  }

  client.publish(topic_state.c_str(), ledState ? "ON" : "OFF");
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP-Lab2-" + String(student_id);
    Serial.print("Đang kết nối MQTT tới Server: ");
    Serial.println(mqtt_server);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("MQTT connected thành công!");
      client.subscribe(topic_cmd.c_str());
      Serial.print("Đã Subscribed topic: ");
      Serial.println(topic_cmd);
    } else {
      Serial.print("MQTT kết nối thất bại, rc=");
      Serial.print(client.state());
      Serial.println(" (Thử lại sau 2 giây...)");
      delay(2000);
    }
  }
}

void setup() {
  pixels.begin();
  pixels.setBrightness(50);
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();

  Serial.begin(115200);
  delay(1000);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setKeepAlive(60);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
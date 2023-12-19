#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnectCore.h>
#include <PubSubClient.h>
#include <Servo.h>

WebServer Server;
AutoConnect Portal(Server);
AutoConnectConfig Config;

WiFiClient espClient;
PubSubClient client(espClient);

const char* mqtt_server = "broker.hivemq.com";
const char* topic = "sensor/atap";

const int pinHujan = 12;
const int pinLdr = 15;
const int pinServo = 4;

int tutup = 180;
int buka = 80;

Servo servo1;

void rootPage() {
  char content[] = "<a href=https://ti.polindra.ac.id>IoT Politeknik Negeri Indramayu</a>";
  Server.send(200, "text/html", content);
}

void callback(char* topic, byte* payload, unsigned int length) {
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "POLINDRA-IOT-2023-KELOMPOK-2-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      client.loop();
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Serial.println();

  Server.on("/", rootPage);

  Config.apid = "IOT-KELOMPOK-2";
  Config.hostName = "IOT-KELOMPOK-2"; 
  Config.title = "IOT-KELOMPOK-2";
  Config.autoReconnect = true;
  Config.reconnectInterval = 6;  // Seek interval time is 180[s].
  Portal.config(Config);

  Portal.begin();
  Serial.println("Web Server started:" + WiFi.localIP().toString());

  pinMode(pinHujan, INPUT);
  pinMode(pinLdr, INPUT);

  servo1.attach(pinServo);
}

void loop() {
  Portal.handleClient();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  kirimDataSensor();
}

void kirimDataSensor() {
  int hujanValue = digitalRead(pinHujan);
  int cahayaValue = digitalRead(pinLdr);

  Serial.print("Nilai Hujan: ");
  Serial.println(hujanValue == HIGH ? "Tidak Hujan" : "Hujan");
  Serial.print("Nilai Cahaya: ");
  Serial.println(cahayaValue == HIGH ? "Gelap" : "Terang");
  Serial.println(" ");

  // Logika kontrol servo
  String statusAtap;

  // Jika cahaya terang dan hujan
  if (cahayaValue == HIGH && hujanValue == HIGH) {
    servo1.write(tutup);
    Serial.println("Atap Tertutup");
    statusAtap = "Atap Tertutup";
  // Jika cahaya gelap dan hujan
  } else if (cahayaValue == HIGH && hujanValue == LOW) {
    servo1.write(tutup);
    Serial.println("Atap Tertutup");
    statusAtap = "Atap Tertutup";
    // Jika cahaya terang dan tidak hujan
  } else if (cahayaValue == LOW && hujanValue == HIGH) {
    servo1.write(buka);
    Serial.println("Atap Terbuka");
    statusAtap = "Atap Terbuka";
  } else {
    servo1.write(tutup);
    Serial.println("Atap Tertutup");
    statusAtap = "Atap Tertutup";
  }

  // Format pesan dengan menyatukan nilai sensor dan status atap
  String message = String(hujanValue) + '#' + String(cahayaValue) + '#' + statusAtap;

  // Kirim pesan ke topik "sensor/atap"
  client.publish(topic, message.c_str());

  delay(300);
}

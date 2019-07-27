#include <ESP8266WiFi.h>
#include <DHTesp.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

// Definições de valores de referencia
#define DHT_PIN D2
#define LED_PIN D4

// Variáveis globais
String client_id = "esp8266_vinialves08";
const char* ssid = "Iot Smart House";
const char* pass = "IotSmartHousei8#!";
float temperatura = 0.0f;
float umidade = 0.0f;
bool cmd_led = false;

// Informações de acesso ao MQTT
const char* mqtt_broker = "iot.eclipse.org";
const int mqtt_port = 1883;
const char* topic_sub = "iot/senai/101/modesto/cmd";
const char* topic_pub = "iot/senai/101/modesto/data";

//Objetos de suporte
WiFiClient espClient;
DHTesp dht;
DynamicJsonDocument doc(1024);
PubSubClient mqttClient(espClient);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  dht.setup(DHT_PIN, DHTesp::DHT22);
  Serial.println("Conectando ao WIFi...");
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi Conectado!!!");
  Serial.println(WiFi.localIP());
  
  mqttClient.setServer(mqtt_broker, mqtt_port);
  while(!mqttClient.connected()){
    if(mqttClient.connect(client_id.c_str())){
      Serial.println("Conectado ao Broker!");
    }else{
      Serial.print(".");
      delay(2000);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

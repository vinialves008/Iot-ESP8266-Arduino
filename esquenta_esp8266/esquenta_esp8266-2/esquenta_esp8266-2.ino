#include <ESP8266WiFi.h>
#include <DHTesp.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

// Definições de valores de referencia
#define DHT_PIN D2
#define LED_PIN D4

// Variáveis globais
const String client_id = "esp8266_vinialves08";
const char* ssid = "Iot Smart House";
const char* pass = "IotSmartHousei8#!";
float temperatura = 0.0f;
float umidade = 0.0f;
bool cmd_led = false;
int nr = 0;

// Informações de acesso ao MQTT
const char* mqtt_broker = "iot.eclipse.org";
const int mqtt_port = 1883;
const char* topic_sub = "iot/senai/101/modesto/cmd";
const char* topic_pub = "iot/senai/101/modesto/data";

//Manipulação de texto e dados
char serialMessage[1024];
char jsonBuffer[1024];

//Objetos de suporte
WiFiClient espClient;
DHTesp dht;
PubSubClient mqttClient(espClient);

//Declarações de Métodos
void check_connect_broker();
void callback(char* topic, byte* payload, unsigned int length);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  dht.setup(DHT_PIN, DHTesp::DHT22);
  Serial.print("Conectando ao WIFi...");
  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWifi Conectado!!!");
  Serial.println(WiFi.localIP());
  mqttClient.setServer(mqtt_broker, mqtt_port);
  mqttClient.setCallback(callback);
  check_connect_broker();
}

void loop() {
  // put your main code here, to run repeatedly:
  DynamicJsonDocument doc(128);

  check_connect_broker();
  mqttClient.loop();

  nr++;
  temperatura = dht.getTemperature();
  umidade = dht.getHumidity();

  if(isnan(temperatura) && isnan(umidade)){
    temperatura = umidade = 9999;
  }
  doc["NR"] = nr;
  doc["ID"] = client_id;
  doc["TEMP"] = temperatura;
  doc["UMID"] = umidade;
  serializeJson(doc, jsonBuffer);
  Serial.println(jsonBuffer);

  mqttClient.publish(topic_pub, jsonBuffer);

  delay(1000);
}
void check_connect_broker(){
  
  while(!mqttClient.connected()){
      Serial.print("Conectando ao Broker...");
    if(mqttClient.connect(client_id.c_str())){
      Serial.println("\nConectado ao Broker!");
      mqttClient.subscribe(topic_sub);
    }else{
      Serial.print(".");
      delay(2000);
    }
  }
}
void callback(char* topic, byte* payload, unsigned int length){
 String msg_mqtt;

 msg_mqtt = "";
 
  Serial.print("Msg: [");
  Serial.print(topic);
  Serial.print("] - ");

  for(int i = 0; i < length; i++){
    msg_mqtt += (char) payload[i];
  }
  Serial.println(msg_mqtt);
  if(msg_mqtt == "ON"){
    digitalWrite(LED_PIN, HIGH);
  }else if(msg_mqtt == "OFF"){
    digitalWrite(LED_PIN, LOW);
  }
  
}

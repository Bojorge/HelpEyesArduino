#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <ArduinoJson.h>


const char* ssid = "HUAWEI-B315-9255";
const char* password =  "F6LFARAH69H";
const char* mqttServer = "soldier.cloudmqtt.com";
const int mqttPort = 13129 ;
const char* mqttUser = "hjnnghph";
const char* mqttPassword = "tVp3MiDwDeUt";
const char* listen_topic1 = "/esp/test";
const char * clientID = "ESP8266Cl**hient";

char SALIDADIGITAL[50];
unsigned long previousMillis = 0;


WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length);











void setup() {
  pinMode(12, OUTPUT); // D6 salida digital
  digitalWrite(12, LOW);

  Serial.begin(115200);
  Serial.println("");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando al WiFi..");
  }
  Serial.println("Se ha conectado a la red WiFi");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Conectando a MQTT...");
    if (client.connect(clientID, mqttUser, mqttPassword )) {
      Serial.println("¡conectado!");
      //break;
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }

  }

  String salidaDigital = listen_topic1; 
  salidaDigital.toCharArray(SALIDADIGITAL, 50);
}


void callback(char* topic, byte* payload, unsigned int length) {
  char PAYLOAD[5] = "    ";
  
  Serial.print("Mensaje Recibido: [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    PAYLOAD[i] = (char)payload[i];
  }
  Serial.println(PAYLOAD);

  if (String(topic) ==  String(SALIDADIGITAL)) {
    if (payload[1] == 'N'){
     digitalWrite(12, HIGH);
    }
    if (payload[1] == 'F'){
      digitalWrite(12, LOW);
    }
  }
}

/**
  @brief Function for reconnecting to mqtt broker if connection is lost.
*/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Tratando conexion MQTT ... ");
    // Attempt to connect
    if (client.connect(clientID, mqttUser, mqttPassword )) { //client.connect(clientID, mqttUser, mqttPassword )
      Serial.println(" ¡conectado! ");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe(listen_topic1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}



void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.publish(listen_topic1, "Hello from ESP8266");
  client.subscribe(listen_topic1);
  client.loop();

 
  //Serial.println("Enviando algo");
  delay(3000);
}

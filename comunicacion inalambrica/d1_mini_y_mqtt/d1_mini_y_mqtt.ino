#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <SPI.h>
#include <DallasTemperature.h>

#define Pecho D6
#define Ptrig D7
#define Pin 2 //Se declara el pin D4 donde se conectará el sensor de temperatura
OneWire ourWire(Pin); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature
String temperatura;
long duracion, distancia;


const char* ssid = "HUAWEI-B315-9255";
const char* password =  "F6LFARAH69H";
const char* mqttServer = "soldier.cloudmqtt.com";
const int mqttPort = 13129 ;
const char* mqttUser = "hjnnghph";
const char* mqttPassword = "tVp3MiDwDeUt";
const char* topic1 = "sensorArduino";
const char* topic2 = "Advertencia";
const char * clientID = "ESP8266Cl**hient";

char SALIDADIGITAL[50];
//unsigned long previousMillis = 0;


WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length);


void setup() {
  Serial.begin(115200);
  sensors.begin(); //Se inician los sensores
  //pinMode(12, OUTPUT); // D6 salida digital
  digitalWrite(12, LOW);
  pinMode(Pecho, INPUT);     // define el pin 6 como entrada (echo)
  pinMode(Ptrig, OUTPUT);    // define el pin 7 como salida  (triger)
 


  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando al WiFi..");
  }
  Serial.println("Se ha conectado a la red WiFi");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Conectando a MQTT...\n");
    if (client.connect(clientID, mqttUser, mqttPassword )) {
      Serial.println("¡conectado a MQTT!");
      //break;
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }

  }

  String salidaDigital = topic2; 
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
    if (payload[1] == 'a'){
     //digitalWrite(12, HIGH);
    }
    else if (payload[1] == 'i'){
      //digitalWrite(12, LOW);
    }
    else if (PAYLOAD=="dark"){
      //digitalWrite(12, HIGH);
    }
    else if (PAYLOAD=="light"){
      //digitalWrite(12, LOW);
    }
    else if (PAYLOAD=="hot"){
      
    }
    else if (PAYLOAD=="cold"){
      
    }
    else if (PAYLOAD=="incL"){
      
    }
    else if (PAYLOAD=="incR"){
      
    }
    else if (PAYLOAD=="frte"){
      
    }
    else if (PAYLOAD=="esp"){
      
    }
    else if (PAYLOAD=="beat"){
      
    }
    else if (PAYLOAD=="warn"){
      
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
      //client.subscribe(topic2);
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
  sensors.requestTemperatures(); //Prepara el sensor para la lectura de temperatura
  float temperatura = sensors.getTempCByIndex(0);

  //digitalWrite(Ptrig, LOW);
  //delayMicroseconds(2);
  digitalWrite(Ptrig, HIGH);   // genera el pulso de triger por 10ms
  delayMicroseconds(10);
  digitalWrite(Ptrig, LOW);
  
  duracion = pulseIn(Pecho, HIGH);
  distancia = (duracion/2) / 29;            // calcula la distancia en centimetros

  
  if (!client.connected()) {
    reconnect();
  }
  client.subscribe(topic2);
    
  if (temperatura> 30){
    client.publish(topic1, "hot");
    //delay(100);
  }
  
  if (temperatura< 20){
    client.publish(topic1, "cold");
  }  

  if (distancia <= 30 && distancia >= 0){
    client.publish(topic1, "beat");
    Serial.println("Alarma.......");         // envia la palabra Alarma por el puerto serial
  }
  
  if (distancia <= 100 && distancia >= 31){
    client.publish(topic1, "warn");
    Serial.print("PELIGRO hay un obstaculo a menos de un metro");
  }
  if (distancia <= 500 && distancia >= 101){  // si la distancia es mayor a 100cm o menor a 500cm 
    Serial.print("hay un obstaculo a: ");
    Serial.print(distancia);
    Serial.println(" cm");
                     
  }
  //Serial.println(distancia);
   
  
  client.loop();
  
}

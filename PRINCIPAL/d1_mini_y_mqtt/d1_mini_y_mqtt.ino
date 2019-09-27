#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <SPI.h>
#include <DallasTemperature.h>
#include <OneWire.h>


#define Pecho D2
#define Ptrig D1
#define Ptrig2 D7
#define Pecho2 D6

long duracion, distancia, duracion2, distancia2;
/*
String temperatura;
#define Pin 2 //Se declara el pin D4 donde se conectará el sensor de temperatura
OneWire ourWire(Pin); //Se establece el pin declarado como bus para la comunicación OneWire
DallasTemperature sensors(&ourWire); //Se instancia la librería DallasTemperature

*/
//OneWire  ds(D4);

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
  
  //sensors.begin(); //Se inician los sensores
  //pinMode(12, OUTPUT); // D6 salida digital
  //digitalWrite(12, LOW);
  pinMode(Pecho, INPUT);     // define el pin D7 como entrada (echo)
  pinMode(Ptrig, OUTPUT);    // define el pin D6 como salida  (triger)
  pinMode(Ptrig2, OUTPUT);    // define el pin D6 como salida  (triger)
  pinMode(Pecho2, INPUT);     // define el pin D5 como entrada (echo)


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

  //avisar al usuario (usar vibradores y otras cosas)
  if (String(topic) ==  String(SALIDADIGITAL)) {    
    if (payload[0] == 'd'){
     //digitalWrite(12, HIGH);
     Serial.println("esta oscuro");
    }
    else if (payload[0] == 'l'){
      Serial.println("mucha luz");
      //digitalWrite(12, LOW);
    }
    else if (payload[0] == 'h'){
      Serial.println("muy cliente");
      
    }
    else if (payload[0] == 'c'){
      Serial.println("temperatura baja");
      
    }
    else if (payload[0] == 'L'){
      Serial.println("inclinacion hacia la izquierda");
      Serial.println(PAYLOAD);
    }
    else if (payload[0] == 'R'){
      Serial.println("inclinacion hacia la derecha");
    }
    else if (payload[0] == 'f'){
      Serial.println("caida de frente");
      }
    else if (payload[0] == 'e'){
      Serial.println("caida de espalda");
      }
    else if (payload[0] == 'x'){
      Serial.println("golpe asegurado");
      }
    else if (payload[0] == 'p'){
      Serial.println("obstaculo a menos de un metro");
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
  /*
  sensors.requestTemperatures(); //Prepara el sensor para la lectura de temperatura
  float temperatura = sensors.getTempCByIndex(0);
  */
  digitalWrite(Ptrig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ptrig, HIGH);   // genera el pulso de triger por 10ms
  delayMicroseconds(10);
  digitalWrite(Ptrig, LOW);

  digitalWrite(Ptrig2, LOW);
  delayMicroseconds(2);
  digitalWrite(Ptrig2, HIGH);   // genera el pulso de triger2 por 10ms
  delayMicroseconds(10);
  digitalWrite(Ptrig2, LOW);
  
  duracion = pulseIn(Pecho, HIGH);
  distancia = (duracion/2) / 29;            // calcula la distancia en centimetros

  duracion2 = pulseIn(Pecho2, HIGH);
  distancia2 = (duracion2/2) / 29;
  
  
  
  if (!client.connected()) {
    reconnect();
  }
  client.subscribe(topic2);

  client.loop();

  /*
    
  if (temperatura> 30){
    client.publish(topic1, "h");
  }
  
  if (temperatura< 20){
    client.publish(topic1, "c");
  }  
  */
   /*
  if (distancia <= 30 && distancia >= 0){
    client.publish(topic1, "x");
  }
  
  if (distancia <= 100 && distancia >= 31){
    client.publish(topic1, "p");
  }
  if (distancia <= 500 && distancia >= 101){  // si la distancia es mayor a 100cm o menor a 500cm 
    Serial.print("hay un obstaculo a: ");
    Serial.print(distancia);
    Serial.println(" cm");
                     
  }*/
  Serial.println(distancia);
  Serial.println(distancia2);
  
  /*
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
 
  if ( !ds.search(addr)) 
  {
    ds.reset_search();
    delay(250);
    return;
  }
 
  if (OneWire::crc8(addr, 7) != addr[7]) 
  {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();
 
  // the first ROM byte indicates which chip
  switch (addr[0]) 
  {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default:
      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 
 
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end  
  delay(1000);
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
 
  for ( i = 0; i < 9; i++) 
  {           
    data[i] = ds.read();
  }
 
  // Convert the data to actual temperature
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) 
    {
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } 
  else 
  {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
 
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print(fahrenheit);
  Serial.println(" Fahrenheit");
  
  */
  
  
}

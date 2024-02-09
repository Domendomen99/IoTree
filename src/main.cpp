#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <stdio.h>
#include <stdlib.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
//#include <string.h>

///////////////////////////////////////////////////////////
//-DATI RETE E CLIENT E SERVER MQTT
//const char* nomeRete = "MORE-IOT";
//const char* pwdRete = "MORE-IOT-PWD";
const char* nomeRete = "FASTWEB-1-A91631_EXT";
const char* pwdRete = "4452CC2BAB";
//IPAddress ip_scheda(192,168,0,5);
//IPAddress ip_server(192, 168, 0, 13);
IPAddress ip_scheda(192,168,1,104);
IPAddress ip_server(192,168,1,86);
///////////////////////////////////////////////////////////
int pinLetturaAnalogica = 34;
int pinLed1 = 25;
//int pinLed2 = 23;
///////////////////////////////////////////////////////////
//int luminosiaLimite1 = 1000;
///////////////////////////////////////////////////////////
int pinDHT = 26;
DHT dht(pinDHT,DHT11);
///////////////////////////////////////////////////////////
bool statusLight = true;
bool statusHum = true;
bool statusTemp = true;
int luminositaINT=0;
///////////////////////////////////////////////////////////


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Ingresso in CALLBACK");
  // handle message arrived
  Serial.print(" - Message arrived on topic : [");
  Serial.print(topic);
  Serial.print("] ->  ");
  String payloaSTR="";
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    payloaSTR = payloaSTR + (char)payload[i];
  }
  Serial.println("");
  String topicStr(topic);
  String stringaTopicLight = "domenicoRossini/IoTree/light";
  String stringaTopicHum = "domenicoRossini/IoTree/hum";
  String stringaTopicTemp = "domenicoRossini/IoTree/temp";
  String stringaTopicLightStatus = "domenicoRossini/IoTree/light/status";
  String stringaTopicHumStatus = "domenicoRossini/IoTree/hum/status";
  String stringaTopicTempStatus = "domenicoRossini/IoTree/temp/status";
  if(topicStr==stringaTopicLight){
    Serial.println("Ricevuto lightMSG");
    luminositaINT = payloaSTR.toInt();
    /*if(luminositaINT>luminosiaLimite1){
      digitalWrite(pinLed1,LOW);
      digitalWrite(pinLed2,HIGH);
    }else{
      digitalWrite(pinLed1,HIGH);
      digitalWrite(pinLed2,LOW);
    }*/
  }
  if(topicStr==stringaTopicHum){
    Serial.println("Ricevuto humMSG");
  }
  if(topicStr==stringaTopicTemp){
    Serial.println("Ricevuto tempMSG");
  }
  if(topicStr==stringaTopicTempStatus){
    Serial.println("Ricevuto tempStatMSG");
    if(payloaSTR.equals("false")){
      statusTemp = false;
    }else{
      statusTemp = true;
    }
  }
  if(topicStr==stringaTopicHumStatus){
    Serial.println("Ricevuto humStatMSG");
    if(payloaSTR.equals("false")){
      statusHum = false;
    }else{
      statusHum = true;
    }
  }
  if(topicStr==stringaTopicLightStatus){
    Serial.println("Ricevuto lightStatMSG");
    if(payloaSTR.equals("false")){
      statusLight = false;
    }else{
      statusLight = true;
    }
  }
  Serial.println();
}

///////////////////////////////////////////////////////////
WiFiClient wifiClient;
PubSubClient client(ip_server,1883,wifiClient);
///////////////////////////////////////////////////////////

void connessioneaWiFi(){
  Serial.println("Ingresso in CONNESSIONE_WIFI");
  WiFi.mode(WIFI_STA);
  WiFi.begin(nomeRete,pwdRete);
  Serial.println("\nTentativo di connessione alla rete");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
  }
  Serial.println("\nConnessione Stabilita con IP : ");
  Serial.println(WiFi.localIP());
}

void inizializzaLED(){
  Serial.println("Ingresso in InizializzaLED");
  pinMode(pinLed1,OUTPUT);
  //pinMode(pinLed2,OUTPUT);
  //digitalWrite(pinLed1,HIGH);
  //digitalWrite(pinLed2,LOW);
}

///////////////////////////////////////////////////////////

void setup() {
  ///////////////////////////////////////////////////////////
  // put your setup code here, to run once:
  //int result = myFunction(2, 3);
  ///////////////////////////////////////////////////////////
  Serial.begin(9600);
  Serial.println("Ingresso in SETUP");
  connessioneaWiFi();
  inizializzaLED();
  dht.begin();
  client.setKeepAlive(30000);
  client.setCallback(callback);
}

void iscrizioneTopic(){
  Serial.println("Ingresso in IscrizioneTOPIC");
  /*client.subscribe("inTopic");
  Serial.println("\n-> Iscrizione a topic (inTopic)");
  client.subscribe("outTopic");
  Serial.println("\n-> Iscrizione a topic (outTopic)");*/
  //client.subscribe("domenicoRossini/IoTree/light");
  //Serial.println("\n-> Iscrizione a topic (domenicoRossini/IoTree/light)");
  //client.subscribe("domenicoRossini/IoTree/temp");
  //Serial.println("\n-> Iscrizione a topic (domenicoRossini/IoTree/temp)");
  //client.subscribe("domenicoRossini/IoTree/hum");
  //Serial.println("\n-> Iscrizione a topic (domenicoRossini/IoTree/hum)");
  client.subscribe("domenicoRossini/IoTree/light/status");
  Serial.println("\n-> Iscrizione a topic (domenicoRossini/IoTree/light/status)");
  client.subscribe("domenicoRossini/IoTree/hum/status");
  Serial.println("\n-> Iscrizione a topic (domenicoRossini/IoTree/hum/status)");
  client.subscribe("domenicoRossini/IoTree/temp/status");
  Serial.println("\n-> Iscrizione a topic (domenicoRossini/IoTree/temp/status)");
}

void invioLuminositaMisurata(){
  Serial.println("Ingresso in invioLight");
  //client.publish("outTopic","hello_world");
  //Serial.println("\n-> Invio su topic (outTopic) -> (hello_world)");
  int lightINT = analogRead(pinLetturaAnalogica);

  Serial.print("Luce  : ");
  Serial.print(lightINT);
  Serial.println("");

  char lightChar[5];
  snprintf(lightChar, sizeof(lightChar), "%d", lightINT);
  //client.publish("domenicoRossini/test",lightStringa);
  client.publish("domenicoRossini/IoTree/light",lightChar);
  //client.publish("domenicoRossini/IoTree/light","1000");
  //Serial.println("\n-> Invio su topic (domenicoRossini/lightSensor) -> (lightStringa)");
}

void invioTemperaturaMisurata(){
  Serial.println("Ingresso in invioTemp");
  int t = dht.readTemperature();
  
  Serial.print("Temp : ");
  Serial.print(t);
  Serial.println("");

  char tChar[5];
  snprintf(tChar, sizeof(tChar), "%d", t);
  client.publish("domenicoRossini/IoTree/temp",tChar);
}

void invioUmiditaMisurata(){
  Serial.println("Ingresso in invioHum");
  int h = dht.readHumidity();

  Serial.print("Umidità : ");
  Serial.print(h);
  Serial.println("");

  char hChar[5];
  snprintf(hChar, sizeof(hChar), "%d", h);
  client.publish("domenicoRossini/IoTree/hum",hChar);
}

void reconnect() {
  Serial.println("Ingresso in reconnect");
  // Loop until we're reconnected

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect

    // codice per connessione con nome casuale
    //String clientId = "ESP8266Client-";
    //clientId += String(random(0xffff), HEX);
    
    if (client.connect("#monitorIoTree#")){
      Serial.println("\n-> Connessione Stabilita");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 second");
      delay(1000);
    }
  }
  
}

void invioValoriMisurati(){
  if(statusLight){
    invioLuminositaMisurata();
  }
  if(statusTemp){
    invioTemperaturaMisurata();
  }
  if(statusHum){
    invioUmiditaMisurata();
  }
  digitalWrite(pinLed1,HIGH);
  delay(1000);
  digitalWrite(pinLed1,LOW);
}

void loop() {
  if (!client.connected()) {
    reconnect();
    iscrizioneTopic();
  }
  client.loop();
  Serial.println("Inizio del LOOP");
  invioValoriMisurati();
  delay(5000);
  Serial.println("Fine del LOOP");
  Serial.println("");
}

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Ljubo";
const char* password = "H4kXnyZ4";
const char* mqttServer = "m24.cloudmqtt.com";
const int mqttPort = 10396; 
const char* mqttPassword = "AAdW5aQnAHgD";
const char* mqttUser = "rzgylfzg";
int prevTemp = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("Establishing Connection to: ");
    Serial.println(ssid);
  }

  Serial.println("Connected");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()){
    Serial.println("Connecting to MQTT...");

    if(client.connect("ESP8266Client", mqttUser, mqttPassword))
      Serial.println("Connected");
    else {
      Serial.println("Failed");
      delay(2000);
    }
  }

  client.subscribe("esp/test");
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for(int i = 0; i < length; i++)
    Serial.print((char)payload[i]);
    
  Serial.println();
  Serial.println("====================================");
}

void loop() {
  client.loop();
  
  float raw = analogRead(0);
  float mV = (raw / 1024) * 3300;
  float temp = mV / 10; 

  
  
  char msg[8];
  dtostrf(temp, 6, 2, msg);

  client.publish("esp/tmp", msg);

  if(temp > prevTemp)
  client.publish("esp/tmpChange", "1");

  if(temp <= prevTemp)
  client.publish("esp/tmpChange", "0");

  prevTemp = temp;


  
  delay(10000);
}


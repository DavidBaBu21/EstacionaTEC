#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);
const char* ssid = "Totalplay-2.4G-44a8";
const char* password = "";
const char* mqtt_server = "192.168.100.16";
const char* mqtt_server2 = "127.0.0.1";

void setup() {
  setup_wifi();
  delay(500);
  client.setServer(mqtt_server, 1883);
  delay(1000);
}

void loop() {
  
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  client.publish("canal", "1");
  delay(3000);
}

void setup_wifi(){
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());
}

void reconnect(){
  Serial.print("Trying MQTT conection...");
  if(client.connect("ESP32")){
    Serial.println("Conexión exitosa con el broker");
  }
  else{
    Serial.println("Conexión fallida");
    Serial.print("Fallo, rc=");
    Serial.println(client.state());
  }
}
